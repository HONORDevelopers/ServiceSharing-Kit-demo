/*
 * Copyright(c) Honor Device Co., Ltd. 2020 - 2023. All rights reserved.
 * Description: 分布式SDK错误码定义
 * Author: z00018149
 * Create: 2023-03-24
 */
#pragma once
#ifndef HONOR_DISTRIBUTED_C_SDK_H
#define HONOR_DISTRIBUTED_C_SDK_H

#ifdef  DISTRIBUTEDAPI_DLL_EXPORTS
#define DISTRIBUTEDAPI_EXPORTS __declspec(dllexport)
#else
#define DISTRIBUTEDAPI_EXPORTS __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DISTRIBUTED_MAJOR_VERSION 1
#define DISTRIBUTED_MINOR_VERSION 1
#define MAX_NAME_LEN 64
#define DEVICE_ID_LEN 64
#define MAX_EXTRA_MESSAGE_LEN 1024
#define MAX_ACTION_LEN 128
#define MAX_PACKAGE_NAME_LEN 256
#define MAX_COMPONENT_NAME_LEN 512
#define MAX_BUNDLE_KEY_LEN 1024
#define MAX_BUNDLE_STRING_VALUE_LEN 4095

#define MSG_DISTRIBUTED_START_ACTIVITY              1 // 启动Activity的会话，执行过程中的消息回调和结果返回
#define MSG_DISTRIBUTED_START_ACTIVITY_FOR_RESULT   2 // 启动Activity并等待结果的消息的会话，执行过程中的消息回调
#define MSG_DISTRIBUTED_QUERY_ACTIVITIES_RESPONSE   3 // 设备能力查询结果
#define MSG_DISTRIBUTED_ACTIVITY_RESULT             4 // 通过startActivityForResult方式启动的Activity结束后，返回的数据消息
#define MSG_DISTRIBUTED_DEVICE_ONLINE               5 // 设备上线
#define MSG_DISTRIBUTED_DEVICE_OFFLINE              6 // 设备下线
#define MSG_DISTRIBUTED_FILESENDFAILED              7 // 文件发送失败
#define MSG_DISTRIBUTED_SESSION_CLOSE               8 // session close
#define MSG_DISTRIBUTED_SERVICE_START               9 // 跨端调用服务重新启动
#define MSG_DISTRIBUTED_DEVICE_UPDATE               10 // 设备信息变更

#define HONOR_DISTRIBUTED_SUCCESS(errCode) ((errCode) == ERR_DISTRIBUTED_NONE ? 1 : 0)

///
/// \brief 定义日志输出级别
/// \param  INFO: Info级别日志信息
/// \param  DEBUG: Debug级别日志信息
/// \param  WARN: Warn级别日志信息
/// \param  ERR: Error级别日志信息
///
enum DistributedLogLevel {
    INFO = 1,
    DEBUG,
    WARN,
    ERR,
};

struct DeviceInfo {
    char deviceName[MAX_NAME_LEN];// 设备名
    char deviceId[DEVICE_ID_LEN + 1];// 设备id
    unsigned char deviceType; // 0:Tv; 1:手机; 2:Watch; 3: PC; 4: Pad; 5: 未知
    char osType[MAX_NAME_LEN]; // 操作系统类型，可能是"Android", "Windows"
    char osVersion[MAX_NAME_LEN]; // 操作系统版本号
};

struct DistributedBundle;

struct DistributedMessage {
    unsigned int verison;// 兼容性问题控制，预留
    unsigned int messageId; // 消息id
    unsigned int errorCode; // 错误码
    int sessionId;
    char extraMessage[MAX_EXTRA_MESSAGE_LEN]; // 消息附带的信息
    union {
        DistributedBundle *bundle; // 附带的bundle数据
        struct {
            unsigned int dataSize;  // 消息附带的内存块大小
            void* data; // 消息附带的内存块，可用于存储附加数据, 使用方使用消息时，如果需要此部分内存数据，需要copy出来，不能直接使用，否则可能内存会被系统释放,导致内存问题
        };
    };
};

struct DistributedResovleInfo {
    char action[MAX_ACTION_LEN];
    char packageName[MAX_PACKAGE_NAME_LEN];
    char componentName[MAX_COMPONENT_NAME_LEN];
};

typedef void(*DISTRIBUTED_PROC)(const DistributedMessage* msg);

///
/// \brief 定义日志输出函数
/// \param [in] tag: 日志标签信息
/// \param [in] format: 日志输出格式
/// \param [in] ...：日志参数
///
typedef void(*DistributedLogFunc)(enum DistributedLogLevel level, const char *tag, const char *format, ...);

///
/// \brief 日志打印回调接口注册(不注册不影响功能使用)
/// \param [in] logFunc: 应用日志输出函数
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_LogRegister(DistributedLogFunc logFunc);

///
/// \brief 发送接续消息
/// \param [in] jsonBuf: 业务JSON数据
/// \param [in] jsonLen: 业务JSON数据长度
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_SendMsg(const char* jsonBuf, int jsonLen);

///
/// \brief 初始化分布式应用框架环境
///
/// \param appId: 应用云平台注册的AppId号
/// \param processName: 应用主进程名
/// \param nameLen: processName长度
/// \param lpfnProc：处理过程中的消息的处理函数的指针
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Initialize(int appId, const char* processName, int nameLen, DISTRIBUTED_PROC lpfnProc);

///
/// \brief 去注册, 应用退出前需要去注册接续框架服务
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_Release();

///
/// \brief 获取当前系统中的所有设备列表
///
/// \param [out] ppDeviceList: 保存设备的列表信息，该信息为连续数组。若无信息，则为NULL
/// \param [out] deviceCount: 保存设备的刷量信息
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_GetDeviceList(DeviceInfo** ppDeviceList, int* deviceCount);

///
/// \brief 释放DeviceList的内存
///
/// \param pDeviceList: 设备信息的内存
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_ReleaseDeviceList(DeviceInfo* pDeviceList);

///
/// \brief 获取指定设备上的能够响应action的所有组件信息，返回结果以DistributedResovleInfo的数组指针传出
///
/// \param deviceId: 指定的目标设备，由Honor_Distributed_GetDeviceList查询到的信息获得
///
/// 查询能力为同步调用查询，可能存在耗时，调用方需要考虑阻塞的可能。ppResolveInfoList返回的内存需要由使用方进行内存释放，
/// 务必使用Honor_Distributed_ReleaseResolveInfoList释放返回的设备信息，否则会出现内存泄漏
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_QueryIntentActivities(const char* deviceId);

///
/// \brief 释放ResolveInfo列表的内存
///
/// \param pResolveInfoList: 能力列表占用的内存
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_ReleaseResolveInfoList(DistributedResovleInfo* pResolveInfoList);

///
/// \brief 通过指定Action的方式启动远程设备上的Activity
///
/// \param deviceId: 指定的目标设备，由Honor_Distributed_GetDeviceList查询到的信息获得
/// \param action: 指定的Action值
/// \param bundle: 额外的参数信息，可为空
/// \param [out] sessionId：调用后，用于管理该调用请求的session
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityWithAction(const char* deviceId, const char* action,
    DistributedBundle* bundle, int* sessionId);

///
/// \brief 通过指定Action的方式启动远程设备上的Activity，并等待结果
///
/// \param deviceId: 指定的目标设备，由Honor_Distributed_GetDeviceList查询到的信息获得
/// \param action: 指定的Action值
/// \param requestCode:请求码
/// \param bundle:额外的参数信息，可为空
/// \param [out] sessionId：调用后，用于管理该调用请求的session
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityForResultWithAction(const char* deviceId, const char* action,
    int requestCode, DistributedBundle* bundle, int* sessionId);

///
/// \brief 通过指定包名+Activity组件名的方式启动远程设备上的Activity
///
/// \param deviceId: 指定的目标设备，由Honor_Distributed_GetDeviceList查询到的信息获得
/// \param packageName：指定启动的包名，由Honor_Distributed_QueryIntentActivities查询到的信息获取，不可为NULL，否则启动失败
/// \param bundle: 额外的参数信息，可为空
/// \param [out] sessionId：调用后，用于管理该调用请求的session
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityWithName(const char* deviceId, const char* packageName,
    const char* actvityName, DistributedBundle* bundle, int* sessionId);

///
/// \brief 通过指定Action的方式启动远程设备上的Activity，并等待结果
///
/// \param deviceId: 指定的目标设备，由Honor_Distributed_GetDeviceList查询到的信息获得
/// \param packageName：指定启动的包名，由Honor_Distributed_QueryIntentActivities查询到的信息获取，不可为NULL，否则启动失败
/// \param requestCode: 请求码
/// \param bundle: 额外的参数信息，可为空
/// \param [out] sessionId：调用后，用于管理该调用请求的session
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityForResultWithName(const char* deviceId, const char* packageName,
    const char* actvityName, int requestCode, DistributedBundle* bundle, int* sessionId);

///
/// \brief 通过指定Session的方式停止远程设备上的Activity
///
/// \param sessionId：需要控制的session对应的id
/// \return 参考错误码
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StopActivity(int sessionId);

///
/// \brief 创建DistributedBundle用于存储数据
///
/// \return 创建成功时返回DistributedBundle的指针对象
///
DISTRIBUTEDAPI_EXPORTS DistributedBundle* Honor_Distributed_Bundle_Create();

///
/// \brief 释放掉bundle的所有内存
///
/// \param appId: 要释放的DistributedBundle对象指针
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_Bundle_Release(DistributedBundle* bundle);

///
/// \brief 向Bundle中添加key-value数据对
///
/// \param bundle: 被操作的bundle
/// \param key: 键名
/// \param data：数据
/// \param dataLen:数据长度
/// \return true：成功 false：失败
///
DISTRIBUTEDAPI_EXPORTS bool Honor_Distributed_Bundle_Put(DistributedBundle* bundle, const char* key, const void* data, const int dataLen);

///
/// \brief 向Bundle中添加key-value数据对
///
/// \param bundle: 被操作的bundle
/// \param key: 键名
/// \param stringValue：数据
/// \return true：成功 false：失败
///
DISTRIBUTEDAPI_EXPORTS bool Honor_Distributed_Bundle_PutString(DistributedBundle* bundle, const char* key, const char* stringValue);

///
/// \brief 从Bundle中根据key名，获取对应的值
///
/// \param bundle: 被操作的bundle
/// \param key: 键名
/// \param [out] data：数据
/// \param [out] dataLen:数据长度
/// \return true：成功 false：失败
///
/// data指针指向的内存，调用方不能释放，若调用方需要长期使用，需要将数据进行copy
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Bundle_Get(DistributedBundle* bundle, const char* key, void* buffer, int bufferLen);

///
/// \brief 从Bundle中根据key名，获取对应的值
///
/// \param bundle: 被操作的bundle
/// \param key: 键名
/// \param [out] data：数据
/// \param [out] dataLen:数据长度
/// \return true：成功 false：失败
///
/// data指针指向的内存，调用方不能释放，若调用方需要长期使用，需要将数据进行copy
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Bundle_GetString(DistributedBundle* bundle, const char* key, char* buffer, int bufferLen);

///
/// \brief 将bundle的内容进行clone，深copy
///
/// \param bundle: 被clone的结构体数组
/// \return clone成功的对象地址，若不成功，则返回NULL
///
/// clone后的Bundle需要业务方自行管理内存
///
DISTRIBUTEDAPI_EXPORTS DistributedBundle* Honor_Distributed_Bundle_Clone(DistributedBundle* bundle);

///
/// \brief 获取当前 SDK 版本
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_Version();

///
/// \brief 将DistributedMessage结构体转成Json string
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_MsgToJson(const DistributedMessage* msg);

///
/// \brief 将DeviceInfoList结构体数组转成Json string
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_DeviceInfoListToJson(DeviceInfo* ppDeviceList, int deviceCount);


#ifdef __cplusplus
}
#endif
#endif