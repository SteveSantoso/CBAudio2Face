// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "Json/CBJsonObject.h"

#include "CBCommon.h"
#include "Json/CBJsonValue.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

typedef TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriterFactory;
typedef TJsonWriter< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriter;

UCBJsonObject::UCBJsonObject(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{
    Reset();
}


UCBJsonObject* UCBJsonObject::ConstructJsonObject(UObject* WorldContextObject)
{
    return NewObject<UCBJsonObject>(WorldContextObject);
}

void UCBJsonObject::Reset()
{
    if (JsonObj.IsValid())
    {
        JsonObj.Reset();
    }

    JsonObj = MakeShareable(new FJsonObject());
}

FString UCBJsonObject::EncodeJson(bool IsCondensedString/*=true*/) const
{
    if (!JsonObj.IsValid())
    {
        return TEXT("");
    }

    FString OutputString;
    if (IsCondensedString)
    {
        TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
    }
    else
    {
        TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
    }

    return OutputString;
}

FString UCBJsonObject::EncodeJsonToSingleString() const
{
    FString OutputString = EncodeJson();

    // Remove line terminators
    (void)OutputString.Replace(LINE_TERMINATOR, TEXT(""));

    // Remove tabs
    (void)OutputString.Replace(LINE_TERMINATOR, TEXT("\t"));

    return OutputString;
}

bool UCBJsonObject::DecodeJson(const FString& JsonString)
{
    TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
    if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
    {
        return true;
    }
    else
    {
        UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
        NewValue->ValueFromJsonString(JsonString);
        if(!NewValue->GetRootValue())
        {
            // If we've failed to deserialize the string, we should clear our internal data
            Reset();

            UE_LOG(LogCBJson, Error, TEXT("Json decoding failed for: %s"), *JsonString);

            return false;
        }
        JsonObj=NewValue->AsObject()->GetRootObject();
        return  true;
    }

   
}

TArray<FString> UCBJsonObject::GetFieldNames()
{
    TArray<FString> Result;

    if (!JsonObj.IsValid())
    {
        return Result;
    }

    JsonObj->Values.GetKeys(Result);

    return Result;
}

bool UCBJsonObject::HasField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return false;
    }

    return JsonObj->HasField(FieldName);
}

void UCBJsonObject::RemoveField(const FString& FieldName)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->RemoveField(FieldName);
}

UCBJsonValue* UCBJsonObject::GetField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return nullptr;
    }

    TSharedPtr<FJsonValue> NewVal = JsonObj->TryGetField(FieldName);
    if (NewVal.IsValid())
    {
        UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
        NewValue->SetRootValue(NewVal);

        return NewValue;
    }

    return nullptr;
}

void UCBJsonObject::SetField(const FString& FieldName, UCBJsonValue* JsonValue)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->SetField(FieldName, JsonValue->GetRootValue());
}

TArray<UCBJsonValue*> UCBJsonObject::GetArrayField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Array"), *FieldName);
    }

    TArray<UCBJsonValue*> OutArray;
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return OutArray;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
    for (auto Value : ValArray)
    {
        UCBJsonValue* NewValue = NewObject<UCBJsonValue>();
        NewValue->SetRootValue(Value);

        OutArray.Add(NewValue);
    }

    return OutArray;
}

void UCBJsonObject::SetArrayField(const FString& FieldName, const TArray<UCBJsonValue*>& InArray)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray;

    // Process input array and COPY original values
    for (auto InVal : InArray)
    {
        TSharedPtr<FJsonValue> JsonVal = InVal->GetRootValue();

        switch (InVal->GetType())
        {
        case ECBJson::None:
            break;

        case ECBJson::Null:
            ValArray.Add(MakeShareable(new FJsonValueNull()));
            break;

        case ECBJson::String:
            ValArray.Add(MakeShareable(new FJsonValueString(JsonVal->AsString())));
            break;

        case ECBJson::Number:
            ValArray.Add(MakeShareable(new FJsonValueNumber(JsonVal->AsNumber())));
            break;

        case ECBJson::Boolean:
            ValArray.Add(MakeShareable(new FJsonValueBoolean(JsonVal->AsBool())));
            break;

        case ECBJson::Array:
            ValArray.Add(MakeShareable(new FJsonValueArray(JsonVal->AsArray())));
            break;

        case ECBJson::Object:
            ValArray.Add(MakeShareable(new FJsonValueObject(JsonVal->AsObject())));
            break;

        default:
            break;
        }
    }

    JsonObj->SetArrayField(FieldName, ValArray);
}

void UCBJsonObject::MergeJsonObject(UCBJsonObject* InJsonObject, bool Overwrite)
{
    TArray<FString> Keys = InJsonObject->GetFieldNames();

    for (auto Key : Keys)
    {
        if (Overwrite == false && HasField(Key))
        {
            continue;
        }

        SetField(Key, InJsonObject->GetField(Key));
    }
}

float UCBJsonObject::GetNumberField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Number>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Number"), *FieldName);
        return 0.0f;
    }

    return JsonObj->GetNumberField(FieldName);
}

void UCBJsonObject::SetNumberField(const FString& FieldName, float Number)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->SetNumberField(FieldName, Number);
}

FString UCBJsonObject::GetStringField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::String>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type String"), *FieldName);
        return TEXT("");
    }

    return JsonObj->GetStringField(FieldName);
}

void UCBJsonObject::SetStringField(const FString& FieldName, const FString& StringValue)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->SetStringField(FieldName, StringValue);
}

bool UCBJsonObject::GetBoolField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Boolean>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Boolean"), *FieldName);
        return false;
    }

    return JsonObj->GetBoolField(FieldName);
}

void UCBJsonObject::SetBoolField(const FString& FieldName, bool InValue)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->SetBoolField(FieldName, InValue);
}

UCBJsonObject* UCBJsonObject::GetObjectField(const FString& FieldName) const
{
    if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Object>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Object"), *FieldName);
        return nullptr;
    }

    TSharedPtr<FJsonObject> JsonObjField = JsonObj->GetObjectField(FieldName);

    UCBJsonObject* OutRestJsonObj = NewObject<UCBJsonObject>();
    OutRestJsonObj->SetRootObject(JsonObjField);

    return OutRestJsonObj;
}

void UCBJsonObject::SetObjectField(const FString& FieldName, UCBJsonObject* JsonObject)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    JsonObj->SetObjectField(FieldName, JsonObject->GetRootObject());
}

void UCBJsonObject::GetBinaryField(const FString& FieldName, TArray<uint8>& OutBinary) const
{
    if (!JsonObj->HasTypedField<EJson::String>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type String"), *FieldName);
    }
    TSharedPtr<FJsonValue> JsonValue = JsonObj->TryGetField(FieldName);

    if (FJsonValueBinary::IsBinary(JsonValue))
    {
        OutBinary = FJsonValueBinary::AsBinary(JsonValue);
    }
    else if (JsonValue->Type == EJson::String)
    {
        //If we got a string that isn't detected as a binary via socket.io protocol hack
        //then we need to decode this string as base 64
        TArray<uint8> DecodedArray;
        bool bDidDecodeCorrectly = FBase64::Decode(JsonValue->AsString(), DecodedArray);
        if (!bDidDecodeCorrectly)
        {
            UE_LOG(LogCBJson, Warning, TEXT("USIOJsonObject::GetBinaryField couldn't decode %s as a binary."), *JsonValue->AsString());
        }
        OutBinary = DecodedArray;
    }
    else
    {
        TArray<uint8> EmptyArray;
        OutBinary = EmptyArray;
    }
}

void UCBJsonObject::SetBinaryField(const FString& FieldName, const TArray<uint8>& Bytes)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }
    TSharedPtr<FJsonValueBinary> JsonValue = MakeShareable(new FJsonValueBinary(Bytes));
    JsonObj->SetField(FieldName, JsonValue);
}

TArray<float> UCBJsonObject::GetNumberArrayField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Array"), *FieldName);
    }

    TArray<float> NumberArray;
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return NumberArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        auto Value = (*It).Get();
        if (Value->Type != EJson::Number)
        {
            UE_LOG(LogCBJson, Error, TEXT("Not Number element in array with field name %s"), *FieldName);
        }

        NumberArray.Add((*It)->AsNumber());
    }

    return NumberArray;
}

void UCBJsonObject::SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;

    for (auto Number : NumberArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueNumber(Number)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<FString> UCBJsonObject::GetStringArrayField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Array"), *FieldName);
    }

    TArray<FString> StringArray;
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return StringArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        auto Value = (*It).Get();
        if (Value->Type != EJson::String)
        {
            UE_LOG(LogCBJson, Error, TEXT("Not String element in array with field name %s"), *FieldName);
        }

        StringArray.Add((*It)->AsString());
    }

    return StringArray;
}

void UCBJsonObject::SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;
    for (auto String : StringArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<bool> UCBJsonObject::GetBoolArrayField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Array"), *FieldName);
    }

    TArray<bool> BoolArray;
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return BoolArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        auto Value = (*It).Get();
        if (Value->Type != EJson::Boolean)
        {
            UE_LOG(LogCBJson, Error, TEXT("Not Boolean element in array with field name %s"), *FieldName);
        }

        BoolArray.Add((*It)->AsBool());
    }

    return BoolArray;
}

void UCBJsonObject::SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;
    for (auto Boolean : BoolArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueBoolean(Boolean)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<UCBJsonObject*> UCBJsonObject::GetObjectArrayField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
    {
        UE_LOG(LogCBJson, Warning, TEXT("No field with name %s of type Array"), *FieldName);
    }

    TArray<UCBJsonObject*> OutArray;
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return OutArray;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
    for (auto Value : ValArray)
    {
        if (Value->Type != EJson::Object)
        {
            UE_LOG(LogCBJson, Error, TEXT("Not Object element in array with field name %s"), *FieldName);
        }

        TSharedPtr<FJsonObject> NewObj = Value->AsObject();

        UCBJsonObject* NewJson = NewObject<UCBJsonObject>();
        NewJson->SetRootObject(NewObj);

        OutArray.Add(NewJson);
    }

    return OutArray;
}

void UCBJsonObject::SetObjectArrayField(const FString& FieldName, const TArray<UCBJsonObject*>& ObjectArray)
{
    if (!JsonObj.IsValid() || FieldName.IsEmpty())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;
    for (auto Value : ObjectArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueObject(Value->GetRootObject())));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TSharedPtr<FJsonObject>& UCBJsonObject::GetRootObject()
{
    return JsonObj;
}

void UCBJsonObject::SetRootObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    JsonObj = JsonObject;
}
