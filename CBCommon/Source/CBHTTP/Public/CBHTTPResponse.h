// Copyright 2023 CB,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBHTTPResponse.generated.h"

class IHttpResponse;
class IHttpRequest;
/**
 * 
 */
UCLASS(ClassGroup=(CBHTTP))
class CBHTTP_API UCBHTTPResponse : public UObject
{
	GENERATED_BODY()

private:
	friend class UCBHTTPRequset;

public:

	UCBHTTPResponse();

	/* 获取全部的Hraders */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Headers") TMap<FString, FString> GetAllHeaders() const;

	/* 获取 request的 binary data */
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	void GetContent(TArray<uint8>& OutContent) const;

	/* 获取 request的 string data */
	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	UPARAM(DisplayName = "Content") FString GetContentAsString() const;

	UFUNCTION(BlueprintCallable, Category = CBHTTP)
	UPARAM(DisplayName = "Json") UCBJsonObject* GetContentAsJson() const;

	/*返回标头中的内容长度（如果可用或为零）*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Length") int32 GetContentLength() const;

	/**若可用返回contenttype*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Type") FString GetContentType() const;

	/**返回 header的key*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Header") FString GetHeader(const FString& Key) const;

	/**返回状态*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Code") int32 GetResponseCode() const;

	/*获取URL. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "URL") FString GetURL() const;

	/* 获取URL参数. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Parameter") FString GetURLParameter(const FString& ParameterName) const;

	/* 返回服务器完全响应请求所花费的时间. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	UPARAM(DisplayName = "Elapsed Time") float GetElapsedTime() const;

private:
	//无法将RAII与UObject一起使用。
	//由于这种变通方法，响应可以是nullptr。
	void InitInternal(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> InResponse, const float &InRequestDuration);

	float RequestDuration;

	TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response;	
};
