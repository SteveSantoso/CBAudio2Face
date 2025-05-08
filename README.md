# CBAudio2Face UE5 Plugin

## 概述
CBAudio2Face是一个基于Unreal Engine 5.4的插件，用于控制Audio2Face功能。该插件通过HTTP请求与Audio2Face服务通信，提供面部动画控制、音频播放和表情设置等功能。

## 主要功能
- 初始化Audio2Face环境
- 加载USD文件
- 播放/停止音频
- 设置面部表情参数
- 控制音频播放范围和时间
- 流式传输控制

## 安装方法
1. 将插件文件夹复制到您的项目Plugins目录下
2. 重新生成项目文件
3. 在编辑器中启用插件

## 接口说明
### CBAudio2FaceSubsystem
- `InitA2F()`: 初始化Audio2Face环境
- `PlayA2F()`: 播放音频并驱动面部动画
- `StopPlayA2F()`: 停止播放
- `GetWavPath()`: 获取音频文件路径

### CBAudio2FaceNodes
提供多种HTTP请求代理类，包括:

#### 状态检查
- `SendHttp2FStatusRequest()`: 获取Audio2Face服务状态

#### USD文件设置
- `SendHttp2FSetUSDRequest(FString USDFileName)`: 设置USD文件路径
- `SendHttp2FSetSettingsRequest(FString Instance, FAudio2FaceSettings InSettings)`: 设置实例参数
- `SendHttp2FSetSettingsRequestByKeys(FString Instance, TMap<FString,float> InSettings)`: 通过键值对设置实例参数

#### 表情权重设置
- `SendHttpA2FSetEmotionWeightsByFrameRequest(FString Instance, FA2FEmotion InEmotion)`: 通过帧设置表情权重
- `SendHttpA2FSetEmotionWeightsByNameRequest(FString Instance, TMap<FString,float> InEmotion)`: 通过名称设置表情权重
- `SendHttpA2FSetEmotionSettingsRequest(FString Instance, FAudio2EmotionSettings Settings)`: 设置表情参数

#### 音频播放控制
- `SendHttpA2FPlayerPlayRequest(FString PlayerInstance)`: 播放音频
- `SendHttpA2FPlayerPauseRequest(FString PlayerInstance)`: 暂停音频
- `SendHttpA2FPlayerRewindRequest(FString PlayerInstance)`: 倒带音频
- `SendHttpA2FSetPlayerTrackRequest(FString PlayerInstance, FA2FPlayerTrack PlayerTrack)`: 设置播放音轨
- `SendHttpA2FSetPlayerRangeRequest(FString PlayerInstance, float Start, float End)`: 设置播放时间范围
- `SendHttpA2FSetPlayerCurrentTimeRequest(FString PlayerInstance, float Time)`: 设置当前播放时间
- `SendHttpA2FSetPlayerLoopingRequest(FString PlayerInstance, bool bLoop)`: 设置循环播放

#### 流式传输控制
- `SendHttpA2FSetEnableStreamingRequest(FString Instance, bool bEnable)`: 启用/禁用流式传输
- `SendHttpA2FActivateStreamLivelinkRequest(FString NodePath, bool bActive)`: 激活/停用实时流式传输

## 注意事项
- 需要配置正确的服务器URL
- 确保USD文件路径正确
- 音频文件需放置在指定目录

## 官方文档
- [NVIDIA Audio2Face 官方文档](https://www.nvidia.cn/omniverse/apps/audio2face/)
