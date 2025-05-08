// Copyright 2023 CB All Rights Reserved.


#include "CBHTTPResponse.h"

#include "Json/CBJsonObject.h"
#include "Interfaces/IHttpResponse.h"

UCBHTTPResponse::UCBHTTPResponse()
	: Super()
	, RequestDuration(0.f)
{
	
}

void UCBHTTPResponse::GetContent(TArray<uint8>& OutContent) const
{
	
	if (Response)
	{
		OutContent = Response->GetContent();
	}
}

FString UCBHTTPResponse::GetContentAsString() const
{
	if (Response)
	{
		return Response->GetContentAsString();
	}
	return TEXT("");
}

UCBJsonObject* UCBHTTPResponse::GetContentAsJson() const
{
	UCBJsonObject* JsonObj = NewObject<UCBJsonObject>();
	if (Response)
	{
		if(JsonObj->DecodeJson(Response->GetContentAsString()))
		{
			return  JsonObj;
		}

		return  JsonObj;
	}
	return  JsonObj;
}

int32 UCBHTTPResponse::GetContentLength() const
{
	return Response ? Response->GetContentLength() : 0;
}

FString UCBHTTPResponse::GetContentType() const
{
	return Response ? Response->GetContentType() : TEXT("");
}

FString UCBHTTPResponse::GetHeader(const FString& Key) const
{
	return Response ? Response->GetHeader(Key) : TEXT("");
}

int32 UCBHTTPResponse::GetResponseCode() const
{
	return Response ? Response->GetResponseCode() : -1;
}

FString UCBHTTPResponse::GetURL() const
{
	return Response ? Response->GetURL() : TEXT("");
}

FString UCBHTTPResponse::GetURLParameter(const FString& ParameterName) const
{
	return Response ? Response->GetURLParameter(ParameterName) : TEXT("");
}

float UCBHTTPResponse::GetElapsedTime() const
{
	return RequestDuration;
}

void UCBHTTPResponse::InitInternal(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> InResponse,
	const float& InRequestDuration)
{
	Response = InResponse;
	RequestDuration = InRequestDuration;
}

TMap<FString, FString> UCBHTTPResponse::GetAllHeaders() const
{
	if (!Response)
	{
		return TMap<FString, FString>();
	}

	TArray<FString> Headers = Response->GetAllHeaders();

	TMap<FString, FString> OutHeaders;
	
	FString* Key;
	FString* Value;

	for (FString & Header : Headers)
	{
		Key		= nullptr;
		Value	= nullptr;

		Header.Split(TEXT(": "), Key, Value, ESearchCase::CaseSensitive);

		if (Key && Value)
		{
			OutHeaders.Emplace(*Key, *Value);
		}
	}

	return OutHeaders;
}
