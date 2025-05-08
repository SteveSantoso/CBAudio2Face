// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CBHTTPTypes.generated.h"


/**
  * 用于内容类型的常见 MIME 类型的content-type。
  * 详细参考：https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Basics_of_HTTP/MIME_types
  **/
UENUM(BlueprintType)
enum class EHttpMimeType : uint8
{
	url		UMETA(DisplayName="application/x-www-form-urlencoded",		ToolTip="Data encoded in URL"),
	form	UMETA(DisplayName="multipart/form-data",					ToolTip="Form data"),
	txt		UMETA(DisplayName="text/plain",								ToolTip="Text"),
	html	UMETA(DisplayName="text/html",								ToolTip="HyperText Markup Language (HTML)"),
	aac		UMETA(DisplayName="audio/aac",								ToolTip="AAC audio"),
	arc		UMETA(DisplayName="application/x-freearc",					ToolTip="Archive document"),
	avi		UMETA(DisplayName="video/x-msvideo",						ToolTip="Audio Video Interleave"),
	bin		UMETA(DisplayName="application/octet-stream",				ToolTip="Any kind of binary data"),
	bmp		UMETA(DisplayName="image/bmp",								ToolTip="Windows OS/2 Bitmap Graphics"),
	css		UMETA(DisplayName="text/css",								ToolTip="Cascading Style Sheets"),
	csv		UMETA(DisplayName="text/csv",								ToolTip="Comma-separated values"),
	doc		UMETA(DisplayName="application/msword",						ToolTip="Microsoft Word"),
	gz		UMETA(DisplayName="application/gzip",						ToolTip="GZip Compressed Archive"),
	gif		UMETA(DisplayName="image/gif",								ToolTip="Graphics Interchange Format (GIF)"),
	jpg		UMETA(DisplayName="image/jpeg",								ToolTip="JPEG images"),
	js		UMETA(DisplayName="text/javascript",						ToolTip="JavaScript"),
	json    UMETA(DisplayName="application/json",						ToolTip="JSON format"),
	mp3		UMETA(DisplayName="audio/mpeg",								ToolTip="MP3 audio"),
	mpkg	UMETA(DisplayName="application/vnd.apple.installer+xml",	ToolTip="Apple Installer Package"),
	png		UMETA(DisplayName="image/png",								ToolTip="Portable Network Graphics"),
	pdf		UMETA(DisplayName="application/pdf",						ToolTip="Adobe Portable Document Format (PDF)"),
	php		UMETA(DisplayName="application/x-httpd-php",				ToolTip="Hypertext Preprocessor (PHP)"),
	rar		UMETA(DisplayName="application/vnd.rar",					ToolTip="RAR archive"),
	tar		UMETA(DisplayName="application/x-tar",						ToolTip="Tape Archive (TAR)"),
	xml		UMETA(DisplayName="application/xml",						ToolTip="XML"),
	zip		UMETA(DisplayName="application/zip",						ToolTip="ZIP archive"),
	_7z		UMETA(DisplayName="application/x-7z-compressed",			ToolTip="7-zip archive")
};



UENUM(BlueprintType, DisplayName = "HTTP Request Status")
enum class EHttpBPRequestStatus : uint8
{
	NotStarted				UMETA(DisplayName="Not Started",				ToolTip = "Has not been started via ProcessRequest()."),
	Processing				UMETA(DisplayName="Processing",					ToolTip = "Currently being ticked and processed."),
	Failed					UMETA(DisplayName="Failed",						ToolTip = "Finished but failed."),
	Failed_ConnectionError	UMETA(DisplayName="Failed: Connection Error",	ToolTip = "Failed because it was unable to connect (safe to retry)."),
	Succeeded				UMETA(DisplayName="Succeeded",					ToolTip = "Finished and was successful.")
};


/**
*详尽的官方响应代码。
*您可以在蓝图中使用它，但它的主要用途是用作查找数据
*在蓝图中使用它可能很复杂，因为它包含很多条目。
**/
UENUM(BlueprintType, DisplayName="HTTP Response Code")
enum class EHttpResponseCode : uint8
{
	C1XX_InformationResponses = 0 UMETA(Hidden),

	C100 UMETA(DisplayName="100 Continue",							ToolTip="This interim response indicates that everything so far is OK and that the client should continue the request, or ignore the response if the request is already finished."),
	C101 UMETA(DisplayName="101 Switching Protocol",				ToolTip="This code is sent in response to an Upgrade request header from the client, and indicates the protocol the server is switching to."),
	C102 UMETA(DisplayName="102 Processing",						ToolTip="This code indicates that the server has received and is processing the request, but no response is available yet."),
	C103 UMETA(DisplayName="103 Early Hints",						ToolTip="This status code is primarily intended to be used with the Link header, letting the user agent start preloading resources while the server prepares a response."),
	
	C2XX_SuccessfulResponses UMETA(Hidden),
	
	C200 UMETA(DisplayName="200 OK",								ToolTip="The request has succeeded."),
	C201 UMETA(DisplayName="201 Created",							ToolTip="The request has succeeded and a new resource has been created as a result. This is typically the response sent after POST requests, or some PUT requests."),
	C202 UMETA(DisplayName="202 Accepted",							ToolTip="The request has been received but not yet acted upon. It is noncommittal, since there is no way in HTTP to later send an asynchronous response indicating the outcome of the request. It is intended for cases where another process or server handles the request, or for batch processing."),
	C203 UMETA(DisplayName="203 Non-Authoritative Information",		ToolTip="This response code means the returned meta-information is not exactly the same as is available from the origin server, but is collected from a local or a third-party copy. This is mostly used for mirrors or backups of another resource. Except for that specific case, the \"200 OK\" response is preferred to this status."),
	C204 UMETA(DisplayName="204 No Content",						ToolTip="There is no content to send for this request, but the headers may be useful. The user-agent may update its cached headers for this resource with the new ones."),
	C205 UMETA(DisplayName="205 Reset Content",						ToolTip="Tells the user-agent to reset the document which sent this request."),
	C206 UMETA(DisplayName="206 Partial Content",					ToolTip="This response code is used when the Range header is sent from the client to request only part of a resource."),
	C207 UMETA(DisplayName="207 Multi-Status",						ToolTip="Conveys information about multiple resources, for situations where multiple status codes might be appropriate."),
	C208 UMETA(DisplayName="208 Already Reported",					ToolTip="Used inside a <dav:propstat> response element to avoid repeatedly enumerating the internal members of multiple bindings to the same collection."),
	C226 UMETA(DisplayName="226 IM Used",							ToolTip="The server has fulfilled a GET request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance."),
	
	C3XX_RedirectionMessages UMETA(Hidden),
	
	C300 UMETA(DisplayName="300 Multiple Choice",					ToolTip="The request has more than one possible response. The user-agent or user should choose one of them. (There is no standardized way of choosing one of the responses, but HTML links to the possibilities are recommended so the user can pick.)"),
	C301 UMETA(DisplayName="301 Moved Permanently",					ToolTip="The URL of the requested resource has been changed permanently. The new URL is given in the response."),
	C302 UMETA(DisplayName="302 Found",								ToolTip="This response code means that the URI of requested resource has been changed temporarily. Further changes in the URI might be made in the future. Therefore, this same URI should be used by the client in future requests."),
	C303 UMETA(DisplayName="303 See Other",							ToolTip="The server sent this response to direct the client to get the requested resource at another URI with a GET request."),
	C304 UMETA(DisplayName="304 Not Modified",						ToolTip="This is used for caching purposes. It tells the client that the response has not been modified, so the client can continue to use the same cached version of the response."),
	C305 UMETA(DisplayName="305 Use Proxy",							ToolTip="Defined in a previous version of the HTTP specification to indicate that a requested response must be accessed by a proxy. It has been deprecated due to security concerns regarding in-band configuration of a proxy."),
	C306 UMETA(DisplayName="306 unused",							ToolTip="This response code is no longer used; it is just reserved. It was used in a previous version of the HTTP/1.1 specification."),
	C307 UMETA(DisplayName="307 Temporary Redirect",				ToolTip="The server sends this response to direct the client to get the requested resource at another URI with same method that was used in the prior request. This has the same semantics as the 302 Found HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request."),
	C308 UMETA(DisplayName="308 Permanent Redirect",				ToolTip="This means that the resource is now permanently located at another URI, specified by the Location: HTTP Response header. This has the same semantics as the 301 Moved Permanently HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request."),
	
	C4XX_ClientErrorResponses UMETA(Hidden),
	
	C400 UMETA(DisplayName="400 Bad Request",						ToolTip="The server could not understand the request due to invalid syntax."),
	C401 UMETA(DisplayName="401 Unauthorized",						ToolTip="Although the HTTP standard specifies \"unauthorized\", semantically this response means \"unauthenticated\". That is, the client must authenticate itself to get the requested response."),
	C402 UMETA(DisplayName="402 Payment Required",					ToolTip="This response code is reserved for future use. The initial aim for creating this code was using it for digital payment systems, however this status code is used very rarely and no standard convention exists."),
	C403 UMETA(DisplayName="403 Forbidden",							ToolTip="The client does not have access rights to the content; that is, it is unauthorized, so the server is refusing to give the requested resource. Unlike 401, the client's identity is known to the server."),
	C404 UMETA(DisplayName="404 Not Found",							ToolTip="The server can not find the requested resource. In the browser, this means the URL is not recognized. In an API, this can also mean that the endpoint is valid but the resource itself does not exist. Servers may also send this response instead of 403 to hide the existence of a resource from an unauthorized client. This response code is probably the most famous one due to its frequent occurrence on the web."),
	C405 UMETA(DisplayName="405 Method Not Allowed",				ToolTip="The request method is known by the server but has been disabled and cannot be used. For example, an API may forbid DELETE-ing a resource. The two mandatory methods, GET and HEAD, must never be disabled and should not return this error code."),
	C406 UMETA(DisplayName="406 Not Acceptable",					ToolTip="This response is sent when the web server, after performing server-driven content negotiation, doesn't find any content that conforms to the criteria given by the user agent."),
	C407 UMETA(DisplayName="407 Proxy Authentication Required",		ToolTip="This is similar to 401 but authentication is needed to be done by a proxy."),
	C408 UMETA(DisplayName="408 Request Timeout",					ToolTip="This response is sent on an idle connection by some servers, even without any previous request by the client. It means that the server would like to shut down this unused connection. This response is used much more since some browsers, like Chrome, Firefox 27+, or IE9, use HTTP pre-connection mechanisms to speed up surfing. Also note that some servers merely shut down the connection without sending this message."),
	C409 UMETA(DisplayName="409 Conflict",							ToolTip="This response is sent when a request conflicts with the current state of the server."),
	C410 UMETA(DisplayName="410 Gone",								ToolTip="This response is sent when the requested content has been permanently deleted from server, with no forwarding address. Clients are expected to remove their caches and links to the resource. The HTTP specification intends this status code to be used for \"limited - time, promotional services\". APIs should not feel compelled to indicate resources that have been deleted with this status code."),
	C411 UMETA(DisplayName="411 Length Required",					ToolTip="Server rejected the request because the Content-Length header field is not defined and the server requires it."),
	C412 UMETA(DisplayName="412 Precondition Failed",				ToolTip="The client has indicated preconditions in its headers which the server does not meet."),
	C413 UMETA(DisplayName="413 Payload Too Large",					ToolTip="Request entity is larger than limits defined by server; the server might close the connection or return an Retry-After header field."),
	C414 UMETA(DisplayName="414 URI Too Long",						ToolTip="The URI requested by the client is longer than the server is willing to interpret."),
	C415 UMETA(DisplayName="415 Unsupported Media Type",			ToolTip="The media format of the requested data is not supported by the server, so the server is rejecting the request."),
	C416 UMETA(DisplayName="416 Range Not Satisfiable",				ToolTip="The range specified by the Range header field in the request can't be fulfilled; it's possible that the range is outside the size of the target URI's data."),
	C417 UMETA(DisplayName="417 Expectation Failed",				ToolTip="This response code means the expectation indicated by the Expect request header field can't be met by the server."),
	C418 UMETA(DisplayName="418 I'm a teapot",						ToolTip="The server refuses the attempt to brew coffee with a teapot. Unofficial code rarely used."),
	C421 UMETA(DisplayName="421 Misdirected Request",				ToolTip="The request was directed at a server that is not able to produce a response. This can be sent by a server that is not configured to produce responses for the combination of scheme and authority that are included in the request URI."),
	C422 UMETA(DisplayName="422 Unprocessable Entity",				ToolTip="The request was well-formed but was unable to be followed due to semantic errors."),
	C423 UMETA(DisplayName="423 Locked",							ToolTip="The resource that is being accessed is locked."),
	C424 UMETA(DisplayName="424 Failed Dependency",					ToolTip="The request failed due to failure of a previous request."),
	C425 UMETA(DisplayName="425 Too Early",							ToolTip="Indicates that the server is unwilling to risk processing a request that might be replayed."),
	C426 UMETA(DisplayName="426 Upgrade Required",					ToolTip="The server refuses to perform the request using the current protocol but might be willing to do so after the client upgrades to a different protocol. The server sends an Upgrade header in a 426 response to indicate the required protocol(s)."),
	C428 UMETA(DisplayName="428 Precondition Required",				ToolTip="The origin server requires the request to be conditional. This response is intended to prevent the 'lost update' problem, where a client GETs a resource's state, modifies it, and PUTs it back to the server, when meanwhile a third party has modified the state on the server, leading to a conflict."),
	C429 UMETA(DisplayName="429 Too Many Requests",					ToolTip="The user has sent too many requests in a given amount of time (\"rate limiting\")."),
	C431 UMETA(DisplayName="431 Request Header Fields Too Large",	ToolTip="The server is unwilling to process the request because its header fields are too large. The request may be resubmitted after reducing the size of the request header fields."),
	C451 UMETA(DisplayName="451 Unavailable For Legal Reasons",		ToolTip="The user-agent requested a resource that cannot legally be provided, such as a web page censored by a government."),
	
	C5XX_ServerErrorResponses UMETA(Hidden),	
	
	C500 UMETA(DisplayName="500 Internal Server Error",				ToolTip="The server has encountered a situation it doesn't know how to handle."),
	C501 UMETA(DisplayName="501 Not Implemented",					ToolTip="The request method is not supported by the server and cannot be handled. The only methods that servers are required to support (and therefore that must not return this code) are GET and HEAD."),
	C502 UMETA(DisplayName="502 Bad Gateway",						ToolTip="This error response means that the server, while working as a gateway to get a response needed to handle the request, got an invalid response."),
	C503 UMETA(DisplayName="503 Service Unavailable",				ToolTip="The server is not ready to handle the request. Common causes are a server that is down for maintenance or that is overloaded. Note that together with this response, a user-friendly page explaining the problem should be sent. This responses should be used for temporary conditions and the Retry-After: HTTP header should, if possible, contain the estimated time before the recovery of the service. The webmaster must also take care about the caching-related headers that are sent along with this response, as these temporary condition responses should usually not be cached."),
	C504 UMETA(DisplayName="504 Gateway Timeout",					ToolTip="This error response is given when the server is acting as a gateway and cannot get a response in time."),
	C505 UMETA(DisplayName="505 HTTP Version Not Supported",		ToolTip="The HTTP version used in the request is not supported by the server."),
	C506 UMETA(DisplayName="506 Variant Also Negotiates",			ToolTip="The server has an internal configuration error: the chosen variant resource is configured to engage in transparent content negotiation itself, and is therefore not a proper end point in the negotiation process."),
	C507 UMETA(DisplayName="507 Insufficient Storage",				ToolTip="The method could not be performed on the resource because the server is unable to store the representation needed to successfully complete the request."),
	C508 UMETA(DisplayName="508 Loop Detected",						ToolTip="The server detected an infinite loop while processing the request."),
	C510 UMETA(DisplayName="510 Not Extended",						ToolTip="Further extensions to the request are required for the server to fulfil it."),
	C511 UMETA(DisplayName="511 Network Authentication Required",	ToolTip="The client needs to authenticate to gain network access."),

	CUknown UMETA(DisplayName="Unofficial Response Code")
};


UENUM(BlueprintType)
enum class EHttpVerb : uint8
{
	GET,
	POST,
	PUT,
	PATCH,
	HEAD,
	MAX_COUNT UMETA(Hidden)
};