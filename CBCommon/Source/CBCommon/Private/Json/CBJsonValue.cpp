// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "Json/CBJsonValue.h"

#include "CBCommon.h"
#include "Json/CBJsonObject.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"

typedef TJsonWriter< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriter;
typedef TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriterFactory;

bool FJsonValueBinary::IsBinary(const TSharedPtr<FJsonValue>& InJsonValue)
{
	if (!InJsonValue.IsValid())
	{
		return false;
	}

	bool IgnoreBool;
	return !InJsonValue->TryGetBool(IgnoreBool);
}

TArray<uint8> FJsonValueBinary::AsBinary(const TSharedPtr<FJsonValue>& InJsonValue)
{
	if (FJsonValueBinary::IsBinary(InJsonValue))
	{
		TSharedPtr<FJsonValueBinary> BinaryValue = StaticCastSharedPtr<FJsonValueBinary>(InJsonValue);

		return BinaryValue->AsBinary();
	}
	else if(InJsonValue->Type==EJson::String)
	{
		//如果我们通过 socket.io 协议 hack 得到一个未被检测为二进制的字符串,需要将这个字符串解码为 base 64
		TArray<uint8> DecodedArray;
		bool bDidDecodeCorrectly = FBase64::Decode(InJsonValue->AsString(), DecodedArray);
		if (!bDidDecodeCorrectly)
		{
			UE_LOG(LogCBJson, Warning, TEXT("FJsonValueBinary::AsBinary couldn't decode %s as a binary."), *InJsonValue->AsString());
		}
		return DecodedArray;
	}
	else
	{
		TArray<uint8> EmptyArray;
		return EmptyArray;
	}
}

UCBJsonValue::UCBJsonValue(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

UCBJsonValue* UCBJsonValue::ConstructJsonValueNumber(float Number)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueNumber(Number));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValueString(const FString& StringValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueString(StringValue));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValueBool(bool InValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueBoolean(InValue));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValueArray(const TArray<UCBJsonValue*>& InArray)
{
	// Prepare data array to create new value
	TArray< TSharedPtr<FJsonValue> > ValueArray;
	for (auto InVal : InArray)
	{
		ValueArray.Add(InVal->GetRootValue());
	}

	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueArray(ValueArray));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValueObject(UCBJsonObject* JsonObject)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueObject(JsonObject->GetRootObject()));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValueBinary(TArray<uint8> ByteArray)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueBinary(ByteArray));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ConstructJsonValue(const TSharedPtr<FJsonValue>& InValue)
{
	TSharedPtr<FJsonValue> NewVal = InValue;

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonValue::ValueFromJsonString( const FString& StringValue)
{
	TSharedPtr<FJsonValue> NewVal =JsonStringToJsonValue(StringValue);

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

TSharedPtr<FJsonValue>& UCBJsonValue::GetRootValue()
{
	return JsonVal;
}

void UCBJsonValue::SetRootValue(TSharedPtr<FJsonValue>& JsonValue)
{
	JsonVal = JsonValue;
}

ECBJson::Type UCBJsonValue::GetType() const
{
	if (!JsonVal.IsValid())
	{
		return ECBJson::None;
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return ECBJson::None;

	case EJson::Null:
		return ECBJson::Null;

	case EJson::String:
		if (FJsonValueBinary::IsBinary(JsonVal))
		{
			return ECBJson::Binary;
		}
		else
		{
			return ECBJson::String;
		}
	case EJson::Number:
		return ECBJson::Number;

	case EJson::Boolean:
		return ECBJson::Boolean;

	case EJson::Array:
		return ECBJson::Array;

	case EJson::Object:
		return ECBJson::Object;

	default:
		return ECBJson::None;
	}
}

FString UCBJsonValue::GetTypeString() const
{
	if (!JsonVal.IsValid())
	{
		return "None";
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return TEXT("None");

	case EJson::Null:
		return TEXT("Null");

	case EJson::String:
		return TEXT("String");

	case EJson::Number:
		return TEXT("Number");

	case EJson::Boolean:
		return TEXT("Boolean");

	case EJson::Array:
		return TEXT("Array");

	case EJson::Object:
		return TEXT("Object");

	default:
		return TEXT("None");
	}
}

bool UCBJsonValue::IsNull() const
{
	if (!JsonVal.IsValid())
	{
		return true;
	}

	return JsonVal->IsNull();
}

float UCBJsonValue::AsNumber() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Number"));
		return 0.f;
	}

	return JsonVal->AsNumber();
}

FString UCBJsonValue::AsString() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("String"));
		return FString();
	}

	//Auto-convert non-strings instead of getting directly
	if (JsonVal->Type != EJson::String)
	{
		return EncodeJson();
	}
	else
	{
		return JsonVal->AsString();
	}
}

bool UCBJsonValue::AsBool() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Boolean"));
		return false;
	}

	return JsonVal->AsBool();
}

TArray<UCBJsonValue*> UCBJsonValue::AsArray() const
{
	TArray<UCBJsonValue*> OutArray;

	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Array"));
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonVal->AsArray();
	for (auto Value : ValArray)
	{
		UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
		NewValue->SetRootValue(Value);

		OutArray.Add(NewValue);
	}

	return OutArray;
}

UCBJsonObject* UCBJsonValue::AsObject()
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Object"));
		return nullptr;
	}

	TSharedPtr<FJsonObject> NewObj = JsonVal->AsObject();

	UCBJsonObject* JsonObj = NewObject<UCBJsonObject>();
	JsonObj->SetRootObject(NewObj);

	return JsonObj;
}

TArray<uint8> UCBJsonValue::AsBinary()
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Binary"));
		TArray<uint8> ByteArray;
		return ByteArray;
	}

	//binary object pretending & starts with non-json format? it's our disguise binary
	if (JsonVal->Type == EJson::String)
	{
		//it's a legit binary
		if (FJsonValueBinary::IsBinary(JsonVal))
		{
			//Valid binary available
			return FJsonValueBinary::AsBinary(JsonVal);
		}

		//It's a string, decode as if hex encoded binary
		else
		{
			const FString& HexString = JsonVal->AsString();

			TArray<uint8> ByteArray;
			ByteArray.AddUninitialized(HexString.Len() / 2);

			bool DidConvert = FString::ToHexBlob(HexString, ByteArray.GetData(), ByteArray.Num());

			//Empty our array if conversion failed
			if (!DidConvert)
			{
				ByteArray.Empty();
			}
			return ByteArray;
		}
	}
	//Not a binary nor binary string, return empty array
	else
	{
		//Empty array
		TArray<uint8> ByteArray;
		return ByteArray;
	}
}

FString UCBJsonValue::EncodeJson() const
{
	return ToJsonString(JsonVal);
}

TSharedPtr<FJsonValue> UCBJsonValue::JsonStringToJsonValue(const FString& JsonString)
{
	//Null
	if (JsonString.IsEmpty())
	{
		return MakeShareable(new FJsonValueNull);
	}

	//Number
	if (JsonString.IsNumeric())
	{
		//convert to double
		return MakeShareable(new FJsonValueNumber(FCString::Atod(*JsonString)));
	}

	//Object
	if (JsonString.StartsWith(FString(TEXT("{"))))
	{
		TSharedPtr< FJsonObject > JsonObject = ToJsonObject(JsonString);
		return MakeShareable(new FJsonValueObject(JsonObject));
	}

	//Array
	if (JsonString.StartsWith(FString(TEXT("["))))
	{
		TArray < TSharedPtr<FJsonValue>> RawJsonValueArray;
		TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
		bool success = FJsonSerializer::Deserialize(Reader, RawJsonValueArray);

		if (success)
		{
			return MakeShareable(new FJsonValueArray(RawJsonValueArray));
		}
	}

	//Bool
	if (JsonString == FString("true") || JsonString == FString("false"))
	{
		bool BooleanValue = (JsonString == FString("true"));
		return MakeShareable(new FJsonValueBoolean(BooleanValue));
	}

	//String
	return MakeShareable(new FJsonValueString(JsonString));
}

TSharedPtr<FJsonObject> UCBJsonValue::ToJsonObject(const FString& JsonString)
{
	TSharedPtr< FJsonObject > JsonObject = MakeShareable(new FJsonObject);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
	FJsonSerializer::Deserialize(Reader, JsonObject);
	return JsonObject;
}

FString UCBJsonValue::ToJsonString(const TSharedPtr<FJsonObject>& JsonObject)
{
	FString OutputString;
	TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return OutputString;
}

FString UCBJsonValue::ToJsonString(const TSharedPtr<FJsonValue>& JsonValue)
{
	if(JsonValue->Type == EJson::None)
	{
		return FString();
	}
	else if(JsonValue->Type == EJson::Null)
	{
		return FString();
	}
	else if(JsonValue ->Type == EJson::String)
	{
		return JsonValue->AsString();
	}
	else if(JsonValue ->Type == EJson::Number)
	{
		return FString::Printf(TEXT("%f"), JsonValue->AsNumber());
	}
	else if (JsonValue->Type == EJson::Boolean)
	{
		return FString::Printf(TEXT("%d"), JsonValue->AsBool());
	}
	else if (JsonValue->Type == EJson::Array)
	{
		return ToJsonString(JsonValue->AsArray());
	}
	else if (JsonValue->Type == EJson::Object)
	{
		return ToJsonString(JsonValue->AsObject());
	}
	else
	{
		return FString();
	}
}

FString UCBJsonValue::ToJsonString(const TArray<TSharedPtr<FJsonValue>>& JsonValueArray)
{
	FString OutputString;
	TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
	FJsonSerializer::Serialize(JsonValueArray, Writer);
	return OutputString;
}

void UCBJsonValue::ErrorMessage(const FString& InType) const
{
	UE_LOG(LogCBJson, Error, TEXT("Json Value of type '%s' used as a '%s'."), *GetTypeString(), *InType);
}
