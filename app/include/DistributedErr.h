/*
 * Copyright(c) Honor Device Co., Ltd. 2020 - 2023. All rights reserved.
 * Description: 分布式SDK错误码定义
 * Author: z00018149
 * Create: 2023-03-24
 */
#pragma once
#define ERR_DISTRIBUTED_NONE                      0 //成功
#define ERR_DISTRIBUTED_SECURITY                  -1 //安全问题
#define ERR_DISTRIBUTED_BAD_PARCELABLE            -2 //序列化失败
#define ERR_DISTRIBUTED_ILLEGAL_ARGUMENT          -3 //非法状态
#define ERR_DISTRIBUTED_NULL_POINTER              -4 //空指针
#define ERR_DISTRIBUTED_ILLEGAL_STATE             -5 //非法状态
#define ERR_DISTRIBUTED_NETWORK_MAIN_THREAD       -6 //在主线程中进行网络处理
#define ERR_DISTRIBUTED_UNSUPPORTED_OPERATION     -7 //不支持的操作
#define ERR_DISTRIBUTED_SERVICE_SPECIFIC          -8 //service指定错误
#define ERR_DISTRIBUTED_PARCELABLE                -9 //序列化
#define ERR_DISTRIBUTED_DEVICE_OFFLINE            -1000 //设备下线
#define ERR_DISTRIBUTED_DEVICE_BUSY               -1001 //设备繁忙，处于冲突状态
#define ERR_DISTRIBUTED_USER_ABORT                -1002 //用户中断操作
#define ERR_DISTRIBUTED_PERMISSION_DENIED         -1003 //权限拒绝
#define ERR_DISTRIBUTED_NETWORK_TYPE_NOT_ALLOW    -1004 //当前网络不允许
#define ERR_DISTRIBUTED_INVALID_SESSION           -1005 //非法的会话
#define ERR_DISTRIBUTED_IMCOMPATIABLE             -1006 //版本不兼容
#define ERR_DISTRIBUTED_PARSE_ERROR               -1007 //解析错误

/// 返回值及错误码定义
#define ERR_DISTRIBUTED_SDK_UNKNOWN               -2000  // 未知错误
#define ERR_DISTRIBUTED_SDK_MODULE_INIT_FAILED    -2001  // SDK初始化失败,加载SDK进程过多
#define ERR_DISTRIBUTED_SDK_REGISTER_ABNORMAL     -2002  // SDK注册app信息失败
#define ERR_DISTRIBUTED_CONNECT_TO_SERVICE_FAILED -2004  // 连接到PC分布式框架失败
#define ERR_DISTRIBUTED_CONNECT_AUTH_FAIL         -2005  // 连接到PC分布式框架鉴权失败
#define ERR_DISTRIBUTED_SIGN_CHECK_FAILED         -2006  // 应用主进程签名校验失败
#define ERR_DISTRIBUTED_INSTALL_CHECK_ERROR       -2007  // 应用安装状态异常
