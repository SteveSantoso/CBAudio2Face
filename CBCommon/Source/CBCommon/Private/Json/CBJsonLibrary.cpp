// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "Json/CBJsonLibrary.h"

#include "CBCommon.h"
#include "Json/CBJsonValue.h"
#include "Kismet/KismetStringLibrary.h"
#include "Library/CBFileHeplerBPLibrary.h"
#include "Misc/Base64.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FString UCBJsonLibrary::PercentEncode(const FString& Source)
{
	
	FString OutText = Source;

	OutText = OutText.Replace(TEXT(" "), TEXT("%20"));
	OutText = OutText.Replace(TEXT("!"), TEXT("%21"));
	OutText = OutText.Replace(TEXT("\""), TEXT("%22"));
	OutText = OutText.Replace(TEXT("#"), TEXT("%23"));
	OutText = OutText.Replace(TEXT("$"), TEXT("%24"));
	OutText = OutText.Replace(TEXT("&"), TEXT("%26"));
	OutText = OutText.Replace(TEXT("'"), TEXT("%27"));
	OutText = OutText.Replace(TEXT("("), TEXT("%28"));
	OutText = OutText.Replace(TEXT(")"), TEXT("%29"));
	OutText = OutText.Replace(TEXT("*"), TEXT("%2A"));
	OutText = OutText.Replace(TEXT("+"), TEXT("%2B"));
	OutText = OutText.Replace(TEXT(","), TEXT("%2C"));
	OutText = OutText.Replace(TEXT("/"), TEXT("%2F"));
	OutText = OutText.Replace(TEXT(":"), TEXT("%3A"));
	OutText = OutText.Replace(TEXT(";"), TEXT("%3B"));
	OutText = OutText.Replace(TEXT("="), TEXT("%3D"));
	OutText = OutText.Replace(TEXT("?"), TEXT("%3F"));
	OutText = OutText.Replace(TEXT("@"), TEXT("%40"));
	OutText = OutText.Replace(TEXT("["), TEXT("%5B"));
	OutText = OutText.Replace(TEXT("]"), TEXT("%5D"));
	OutText = OutText.Replace(TEXT("{"), TEXT("%7B"));
	OutText = OutText.Replace(TEXT("}"), TEXT("%7D"));

	return OutText;
}

FString UCBJsonLibrary::Base64Encode(const FString& Source)
{
	return FBase64::Encode(Source);
}

FString UCBJsonLibrary::Base64EncodeBytes(const TArray<uint8>& Source)
{
	return FBase64::Encode(Source);
}

bool UCBJsonLibrary::Base64Decode(const FString& Source, FString& Dest)
{
	return FBase64::Decode(Source, Dest);
}

bool UCBJsonLibrary::Base64DecodeBytes(const FString& Source, TArray<uint8>& Dest)
{
	return FBase64::Decode(Source, Dest);
}

bool UCBJsonLibrary::StringToJsonValueArray(const FString& JsonString, TArray<UCBJsonValue*>& OutJsonValueArray)
{
	TArray < TSharedPtr<FJsonValue>> RawJsonValueArray;
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
	FJsonSerializer::Deserialize(Reader, RawJsonValueArray);

	for (auto Value : RawJsonValueArray)
	{
		auto SJsonValue = NewObject<UCBJsonValue>();
		SJsonValue->SetRootValue(Value);
		OutJsonValueArray.Add(SJsonValue);
	}

	return OutJsonValueArray.Num() > 0;
}

UCBJsonValue* UCBJsonLibrary::Conv_ArrayToJsonValue(const TArray<UCBJsonValue*>& InArray)
{
	return UCBJsonValue::ConstructJsonValueArray(InArray);
}

UCBJsonValue* UCBJsonLibrary::Conv_JsonObjectToJsonValue(UCBJsonObject* InObject)
{
	return UCBJsonValue::ConstructJsonValueObject(InObject);
}

UCBJsonValue* UCBJsonLibrary::Conv_BytesToJsonValue(const TArray<uint8>& InBytes)
{
	return UCBJsonValue::ConstructJsonValueBinary( InBytes);
}

UCBJsonValue* UCBJsonLibrary::Conv_StringToJsonValue(const FString& InString)
{
	return UCBJsonValue::ConstructJsonValueString(InString);
}

UCBJsonValue* UCBJsonLibrary::Conv_IntToJsonValue(int32 InInt)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueNumber(InInt));

	UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UCBJsonValue* UCBJsonLibrary::Conv_FloatToJsonValue(float InFloat)
{
	return UCBJsonValue::ConstructJsonValueNumber(InFloat);
}

UCBJsonValue* UCBJsonLibrary::Conv_BoolToJsonValue(bool InBool)
{
	return UCBJsonValue::ConstructJsonValueBool(InBool);
}

FString UCBJsonLibrary::Conv_JsonValueToString(UCBJsonValue* InValue)
{
	if (InValue)
	{
		return InValue->AsString();
	}

	return TEXT("");
}

int32 UCBJsonLibrary::Conv_JsonValueToInt(UCBJsonValue* InValue)
{
	
	if(InValue)
	{
		return (int32)InValue->AsNumber();
	}

	return 0;
}

float UCBJsonLibrary::Conv_JsonValueToFloat(UCBJsonValue* InValue)
{
	if (InValue)
	{
		return InValue->AsNumber();
	}

	return 0.f;
}

bool UCBJsonLibrary::Conv_JsonValueToBool(UCBJsonValue* InValue)
{
	if (InValue)
	{
		return InValue->AsBool();
	}

	return false;
}

TArray<uint8> UCBJsonLibrary::Conv_JsonValueToBytes(UCBJsonValue* InValue)
{
	if (InValue)
	{
		return InValue->AsBinary();
	}

	return TArray<uint8>();
}

UCBJsonObject* UCBJsonLibrary::ConvString_ToJsonObject(const FString& InString)
{
	TSharedPtr<FJsonObject> JsonObj;
	UCBJsonObject* Json=NewObject<UCBJsonObject>();
	
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*InString);
	if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
	{
		Json->SetRootObject(JsonObj);
		return Json;
	}
	
	UE_LOG(LogCBJson, Error, TEXT("Json decoding failed for: %s"), *InString);
	return  Json;
}

UCBJsonObject* UCBJsonLibrary::Conv_JsonValueToJsonObject(UCBJsonValue* InValue)
{
	if(InValue)
	{
		return InValue->AsObject();
	}

	return nullptr;
}

bool UCBJsonLibrary::LoadJsonFromFile(UCBJsonObject*& JsonObject, FString FilePath)
{
	JsonObject=NewObject<UCBJsonObject>();
	TSharedPtr<FJsonObject> RootJson;
	if(!DeserializeJsonFromFile(FilePath,RootJson))
	{
		return false;
	}

	JsonObject->SetRootObject(RootJson);
	return true;
}


bool UCBJsonLibrary::DeserializeJson(const FString& JsonString, TSharedPtr<FJsonObject>& JsonObject)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if(!FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		UE_LOG(LogCBJson, Error, TEXT("Failed to deserialize JSON string"));
		
		return  false;
	}
	
	return true;
}

bool UCBJsonLibrary::DeserializeJsonFromFile(FString FilePath, TSharedPtr<FJsonObject>& JsonObject)
{
	//注意：此操作只针对放入iVisual插件对应文件夹有用
	const FString PathPrefix="LocalData://";
	if((UKismetStringLibrary::StartsWith(FilePath,PathPrefix,ESearchCase::IgnoreCase)))
	{
		FString File=UKismetStringLibrary::RightChop(FilePath,PathPrefix.Len());
		FilePath=FPaths::Combine(UCBFileHeplerBPLibrary::GetExtrasDataDir(TEXT("CBCommon"))/File);
	}

	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogCBJson, Error, TEXT("No Valid Json File Found At %s"), *FilePath);
		return false;
	}
	
	FString Contents;
	if(!FFileHelper::LoadFileToString(Contents,*FilePath))
	{
		return false;
	}
	
	return DeserializeJson(Contents,JsonObject);
}


