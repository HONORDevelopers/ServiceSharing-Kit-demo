/*
 * Copyright(c) Honor Device Co., Ltd. 2020 - 2023. All rights reserved.
 * Description: �ֲ�ʽSDK�����붨��
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

#define MSG_DISTRIBUTED_START_ACTIVITY              1 // ����Activity�ĻỰ��ִ�й����е���Ϣ�ص��ͽ������
#define MSG_DISTRIBUTED_START_ACTIVITY_FOR_RESULT   2 // ����Activity���ȴ��������Ϣ�ĻỰ��ִ�й����е���Ϣ�ص�
#define MSG_DISTRIBUTED_QUERY_ACTIVITIES_RESPONSE   3 // �豸������ѯ���
#define MSG_DISTRIBUTED_ACTIVITY_RESULT             4 // ͨ��startActivityForResult��ʽ������Activity�����󣬷��ص�������Ϣ
#define MSG_DISTRIBUTED_DEVICE_ONLINE               5 // �豸����
#define MSG_DISTRIBUTED_DEVICE_OFFLINE              6 // �豸����
#define MSG_DISTRIBUTED_FILESENDFAILED              7 // �ļ�����ʧ��
#define MSG_DISTRIBUTED_SESSION_CLOSE               8 // session close
#define MSG_DISTRIBUTED_SERVICE_START               9 // ��˵��÷�����������
#define MSG_DISTRIBUTED_DEVICE_UPDATE               10 // �豸��Ϣ���

#define HONOR_DISTRIBUTED_SUCCESS(errCode) ((errCode) == ERR_DISTRIBUTED_NONE ? 1 : 0)

///
/// \brief ������־�������
/// \param  INFO: Info������־��Ϣ
/// \param  DEBUG: Debug������־��Ϣ
/// \param  WARN: Warn������־��Ϣ
/// \param  ERR: Error������־��Ϣ
///
enum DistributedLogLevel {
    INFO = 1,
    DEBUG,
    WARN,
    ERR,
};

struct DeviceInfo {
    char deviceName[MAX_NAME_LEN];// �豸��
    char deviceId[DEVICE_ID_LEN + 1];// �豸id
    unsigned char deviceType; // 0:Tv; 1:�ֻ�; 2:Watch; 3: PC; 4: Pad; 5: δ֪
    char osType[MAX_NAME_LEN]; // ����ϵͳ���ͣ�������"Android", "Windows"
    char osVersion[MAX_NAME_LEN]; // ����ϵͳ�汾��
};

struct DistributedBundle;

struct DistributedMessage {
    unsigned int verison;// ������������ƣ�Ԥ��
    unsigned int messageId; // ��Ϣid
    unsigned int errorCode; // ������
    int sessionId;
    char extraMessage[MAX_EXTRA_MESSAGE_LEN]; // ��Ϣ��������Ϣ
    union {
        DistributedBundle *bundle; // ������bundle����
        struct {
            unsigned int dataSize;  // ��Ϣ�������ڴ���С
            void* data; // ��Ϣ�������ڴ�飬�����ڴ洢��������, ʹ�÷�ʹ����Ϣʱ�������Ҫ�˲����ڴ����ݣ���Ҫcopy����������ֱ��ʹ�ã���������ڴ�ᱻϵͳ�ͷ�,�����ڴ�����
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
/// \brief ������־�������
/// \param [in] tag: ��־��ǩ��Ϣ
/// \param [in] format: ��־�����ʽ
/// \param [in] ...����־����
///
typedef void(*DistributedLogFunc)(enum DistributedLogLevel level, const char *tag, const char *format, ...);

///
/// \brief ��־��ӡ�ص��ӿ�ע��(��ע�᲻Ӱ�칦��ʹ��)
/// \param [in] logFunc: Ӧ����־�������
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_LogRegister(DistributedLogFunc logFunc);

///
/// \brief ���ͽ�����Ϣ
/// \param [in] jsonBuf: ҵ��JSON����
/// \param [in] jsonLen: ҵ��JSON���ݳ���
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_SendMsg(const char* jsonBuf, int jsonLen);

///
/// \brief ��ʼ���ֲ�ʽӦ�ÿ�ܻ���
///
/// \param appId: Ӧ����ƽ̨ע���AppId��
/// \param processName: Ӧ����������
/// \param nameLen: processName����
/// \param lpfnProc����������е���Ϣ�Ĵ�������ָ��
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Initialize(int appId, const char* processName, int nameLen, DISTRIBUTED_PROC lpfnProc);

///
/// \brief ȥע��, Ӧ���˳�ǰ��Ҫȥע�������ܷ���
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_Release();

///
/// \brief ��ȡ��ǰϵͳ�е������豸�б�
///
/// \param [out] ppDeviceList: �����豸���б���Ϣ������ϢΪ�������顣������Ϣ����ΪNULL
/// \param [out] deviceCount: �����豸��ˢ����Ϣ
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_GetDeviceList(DeviceInfo** ppDeviceList, int* deviceCount);

///
/// \brief �ͷ�DeviceList���ڴ�
///
/// \param pDeviceList: �豸��Ϣ���ڴ�
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_ReleaseDeviceList(DeviceInfo* pDeviceList);

///
/// \brief ��ȡָ���豸�ϵ��ܹ���Ӧaction�����������Ϣ�����ؽ����DistributedResovleInfo������ָ�봫��
///
/// \param deviceId: ָ����Ŀ���豸����Honor_Distributed_GetDeviceList��ѯ������Ϣ���
///
/// ��ѯ����Ϊͬ�����ò�ѯ�����ܴ��ں�ʱ�����÷���Ҫ���������Ŀ��ܡ�ppResolveInfoList���ص��ڴ���Ҫ��ʹ�÷������ڴ��ͷţ�
/// ���ʹ��Honor_Distributed_ReleaseResolveInfoList�ͷŷ��ص��豸��Ϣ�����������ڴ�й©
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_QueryIntentActivities(const char* deviceId);

///
/// \brief �ͷ�ResolveInfo�б���ڴ�
///
/// \param pResolveInfoList: �����б�ռ�õ��ڴ�
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_ReleaseResolveInfoList(DistributedResovleInfo* pResolveInfoList);

///
/// \brief ͨ��ָ��Action�ķ�ʽ����Զ���豸�ϵ�Activity
///
/// \param deviceId: ָ����Ŀ���豸����Honor_Distributed_GetDeviceList��ѯ������Ϣ���
/// \param action: ָ����Actionֵ
/// \param bundle: ����Ĳ�����Ϣ����Ϊ��
/// \param [out] sessionId�����ú����ڹ���õ��������session
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityWithAction(const char* deviceId, const char* action,
    DistributedBundle* bundle, int* sessionId);

///
/// \brief ͨ��ָ��Action�ķ�ʽ����Զ���豸�ϵ�Activity�����ȴ����
///
/// \param deviceId: ָ����Ŀ���豸����Honor_Distributed_GetDeviceList��ѯ������Ϣ���
/// \param action: ָ����Actionֵ
/// \param requestCode:������
/// \param bundle:����Ĳ�����Ϣ����Ϊ��
/// \param [out] sessionId�����ú����ڹ���õ��������session
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityForResultWithAction(const char* deviceId, const char* action,
    int requestCode, DistributedBundle* bundle, int* sessionId);

///
/// \brief ͨ��ָ������+Activity������ķ�ʽ����Զ���豸�ϵ�Activity
///
/// \param deviceId: ָ����Ŀ���豸����Honor_Distributed_GetDeviceList��ѯ������Ϣ���
/// \param packageName��ָ�������İ�������Honor_Distributed_QueryIntentActivities��ѯ������Ϣ��ȡ������ΪNULL����������ʧ��
/// \param bundle: ����Ĳ�����Ϣ����Ϊ��
/// \param [out] sessionId�����ú����ڹ���õ��������session
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityWithName(const char* deviceId, const char* packageName,
    const char* actvityName, DistributedBundle* bundle, int* sessionId);

///
/// \brief ͨ��ָ��Action�ķ�ʽ����Զ���豸�ϵ�Activity�����ȴ����
///
/// \param deviceId: ָ����Ŀ���豸����Honor_Distributed_GetDeviceList��ѯ������Ϣ���
/// \param packageName��ָ�������İ�������Honor_Distributed_QueryIntentActivities��ѯ������Ϣ��ȡ������ΪNULL����������ʧ��
/// \param requestCode: ������
/// \param bundle: ����Ĳ�����Ϣ����Ϊ��
/// \param [out] sessionId�����ú����ڹ���õ��������session
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StartActivityForResultWithName(const char* deviceId, const char* packageName,
    const char* actvityName, int requestCode, DistributedBundle* bundle, int* sessionId);

///
/// \brief ͨ��ָ��Session�ķ�ʽֹͣԶ���豸�ϵ�Activity
///
/// \param sessionId����Ҫ���Ƶ�session��Ӧ��id
/// \return �ο�������
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_StopActivity(int sessionId);

///
/// \brief ����DistributedBundle���ڴ洢����
///
/// \return �����ɹ�ʱ����DistributedBundle��ָ�����
///
DISTRIBUTEDAPI_EXPORTS DistributedBundle* Honor_Distributed_Bundle_Create();

///
/// \brief �ͷŵ�bundle�������ڴ�
///
/// \param appId: Ҫ�ͷŵ�DistributedBundle����ָ��
///
DISTRIBUTEDAPI_EXPORTS void Honor_Distributed_Bundle_Release(DistributedBundle* bundle);

///
/// \brief ��Bundle�����key-value���ݶ�
///
/// \param bundle: ��������bundle
/// \param key: ����
/// \param data������
/// \param dataLen:���ݳ���
/// \return true���ɹ� false��ʧ��
///
DISTRIBUTEDAPI_EXPORTS bool Honor_Distributed_Bundle_Put(DistributedBundle* bundle, const char* key, const void* data, const int dataLen);

///
/// \brief ��Bundle�����key-value���ݶ�
///
/// \param bundle: ��������bundle
/// \param key: ����
/// \param stringValue������
/// \return true���ɹ� false��ʧ��
///
DISTRIBUTEDAPI_EXPORTS bool Honor_Distributed_Bundle_PutString(DistributedBundle* bundle, const char* key, const char* stringValue);

///
/// \brief ��Bundle�и���key������ȡ��Ӧ��ֵ
///
/// \param bundle: ��������bundle
/// \param key: ����
/// \param [out] data������
/// \param [out] dataLen:���ݳ���
/// \return true���ɹ� false��ʧ��
///
/// dataָ��ָ����ڴ棬���÷������ͷţ������÷���Ҫ����ʹ�ã���Ҫ�����ݽ���copy
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Bundle_Get(DistributedBundle* bundle, const char* key, void* buffer, int bufferLen);

///
/// \brief ��Bundle�и���key������ȡ��Ӧ��ֵ
///
/// \param bundle: ��������bundle
/// \param key: ����
/// \param [out] data������
/// \param [out] dataLen:���ݳ���
/// \return true���ɹ� false��ʧ��
///
/// dataָ��ָ����ڴ棬���÷������ͷţ������÷���Ҫ����ʹ�ã���Ҫ�����ݽ���copy
///
DISTRIBUTEDAPI_EXPORTS int Honor_Distributed_Bundle_GetString(DistributedBundle* bundle, const char* key, char* buffer, int bufferLen);

///
/// \brief ��bundle�����ݽ���clone����copy
///
/// \param bundle: ��clone�Ľṹ������
/// \return clone�ɹ��Ķ����ַ�������ɹ����򷵻�NULL
///
/// clone���Bundle��Ҫҵ�����й����ڴ�
///
DISTRIBUTEDAPI_EXPORTS DistributedBundle* Honor_Distributed_Bundle_Clone(DistributedBundle* bundle);

///
/// \brief ��ȡ��ǰ SDK �汾
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_Version();

///
/// \brief ��DistributedMessage�ṹ��ת��Json string
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_MsgToJson(const DistributedMessage* msg);

///
/// \brief ��DeviceInfoList�ṹ������ת��Json string
///
DISTRIBUTEDAPI_EXPORTS const char* Honor_Distrubuted_DeviceInfoListToJson(DeviceInfo* ppDeviceList, int deviceCount);


#ifdef __cplusplus
}
#endif
#endif