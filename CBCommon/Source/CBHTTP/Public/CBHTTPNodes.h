// Copyright 2023 CB,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CBHTTPNodes.generated.h"

USTRUCT(BlueprintType)
struct FHeaders
{
	GENERATED_BODY()
public:
	FHeaders() {};
	FHeaders(const TMap<FString, FString>& InHeaders) : Headers(InHeaders) {};
	FHeaders(TMap<FString, FString>&& InHeaders) : Headers(MoveTemp(InHeaders)) {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Headers)
	TMap<FString, FString> Headers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFileDownloadedEvent, const int32, TotalSizeInBytes, const int32, TotalBytesReceived, const float, PercentDownloaded);

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class UHttpDownloadFileProxy final : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:

    UPROPERTY(BlueprintAssignable)
    FOnFileDownloadedEvent OnFileDownloaded;
    
    UPROPERTY(BlueprintAssignable)
    FOnFileDownloadedEvent OnFileDownloadError;

    UPROPERTY(BlueprintAssignable)
    FOnFileDownloadedEvent OnDownloadProgress;

public:
    virtual void Activate() override;

    /**
    *通过HTTP请求下载文件。
    *@param FileUrl 我们要下载的文件的URL。
    *@param UrlParameters URL的参数。
    *@param Verb 用于请求Verb。
    *@param MimeType 请求内容的mime类型。
    *@param Content 请求的内容。
    *@param Headers 请求的标头。
    *@param SaveFileLocation 我们要保存下载的位置。
    */
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (BlueprintInternalUseOnly = "true", AutoCreateRefTerm = "Headers, UrlParameters", DisplayName = "Download File through HTTP"))
    static UHttpDownloadFileProxy* HttpDownloadFile(const FString& FileUrl, const TMap<FString, FString>& UrlParameters, const EHttpVerb Verb, const EHttpMimeType MimeType, const FString& Content, const TMap<FString, FString>& Headers, const FString& SaveFileLocation);

private:
    UFUNCTION()
    void OnRequestCompleted (UCBHTTPRequset* const Request, UCBHTTPResponse* const Response, const bool bConnectedSuccessfully);
    UFUNCTION()
    void OnRequestTick      (UCBHTTPRequset* const Request, const int32 BytesSent,        const int32 BytesReceived,  const TArray<uint8> Content );
    UFUNCTION()
    void OnHeadersReceived  (UCBHTTPRequset* const Request, const FString& HeaderName,     const FString& NewHeaderValue);

    FORCEINLINE float GetPercents() const
    {
        return ContentLength != 0 ? (Downloaded * 100.f / ContentLength) : 0.f;
    }

private:
    UPROPERTY()
    UCBHTTPRequset* Request;

    int32 ContentLength;
    int32 Downloaded;

    FString SaveLocation;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnRequestEvent,const int32, ResponseCode, const FHeaders&, Headers, const FString&, ContentType, const FString&,Content, UCBJsonObject*,ResponseJson,const float, TimeElapsed, const EHttpBPRequestStatus, ConnectionStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnBinaryRequest, const int32, ResponseCode, const FHeaders&, Headers, const FString&, ContentType, const TArray<uint8>&, Content, const float, TimeElapsed, const EHttpBPRequestStatus, ConnectionStatus);

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class UProcessHttpRequestProxy final : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    UProcessHttpRequestProxy(const FObjectInitializer& ObjectInitializer);

    virtual void Activate() override {};

    /*从服务器发送成功响应时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnResponse;

    /*发生错误时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnError;

    /*当此请求勾选并已下载或上载部分数据时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnTick;

    /*使用前面提供的参数处理请求*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (BlueprintInternalUseOnly = "true", DisplayName="Send Initialized HTTP Request"))
    static UProcessHttpRequestProxy* InlineProcessRequest(UCBHTTPRequset* const Request);

private:
    UFUNCTION()
    void OnCompleteInternal(UCBHTTPRequset* const Request, UCBHTTPResponse* const Response, const bool bConnectedSuccessfully);
    UFUNCTION()
    void OnTickInternal(UCBHTTPRequset* const Request, const int32 InBytesSent, const int32 InBytesReceived);

    UPROPERTY()
    UCBHTTPRequset* RequestWrapper;

    int32 BytesSent;
    int32 BytesReceived;

};


UCLASS(Abstract)
class CBHTTP_API USendHttpRequestProxyBase : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()
public:
    USendHttpRequestProxyBase(const FObjectInitializer& ObjectInitializer);

    virtual void Activate() override {};

public:
    UFUNCTION(BlueprintCallable)
    void CancelRequest();
protected:
    /* Request Events */
    virtual void OnTickInternal   (TArray<uint8> Incontent)         {};
    virtual void OnSuccessInternal(UCBHTTPResponse* const Response) {};
    virtual void OnErrorInternal  (UCBHTTPResponse* const Response) {};

    /* Request data */
    FORCEINLINE int32 GetBytesSent()           const { return BytesSent     ; }
    FORCEINLINE int32 GetBytesReceived()       const { return BytesReceived ; }
    FORCEINLINE TArray<uint8> GetTickContent() const {return Content      ; }
    FORCEINLINE UCBHTTPRequset* GetRequest()     { return RequestWrapper     ; }

    /*发送请求并处理失败的启动*/
    void SendRequest();

private:
    UFUNCTION()
    void _OnCompleteInternal(UCBHTTPRequset* const Request, UCBHTTPResponse* const Response, const bool bConnectedSuccessfully);
    UFUNCTION()
    void _OnTickInternal(UCBHTTPRequset* const Request, const int32 InBytesSent, const int32 InBytesReceived,TArray<uint8> Incontent);

    UPROPERTY()
    UCBHTTPRequset* RequestWrapper;

    int32 BytesSent;
    int32 BytesReceived;
    TArray<uint8> Content;
};

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class USendHttpRequestProxy final : public USendHttpRequestProxyBase
{
    GENERATED_BODY()

public:

    /*从服务器发送成功响应时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnResponse;

    /*发生错误时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnError;

    /*当此请求勾选并已下载或上载部分数据时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnRequestEvent OnTick;

    /** 
    *向指定的URL发送Http请求。
    *@param ServerUrl 连接的服务器。
    *@param UrlParameters 要添加到URL中的可选未配置的参数。
    *@param Verb 我们要用于此请求Verb
    *@param Content 此请求的内容。
    *@param Headers 此请求的标头。
    **/
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", AutoCreateRefTerm = "Headers, UrlParameters"),DisplayName="Send HTTP Request",  Category =CBHTTP)
    static USendHttpRequestProxy* SendHttpRequest(const FString & ServerUrl, const TMap<FString, FString> & UrlParameters, const EHttpVerb Verb, 
        const EHttpMimeType MimeType, const FString& Content, const TMap<FString, FString>& Headers);

protected:
    virtual void OnTickInternal(TArray<uint8> Incontent) override;
    virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
    virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;
};

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class USendBinaryHttpRequestProxy final : public USendHttpRequestProxyBase
{
    GENERATED_BODY()

public:
    
    /*从服务器发送成功响应时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnBinaryRequest OnResponse;

    /*发生错误时调用*/
    UPROPERTY(BlueprintAssignable)
    FOnBinaryRequest OnError;
    
    UPROPERTY(BlueprintAssignable)
    FOnBinaryRequest OnTick;

    /** 
    *向指定的URL发送Http请求。
    *@param ServerUrl 连接的服务器。
    *@param UrlParameters 要添加到URL中的可选未配置的参数。
    *@param Verb 我们要用于此请求Verb
    *@param Content 此请求的内容。
    *@param Headers 此请求的标头。
    **/
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", AutoCreateRefTerm = "Headers, UrlParameters, Content"),DisplayName="Send Binary HTTP Request", Category = CBHTTP)
    static USendBinaryHttpRequestProxy* SendBinaryHttpRequest(const FString& ServerUrl, const TMap<FString, FString>& UrlParameters, const EHttpVerb Verb, const EHttpMimeType MimeType, const TArray<uint8>& Content, const TMap<FString, FString>& Headers);

protected:
    virtual void OnTickInternal(TArray<uint8> Incontent) override;
    virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
    virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

};