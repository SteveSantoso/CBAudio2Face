// Copyright 2023 CB,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include  "CBHTTPTypes.h"
#include "CBHTTPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CBHTTP_API UCBHTTPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	*尝试将字符串转换为EHttpVerb枚举。
	*如果字符串无效，则默认情况下返回GET。
	**/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
	static EHttpVerb StringToVerb(const FString& Verb);

	/**
	*获取允许进行HTTP调用的域。
	*空数组表示允许所有域。
	**/
	UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Get Allowed Domains"))
	void HttpGlobal_GetAllowedDomains(TArray<FString> & AllowedDomains);

	/*获取每个服务器允许的最大连接数*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Get HTTP Max Connections per Server"))
    int32 HttpGlobal_GetHttpMaxConnectionsPerServer();
    
	/*获取用于联系服务器的代理地址*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Get Proxy Address"))
    FString HttpGlobal_GetProxyAddress();
    
	/*获取HTTP请求的最大读取缓冲区大小*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Get Max Read Buffer Size"))
    int32 HttpGlobal_GetMaxReadBufferSize();

	/**
	*添加要附加到未来请求的默认标头
	*如果请求已经指定了此标头，则不会使用默认版本
	*@param HeaderName 标头的名称（例如，“内容类型”）
	*@param HeaderValue 标头的值
	*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Add Default Header"))
	static void HttpGlobal_AddDefaultHeader(const FString &HeaderName, const FString & HeaderValue);

	/**
	*代理地址的设置程序。
	*@param InProxyAddress 要使用的新代理地址。
	*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set Proxy Address"))
	static void HttpGlobal_SetProxyAddress(const FString &InProxyAddress);

	/**
	*设置请求的最大读取缓冲区大小。
	*@param SizeInBytes 用于读取缓冲区的最大字节数
	*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set Max Read Buffer Size"))
	static void HttpGlobal_SetMaxReadBufferSize(const int32 SizeInBytes);

	/*设置整个http请求完成的超时时间（以秒为单位）*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Timeout"))
	static void HttpGlobal_SetHttpTimeout(const float Timeout);

	/*设置空闲HTTP线程的最小 tick rate */
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Thread Idle Minimum Sleep Time in Seconds"))
	static void HttpGlobal_SetHttpThreadIdleMinimumSleepTimeInSeconds(const float Time);

	/*设置空闲HTTP线程的tick rate of an idle*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Thread Idle Frame Time in Seconds"))
	static void HttpGlobal_SetHttpThreadIdleFrameTimeInSeconds(const float Time);

	/*设置活动HTTP线程的最小 tick rate*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Thread Active Minimum Sleep Time in Seconds"))
    static void HttpGlobal_SetHttpThreadActiveMinimumSleepTimeInSeconds(const float Time);

	/*设置活动HTTP线程的目标节拍 tick rate**/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Thread Active Frame Time in Seconds"))
    static void HttpGlobal_SetHttpThreadActiveFrameTimeInSeconds(const float Time);

	/*设置每个http请求的最小延迟时间*/
    UFUNCTION(BlueprintCallable, Category = CBHTTP, meta = (DisplayName = "GLOBAL HTTP - Set HTTP Delay Time"))
    static void HttpGlobal_SetHttpDelayTime(const float Delay);

	/*将响应代码转换为其官方名称代码*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString HttpResponseCodeToString(const int32 ResponseCode);

    /**将响应代码转换为其用例*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString HttpResponseCodeToDescriptionString(const int32 ResponseCode);

	/**
	 *用于创建字符串MIME类型的帮助程序。
	*@return MIME类型与此类型的文件相关联的MIME类型文本。
	**/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString CreateMimeType(const EHttpMimeType Type);

	/**
	*将响应代码转换为枚举以便于进行比较。
	**/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static EHttpResponseCode HttpResponseCodeToEnum(const int32 ResponseCode);

	/**
	*创建一个请求并将其初始化为指定的属性。
	*您可以使用“Content-Type”覆盖MIME类型
	*如果枚举中未定义所需的MIME类型。
	*@return初始化的请求。
	**/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (AutoCreateRefTerm = "Headers, UrlParameters"))
    static UCBHTTPRequset* CreateInitializedRequest(const FString& Url, const TMap<FString, FString> & UrlParameters, const EHttpVerb Verb, const EHttpMimeType MimeType, const FString& Content, const TMap<FString, FString>& Headers);

	/**
	*创建一个请求并将其初始化为指定的属性。
	*您可以使用“Content-Type”覆盖MIME类型
	*如果枚举中未定义所需的MIME类型。
	*@return初始化的请求。
	**/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (AutoCreateRefTerm = "Headers, Content, UrlParameters"))
    static UCBHTTPRequset* CreateInitializedBinaryRequest(const FString& Url, const TMap<FString, FString>& UrlParameters, const EHttpVerb Verb, const EHttpMimeType MimeType, const TArray<uint8> & Content, const TMap<FString, FString>& Headers);

	/*转义参数并将其添加到URL的末尾*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString AddParametersToUrl(FString InUrl, const TMap<FString, FString>& Parameters);

	/*对以下字符串进行转义以符合URL规范*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (Keywords = "url escape encode string"))
    static FString UrlEncodeString(const FString& StringToEscape);

	/*解码此URL编码的字符串*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (Keywords = "decode url parse string"))
    static FString UrlDecodeString(const FString& StringToDecode);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (Keywords = "url escaped encoded"))
    static bool IsUrlEncoded(const TArray<uint8> Payload);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString GetUrlDomain(const FString& Url);

	/*将此字符串编码为HTML实体*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP, meta = (Keywords = "encode html parse entities"))
    static FString HtmlEncodeString(const FString& StringToEncode);

	/*获取操作系统指定的代理地址*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString GetOperatingSystemProxyAddress();

	/**
	*返回要在HTTP请求中使用的默认用户代理字符串。
	*明确设置用户代理标头的请求将不会使用此值。
	*@return  User-Agent
	*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString GetDefaultUserAgent();

	/**转义指定的用户代理*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString EscapeUserAgentString(const FString & RawUserAgent);

	/**
	*获取文件的mime类型
	*@返回文件的mime类型。
	*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = CBHTTP)
    static FString GetFileMimeType(const FString & FilePath);
	
};
