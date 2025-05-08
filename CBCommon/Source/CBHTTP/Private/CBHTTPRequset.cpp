// Copyright 2023 CB All Rights Reserved.


#include "CBHTTPRequset.h"

#include "HttpModule.h"
#include "CBHTTPLibrary.h"
#include "CBHTTPResponse.h"
#include "Interfaces/IHttpResponse.h"

UCBHTTPRequset::UCBHTTPRequset()
	: Super()
{
	Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UCBHTTPRequset::OnRequestCompleteInternal );
	Request->OnRequestProgress       ().BindUObject(this, &UCBHTTPRequset::OnRequestProgressInternal );
	Request->OnHeaderReceived        ().BindUObject(this, &UCBHTTPRequset::OnHeaderReceivedInternal  );
}

UCBHTTPRequset* UCBHTTPRequset::CreateRequest()
{
	return NewObject<UCBHTTPRequset>();
}

void UCBHTTPRequset::SetVerb(const EHttpVerb InVerb)
{
	SetCustomVerb(ConvertEnumVerbToString(InVerb));
}

void UCBHTTPRequset::SetHeader(const FString& Key, const FString& Value)
{
	Request->SetHeader(Key, Value);
}

void UCBHTTPRequset::SetHeaders(const TMap<FString, FString>& Headers)
{
	for (const auto& Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}
}

void UCBHTTPRequset::AppendToHeader(const FString& Key, const FString& Value)
{
	Request->AppendToHeader(Key, Value);
}

void UCBHTTPRequset::SetURL(const FString& Url)
{
	Request->SetURL(Url);
}

void UCBHTTPRequset::SetMimeType(const EHttpMimeType MimeType)
{
	Request->AppendToHeader(TEXT("Content-Type"), UCBHTTPLibrary::CreateMimeType(MimeType));
}

void UCBHTTPRequset::SetMimeTypeAsString(const FString& MimeType)
{
	Request->AppendToHeader(TEXT("Content-Type"), MimeType);
}

void UCBHTTPRequset::SetContent(const TArray<uint8>& Content)
{
	Request->SetContent(Content);
}

void UCBHTTPRequset::SetContentAsString(const FString& Content)
{
	Request->SetContentAsString(Content);
}

void UCBHTTPRequset::SetContentAsStreamedFile(const FString& FileName, bool& bFileValid)
{
	bFileValid = Request->SetContentAsStreamedFile(FileName);
}

TMap<FString, FString> UCBHTTPRequset::GetAllHeaders() const
{
	TArray<FString> Headers = Request->GetAllHeaders();

	TMap<FString, FString> OutHeaders;
	
	FString* Key	= nullptr;
	FString* Value	= nullptr;

	const FString Separator = TEXT(": ");

	for (FString & Header : Headers)
	{
		if (Header.Split(Separator, Key, Value, ESearchCase::CaseSensitive))
		{
			OutHeaders.Emplace(*Key, *Value);
		}
	}

	return OutHeaders;
}

void UCBHTTPRequset::GetContent(TArray<uint8>& OutContent) const
{
	OutContent = Request->GetContent();
}

FString UCBHTTPRequset::GetContentAsString() const
{
	const TArray<uint8>& Content = Request->GetContent();

	return BytesToString(Content.GetData(), Content.Num());
}

int32 UCBHTTPRequset::GetContentLength() const
{
	return Request->GetContentLength();
}

FString UCBHTTPRequset::GetContentType() const
{
	return Request->GetContentType();
}

float UCBHTTPRequset::GetElapsedTime() const
{
	return Request->GetContentLength();
}

FString UCBHTTPRequset::GetHeader(const FString& Key) const
{
	return Request->GetHeader(Key);
}

EHttpBPRequestStatus UCBHTTPRequset::GetStatus() const
{
	return static_cast<EHttpBPRequestStatus>(Request->GetStatus());
}

FString UCBHTTPRequset::GetURL() const
{
	return Request->GetURL();
}

FString UCBHTTPRequset::GetURLParameter(const FString& ParameterName) const
{
	return Request->GetURLParameter(ParameterName);
}

FString UCBHTTPRequset::GetVerb() const
{
	return Request->GetVerb();
}

bool UCBHTTPRequset::ProcessRequest()
{
	if (Request->GetContentType() == TEXT(""))
	{
		SetMimeType(EHttpMimeType::txt);
	}

	return Request->ProcessRequest();
}

void UCBHTTPRequset::CancelRequest()
{
	Request->CancelRequest();
}

TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> UCBHTTPRequset::GetResponse()
{
   return Request->GetResponse();
}

void UCBHTTPRequset::OnRequestCompleteInternal(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest,
                                               TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> RawResponse, bool bConnectedSuccessfully)
{
	OnRequestComplete.Broadcast(this, CreateResponse(RawRequest, RawResponse), bConnectedSuccessfully);
}

void UCBHTTPRequset::OnRequestProgressInternal(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest,
	const int32 BytesSent, const int32 BytesReceived)
{
	TArray<uint8> TemConten;
	OnRequestProgress.Broadcast(this, BytesSent, BytesReceived,GetResponse() ?GetResponse()->GetContent():TemConten);
}

void UCBHTTPRequset::OnHeaderReceivedInternal(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest,
	const FString& HeaderName, const FString& HeaderValue)
{
	OnRequestHeaderReceived.Broadcast(this, HeaderName, HeaderValue);
}

void UCBHTTPRequset::OnRequestWillRetryInternal(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest,
	TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> RawResponse, float SecondsToRetry)
{
	OnRequestWillRetry.Broadcast(this, CreateResponse(RawRequest, RawResponse), SecondsToRetry);
}

UCBHTTPResponse* UCBHTTPRequset::CreateResponse(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>& RawRequest,
	TSharedPtr<IHttpResponse, ESPMode::ThreadSafe>& RawResponse)
{
	UCBHTTPResponse* const WrappedResponse = NewObject<UCBHTTPResponse>();

	WrappedResponse->InitInternal(RawResponse, RawRequest->GetElapsedTime());

	return WrappedResponse;
}

FString UCBHTTPRequset::ConvertEnumVerbToString(const EHttpVerb InVerb)
{
	constexpr uint8 VerbsCount = static_cast<uint8>(EHttpVerb::MAX_COUNT);

	const uint8 Uint8Verb = static_cast<uint8>(InVerb);
	
	if (Uint8Verb >= VerbsCount)
	{
		return TEXT("NONE");
	}

	static const FString Converted[VerbsCount] =
	{
		TEXT("GET"),
		TEXT("POST"),
		TEXT("PUT"),
		TEXT("PATCH"),
		TEXT("HEAD"),
	//	TEXT("DELETE")
	};

	return Converted[Uint8Verb];
}

void UCBHTTPRequset::SetCustomVerb(const FString& Verb)
{
	Request->SetVerb(Verb);
}
