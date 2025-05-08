// Copyright 2023 CB,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBHTTPTypes.h"
#include "CBHTTPRequset.generated.h"

class IHttpRequest;
class IHttpResponse;
class UCBHTTPResponse;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRequestComplete,       UCBHTTPRequset*const, Request, UCBHTTPResponse*const, Response,   const bool,     bConnectedSuccessfully);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRequestProgress,		 UCBHTTPRequset*const, Request, const int32,		   BytesSent,  const int32,    BytesReceived,       TArray<uint8>, Content);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRequestHeaderReceived, UCBHTTPRequset*const, Request, const FString&,        HeaderName, const FString&, NewHeaderValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRequestWillRetry,		 UCBHTTPRequset*const, Request, UCBHTTPResponse*const, Response,   const float,    SecondsToRetry);

/**
 * HTTP请求的包装类
 */
UCLASS(ClassGroup=(CBHTTP))
class CBHTTP_API UCBHTTPRequset : public UObject
{
	GENERATED_BODY()

public:
	UCBHTTPRequset();

	/**创建 HTTP Request*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	static UPARAM(DisplayName = "Request") UCBHTTPRequset* CreateRequest();

	/**设置Verb*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetVerb(const EHttpVerb InVerb);

	/**设置Header*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetHeader(const FString& Key, const FString& Value);

	/**设置Headers*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetHeaders(const TMap<FString, FString> &Headers);

	/**在现有末尾添加Header*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void AppendToHeader(const FString& Key, const FString& Value);

	/**设置Request的URL*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetURL(const FString& Url);

	/**设置MineType*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetMimeType(const EHttpMimeType MimeType);

	/**通过string方式设置MineType*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetMimeTypeAsString(const FString & MimeType);
	
	/**设置Content为binary data*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetContent(const TArray<uint8>& Content);

	/**设置content为string*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetContentAsString(const FString& Content);

	/**
	*将请求的content设置为从文件流式传输。
	*@param FileName 用于流式传输正文的文件名。
	*@return True，如果文件有效并且将用于流式传输请求。否则为假。
	*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void SetContentAsStreamedFile(const FString& FileName, bool & bFileValid);

	/**返回所有标头Headers*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Headers") TMap<FString, FString> GetAllHeaders() const;

	/**获取二进制 content*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	void GetContent(TArray<uint8>& OutContent) const;

	/**获取string content*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Content") FString GetContentAsString() const;

	/*返回标头中的内容长度（如果可用或为零）*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Length") int32 GetContentLength() const;

	/**若可用返回contenttype*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Type") FString GetContentType() const;

	/**返回服务器完全响应花费时间*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Time") float GetElapsedTime() const;

	/**返回 header的key*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Header") FString GetHeader(const FString& Key) const;

	/**返回状态*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Status") EHttpBPRequestStatus GetStatus() const;

	/**返回URL*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "URL") FString GetURL() const;

	/**返回URL参数*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Parameter") FString GetURLParameter(const FString& ParameterName) const;

	/**获取verb*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Verb") FString GetVerb() const;

	/**获取请求进度*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (Keywords = "send process HTTP request"))
	UPARAM(DisplayName = "Has Started") bool ProcessRequest();

	/**取消Requset*/
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void CancelRequest();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = CBHTTP)
	FOnRequestComplete OnRequestComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = CBHTTP)
	FOnRequestProgress OnRequestProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = CBHTTP)
	FOnRequestHeaderReceived OnRequestHeaderReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = CBHTTP)
	FOnRequestWillRetry OnRequestWillRetry;

public:
	 TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> GetResponse();
private:
	void OnRequestCompleteInternal (TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> RawResponse, bool bConnectedSuccessfully);
	void OnRequestProgressInternal (TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest, const int32 BytesSent, const int32 BytesReceived);
	void OnHeaderReceivedInternal  (TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest, const FString& HeaderName, const FString& HeaderValue);
	void OnRequestWillRetryInternal(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> RawRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> RawResponse, float SecondsToRetry);

	UCBHTTPResponse* CreateResponse(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> & RawRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> & RawResponse);

	FString ConvertEnumVerbToString(const EHttpVerb InVerb);

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request;


private:
	/**引擎提供的接口不允许我们编辑CURL Verb，因此为私有方法*/
	void SetCustomVerb(const FString& Verb);
};
