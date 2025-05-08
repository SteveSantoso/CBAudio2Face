// Copyright 2023 CB,  All Rights Reserved.


#include "Library/CBFileHeplerBPLibrary.h"

#include "CBCommon.h"
#include "CBCommonInternalFuncs.h"
#include "IDesktopPlatform.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Async/Async.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Sound/SoundWave.h"

#if WITH_EDITOR
#include "DesktopPlatformModule.h"
#include "Engine/GameViewportClient.h"
#include "Editor.h"
#include "Engine/Engine.h"
#include "Widgets/SWindow.h"
#endif

#if PLATFORM_WINDOWS
#include "HAL/FileManager.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Microsoft/COMPointer.h"
#include <shlobj.h>

#endif

//渲染线程wrapper struct
struct FUpdateTextureData
{
	UTexture2D* Texture2D;
	FUpdateTextureRegion2D Region;
	uint32 Pitch;
	TArray64<uint8> BufferArray;
	TSharedPtr<IImageWrapper> Wrapper;	
};

void FCBLambdaRunnable::RunLambdaOnBackGroundThread(TFunction<void()> InFunction)
{
	Async(EAsyncExecution::Thread, InFunction);
}

void FCBLambdaRunnable::RunLambdaOnBackGroundThreadPool(TFunction<void()> InFunction)
{
	Async(EAsyncExecution::ThreadPool, InFunction);
}

FGraphEventRef FCBLambdaRunnable::RunShortLambdaOnGameThread(TFunction<void()> InFunction)
{
	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
}

FGraphEventRef FCBLambdaRunnable::RunShortLambdaOnBackGroundTask(TFunction<void()> InFunction)
{
	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::AnyThread);
}

void FCBLambdaRunnable::SetTimeout(TFunction<void()> OnDone, float DurationInSec, bool bCallbackOnGameThread)
{
	RunLambdaOnBackGroundThread([OnDone, DurationInSec, bCallbackOnGameThread]()
	{
		FPlatformProcess::Sleep(DurationInSec);

		if (bCallbackOnGameThread)
		{
			RunShortLambdaOnGameThread(OnDone);
		}
		else
		{
			OnDone();
		}
	});
}

FEnginePath UCBFileHeplerBPLibrary::GetEngineDirectories()
{
	FEnginePath EnginePath;
	
	EnginePath.Directory = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	EnginePath.Config = FPaths::ConvertRelativePathToFull(FPaths::EngineConfigDir());
	EnginePath.Content = FPaths::ConvertRelativePathToFull(FPaths::EngineContentDir());
	EnginePath.Intermediate = FPaths::ConvertRelativePathToFull(FPaths::EngineIntermediateDir());
	EnginePath.Plugins = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir());
	EnginePath.Saved =FPaths::ConvertRelativePathToFull( FPaths::EngineSavedDir());
	EnginePath.User = FPaths::ConvertRelativePathToFull(FPaths::EngineUserDir());
	EnginePath.DefaultLayout = FPaths::ConvertRelativePathToFull(FPaths::EngineDefaultLayoutDir());
	EnginePath.PlatformExtensions = FPaths::ConvertRelativePathToFull(FPaths::EnginePlatformExtensionsDir());
	EnginePath.UserLayout = FPaths::ConvertRelativePathToFull(FPaths::EngineUserLayoutDir());
	
	return EnginePath;
}

FProjectPath UCBFileHeplerBPLibrary::GetProjectDirectories()
{
	FProjectPath ProjectPath;
	
	ProjectPath.Directory = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	ProjectPath.Config = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	ProjectPath.Content = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	ProjectPath.Intermediate = FPaths::ConvertRelativePathToFull(FPaths::ProjectIntermediateDir());
	ProjectPath.Log = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
	ProjectPath.Mods = FPaths::ConvertRelativePathToFull(FPaths::ProjectModsDir());
	ProjectPath.Plugins = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
	ProjectPath.Saved = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
	ProjectPath.User = FPaths::ConvertRelativePathToFull(FPaths::ProjectUserDir());
	ProjectPath.PersistentDownload = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir());
	ProjectPath.PlatformExtensions = FPaths::ConvertRelativePathToFull(FPaths::ProjectPlatformExtensionsDir());
	
	return ProjectPath;
}

FString UCBFileHeplerBPLibrary::GetProjectDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
}

FString UCBFileHeplerBPLibrary::GetProjectConfigDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
}

FString UCBFileHeplerBPLibrary::GetProjectContentDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
}

FString UCBFileHeplerBPLibrary::GetProjectLogDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
}

FString UCBFileHeplerBPLibrary::GetProjectModsDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectModsDir());
}

FString UCBFileHeplerBPLibrary::GetProjectPluginsDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
}

FString UCBFileHeplerBPLibrary::GetProjectSavedDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
}

FString UCBFileHeplerBPLibrary::GetProjectUserDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectUserDir());
}

FString UCBFileHeplerBPLibrary::GetProjectPersistentDownloadDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir());
}

FString UCBFileHeplerBPLibrary::GetProjectPlatformExtensionsDir()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectPlatformExtensionsDir());
}

const FString UCBFileHeplerBPLibrary::QualifyFileExtension(const FString& Path, const FString& Name,
	const FString& Extension)
{
	if (CBCommon::HasEmptyParam(Path, Name, Extension))
	{
		UE_LOG(LogCBFile, Error, TEXT("%s: Filepath, Filename or Extension is empty"), *FString(__func__));
		return FString();
	}

	const FString LocalPath = QualifyPath(Path);
	const FString LocalExtension = Extension.Contains(".") ? Extension : "." + Extension;

	FString LocalName = Name;
	if (!Name.Right(Name.Len() - LocalExtension.Len()).Contains(LocalExtension))
	{
		LocalName += LocalExtension;
	}

	FString QualifiedName = LocalPath + LocalName;
	FPaths::NormalizeFilename(QualifiedName);

	UE_LOG(LogCBFile, Log, TEXT("%s: Qualified %s file path: %s"), *FString(__func__), *LocalExtension.ToUpper(), *QualifiedName);

	return QualifiedName;
}

const FString UCBFileHeplerBPLibrary::QualifyPath(const FString& Path)
{
	FString Output = Path;
	FPaths::NormalizeDirectoryName(Output);

	if (!Output.EndsWith("/") && !Output.EndsWith("\""))
	{
		Output += '/';
	}

	UE_LOG(LogCBFile, Log, TEXT("%s: Qualified directory path: %s"), *FString(__func__), *Output);

	return Output;
}

FString UCBFileHeplerBPLibrary::GetExtrasDir(const FString Plugin, const FString FileName)
{
	FString PluginDir = IPluginManager::Get().FindPlugin(Plugin)->GetBaseDir();
	FString ExtrasDir = FPaths::Combine(PluginDir, TEXT("Extras"), FileName);

	return (FPaths::ConvertRelativePathToFull(ExtrasDir));
}

FString UCBFileHeplerBPLibrary::GetExtrasDataDir(const FString Plugin)
{
	return  GetExtrasDir(Plugin,"Data");
}

FString UCBFileHeplerBPLibrary::GetExtrasExeDir(const FString Plugin)
{
	return  GetExtrasDir(Plugin,"Exe");
}

FString UCBFileHeplerBPLibrary::GetExtrasImageDir(const FString Plugin)
{
	return  GetExtrasDir(Plugin,"Image");
}

FString UCBFileHeplerBPLibrary::GetCBActorJsonConfigDir()
{
	return FPaths::ProjectConfigDir() / "ActorJsonConfig";
}

FString UCBFileHeplerBPLibrary::GetCBProjectJsonConfigDir()
{
	return FPaths::ProjectConfigDir() / "ProjectJsonConfig";
}

bool UCBFileHeplerBPLibrary::SaveBytesToFile(const TArray<uint8>& Bytes, const FString& Directory,
                                              const FString& FileName, bool bLogSave)
{
	IPlatformFile& PlatformFile=FPlatformFileManager::Get().GetPlatformFile();
	
	if (PlatformFile.CreateDirectoryTree(*Directory))
	{
		FString AbsoluteFilePath;

		// 获取绝对路径
		if (Directory.EndsWith(TEXT("/")))
		{
			AbsoluteFilePath = FPaths::ConvertRelativePathToFull(Directory + FileName);
		}
		else
		{
			AbsoluteFilePath = FPaths::ConvertRelativePathToFull(Directory + "/" + FileName);
		}
		
		bool bSaveSuccesful = FFileHelper::SaveArrayToFile(Bytes, *AbsoluteFilePath);

		if (bLogSave)
		{
			if (bSaveSuccesful)
			{
				UE_LOG(LogCBFile, Log, TEXT("Saved: %s with %d bytes"), *AbsoluteFilePath, Bytes.Num());
			}
			else
			{
				UE_LOG(LogCBFile, Log, TEXT("Failed to save: %s"), *AbsoluteFilePath);
			}
		}

		return bSaveSuccesful;
	}

	return false;
}

bool UCBFileHeplerBPLibrary::ReadBytesFromFile(const FString& Directory, const FString& FileName,
	TArray<uint8>& OutBytes)
{
	FString AbsoluteFilePath = Directory + "/" + FileName;
	
	return FFileHelper::LoadFileToArray(OutBytes, *AbsoluteFilePath);
}

UTexture2D* UCBFileHeplerBPLibrary::Conv_BytesToTexture(const TArray<uint8>& InBytes)
{
	////将 UTexture2D 转换为image
	UTexture2D* Texture = nullptr;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat DetectedFormat = ImageWrapperModule.DetectImageFormat(InBytes.GetData(), InBytes.Num());
	DetectedFormat=EImageFormat::PNG;
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(DetectedFormat);

	////设置compressed bytes——我们需要游戏线程上的这些信息来确定纹理大小，否则我们将需要一个完整的异步回调
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(InBytes.GetData(), InBytes.Num()))
	{
		//创建给定尺寸的图像
		Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
		Texture->UpdateResource();

		//在后台线程池上解压缩
		FCBLambdaRunnable::RunLambdaOnBackGroundThreadPool([ImageWrapper, Texture] {
			TArray64<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{

				FUpdateTextureData* UpdateData = new FUpdateTextureData;
				UpdateData->Texture2D = Texture;
				UpdateData->Region = FUpdateTextureRegion2D(0, 0, 0, 0, ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
				UpdateData->BufferArray = UncompressedBGRA;
				UpdateData->Pitch = ImageWrapper->GetWidth() * 4;
				UpdateData->Wrapper = ImageWrapper;
				
				ENQUEUE_RENDER_COMMAND(BytesToTextureCommand)(
					[UpdateData](FRHICommandList& CommandList)
				{
					RHIUpdateTexture2D(
						((FTextureResource*)UpdateData->Texture2D->GetResource())->TextureRHI->GetTexture2D(),
						0,
						UpdateData->Region,
						UpdateData->Pitch,
						UpdateData->BufferArray.GetData()
					);
					delete UpdateData; //释放数据
				});//End Enqueue
			}
		});
	}
	else
	{
		UE_LOG(LogCBFile,Warning, TEXT("Invalid image format cannot decode %d"), (int32)DetectedFormat);
	}

	return Texture;
}

UTexture2D* UCBFileHeplerBPLibrary::LoadImageFromFile(FString ImagePath /**=TEXT("LocalImage://")*/)
{
	//注意：此操作只针对放入CB插件对应文件夹有用
	const FString PathPrefix="LocalImage://";
	if((UKismetStringLibrary::StartsWith(ImagePath,PathPrefix,ESearchCase::IgnoreCase)))
	{
		FString File=UKismetStringLibrary::RightChop(ImagePath,PathPrefix.Len());
		ImagePath=FPaths::Combine(UCBFileHeplerBPLibrary::GetExtrasImageDir(TEXT("CBCommon"))/File);
	}

	if (!FPaths::FileExists(ImagePath))
	{
		UE_LOG(LogCBFile, Error, TEXT("No Valid Image File Found At %s"), *ImagePath);
		return nullptr;
	}

	// 获取图片字节数据
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
	{
		return nullptr;
	}

	return  Conv_BytesToTexture(FileData);
	
}

bool UCBFileHeplerBPLibrary::Conv_TextureToBytes(UTexture2D* Texture, TArray<uint8>& OutBuffer,
	ECBImageFormatBPType Format)
{
	if (!Texture || !Texture->IsValidLowLevel())
	{
		return false;
	}
	
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper((EImageFormat)Format);

	int32 Width = Texture->GetPlatformData()->Mips[0].SizeX;
	int32 Height = Texture->GetPlatformData()->Mips[0].SizeY;
	int32 DataLength = Width * Height * 4;

	void* TextureDataPointer = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_ONLY);

	ImageWrapper->SetRaw(TextureDataPointer, DataLength, Width, Height, ERGBFormat::BGRA, 8);
	
	OutBuffer = ImageWrapper->GetCompressed();

	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	return true;
}

UTexture2D* UCBFileHeplerBPLibrary::LoadImageFromExplorer(const FString& DefaultPath)
{
	TArray<FString> OutFilenames;

	FString	FileTypes = TEXT("All Files (*.png;*.jpg;)|*.png;*.jpg");

#if WITH_EDITOR
	if (GEngine && GEngine->GameViewport)
	{
		void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			//Opening the file picker!
			uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
			DesktopPlatform->OpenFileDialog(ParentWindowHandle, TEXT("Choose A File"), DefaultPath, FString(""), FileTypes, SelectionFlag, OutFilenames);
		}
	}
#elif PLATFORM_WINDOWS
	TComPtr<IFileDialog> FileDialog;
	if (SUCCEEDED(::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, IID_PPV_ARGS_Helper(&FileDialog))))
	{
		// Set up common settings
		FileDialog->SetTitle(TEXT("Choose A File"));
		if (!DefaultPath.IsEmpty())
		{
			// SHCreateItemFromParsingName requires the given path be absolute and use \ rather than / as our normalized paths do
			FString DefaultWindowsPath = FPaths::ConvertRelativePathToFull(DefaultPath);
			DefaultWindowsPath.ReplaceInline(TEXT("/"), TEXT("\\"), ESearchCase::CaseSensitive);

			TComPtr<IShellItem> DefaultPathItem;
			if (SUCCEEDED(::SHCreateItemFromParsingName(*DefaultWindowsPath, nullptr, IID_PPV_ARGS(&DefaultPathItem))))
			{
				FileDialog->SetFolder(DefaultPathItem);
			}
		}

		// Set-up the file type filters
		TArray<FString> UnformattedExtensions;
		TArray<COMDLG_FILTERSPEC> FileDialogFilters;
		{
			const FString DefaultFileTypes = TEXT("All Files (*.*)|*.*");
			DefaultFileTypes.ParseIntoArray(UnformattedExtensions, TEXT("|"), true);

			if (UnformattedExtensions.Num() % 2 == 0)
			{
				FileDialogFilters.Reserve(UnformattedExtensions.Num() / 2);
				for (int32 ExtensionIndex = 0; ExtensionIndex < UnformattedExtensions.Num();)
				{
					COMDLG_FILTERSPEC& NewFilterSpec = FileDialogFilters[FileDialogFilters.AddDefaulted()];
					NewFilterSpec.pszName = *UnformattedExtensions[ExtensionIndex++];
					NewFilterSpec.pszSpec = *UnformattedExtensions[ExtensionIndex++];
				}
			}
		}
		FileDialog->SetFileTypes(FileDialogFilters.Num(), FileDialogFilters.GetData());

		// Show the picker
		if (SUCCEEDED(FileDialog->Show(NULL)))
		{
			int32 OutFilterIndex = 0;
			if (SUCCEEDED(FileDialog->GetFileTypeIndex((UINT*)&OutFilterIndex)))
			{
				OutFilterIndex -= 1; // GetFileTypeIndex returns a 1-based index
			}

			TFunction<void(const FString&)> AddOutFilename = [&OutFilenames](const FString& InFilename)
			{
				FString& OutFilename = OutFilenames.Add_GetRef(InFilename);
				OutFilename = IFileManager::Get().ConvertToRelativePath(*OutFilename);
				FPaths::NormalizeFilename(OutFilename);
			};

			{
				IFileOpenDialog* FileOpenDialog = static_cast<IFileOpenDialog*>(FileDialog.Get());

				TComPtr<IShellItemArray> Results;
				if (SUCCEEDED(FileOpenDialog->GetResults(&Results)))
				{
					DWORD NumResults = 0;
					Results->GetCount(&NumResults);
					for (DWORD ResultIndex = 0; ResultIndex < NumResults; ++ResultIndex)
					{
						TComPtr<IShellItem> Result;
						if (SUCCEEDED(Results->GetItemAt(ResultIndex, &Result)))
						{
							PWSTR pFilePath = nullptr;
							if (SUCCEEDED(Result->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath)))
							{
								AddOutFilename(pFilePath);
								::CoTaskMemFree(pFilePath);
							}
						}
					}
				}
			}
		}
	}
#endif

	UTexture2D* Texture=nullptr;
	if (OutFilenames.Num() > 0)
	{
		UE_LOG(LogCBFile,Log,TEXT("Read image:%s"),*FPaths::ConvertRelativePathToFull(OutFilenames[0]))
		return  UCBFileHeplerBPLibrary::LoadImageFromFile(FPaths::ConvertRelativePathToFull(OutFilenames[0]));
	}
	else
	{
		UE_LOG(LogCBFile, Warning, TEXT("not valid texture"));
		return nullptr;
	}
}

FString UCBFileHeplerBPLibrary::MakeTexture2Base64EncodeData(UTexture2D* Texture)
{
	if (!Texture )
	{
		UE_LOG(LogCBFile, Warning, TEXT("Invalid texture"));
		return FString();
	}
	
	if (Texture->GetPlatformData() == nullptr)
	{
		//. maybe Dxt1 to PNG
		return FString();
	}

	if (Texture->GetPlatformData()->PixelFormat != PF_B8G8R8A8)
	{
		Texture = ConvertPixelFormatToB8G8R8A8(Texture);
	}

	const int32 Width = Texture->GetSizeX();
	const int32 Height = Texture->GetSizeY();
	
	TArray<FColor> Pixels;
	Pixels.SetNumUninitialized(Width * Height);
	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

#if WITH_EDITOR
	if (!Mip.BulkData.IsBulkDataLoaded())
	{
		Mip.BulkData.LoadBulkDataWithFileReader();
	}

#endif

	void* Data = nullptr;

	if (Mip.BulkData.IsLocked() == false)
	{

		Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
	}
	else
	{
		Mip.BulkData.Unlock();
		Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
	}
	if (Data)
	{
		FMemory::Memcpy(Pixels.GetData(), Data, Width * Height * sizeof(FColor));
		Mip.BulkData.Unlock();
	}
	else
	{
		UE_LOG(LogCBFile, Warning, TEXT("Failed to lock texture data"));
		return FString();
	}
	
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	ImageWrapper->SetRaw(Pixels.GetData(), Pixels.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
	const TArray64<uint8>& EncodedData = ImageWrapper->GetCompressed(100);

	FString Base64String;
	Base64String = FBase64::Encode(EncodedData.GetData(), EncodedData.Num());

	return Base64String;
}

UTexture2D* UCBFileHeplerBPLibrary::ConvertPixelFormatToB8G8R8A8(UTexture2D* SourceTexture)
{
	TextureCompressionSettings OldCompressionSettings = SourceTexture->CompressionSettings;
	bool OldSRGB = SourceTexture->SRGB;
	SourceTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
#if WITH_EDITOR
	SourceTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif
	SourceTexture->SRGB = false;
	SourceTexture->UpdateResource();

	int32 width = SourceTexture->GetPlatformData()->SizeX;
	int32 height = SourceTexture->GetPlatformData()->SizeY;

	const FColor* FormatedImageData = static_cast<const FColor*>(SourceTexture->GetPlatformData()->Mips[0].BulkData.LockReadOnly());
	SourceTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

	int32 Width = SourceTexture->GetPlatformData()->SizeX;
	int32 Height = SourceTexture->GetPlatformData()->SizeY;
	TArray<FColor> ColorArray;
	ColorArray.SetNumUninitialized(Width * Height);

	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{
			FColor PixelColor = FormatedImageData[Y * Width + X];
			ColorArray[Y * Width + X] = PixelColor;
		}
	}

	if (ColorArray.Num() != Width * Height)
	{
		UE_LOG(LogCBFile, Error, TEXT("ColorArray size does not match the specified width and height."));
		return nullptr;
	}

	UTexture2D* NewTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	if (!NewTexture)
	{
		UE_LOG(LogCBFile, Error, TEXT("Failed to create transient texture."));
		return nullptr;
	}

	FTexture2DMipMap& MipMap = NewTexture->GetPlatformData()->Mips[0];
	uint8* DestPixels = static_cast<uint8*>(MipMap.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			int32 Index = (y * Width + x);
			FColor Color = ColorArray[Index];
			reinterpret_cast<FColor*>(DestPixels)[Index] = Color;
		}
	}

	MipMap.BulkData.Unlock();

	NewTexture->UpdateResource();

	return NewTexture;
}



