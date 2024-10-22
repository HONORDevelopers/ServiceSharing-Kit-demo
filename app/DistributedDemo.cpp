/*
 * Copyright(c) Honor Device Co., Ltd. 2024 - 2027. All rights reserved.
 * Description: ServiceSharing Kit Demo
 * Author:
 * Create: 2024-10-12
 */

#include <windows.h>
#include <stdexcept>
#include "DistributedDemo.h"
#include <fstream>

using namespace std;
using namespace DistributedDemo;
#define LOG_LEN 2000
#define SAFE_DELETE_BUNDLE(bundle) if (bundle != nullptr) { \
                                        delete bundle; \
                                        bundle = nullptr;}
const static std::string COMMAND_VIEW_DEVICE_LIST = "view"; // 查询当前设备列表
const static std::string COMMAND_START = "start"; // 发起调用
const static std::string COMMAND_STOP = "stop"; // 停止调用
const static std::string PROMPT = "\n\nEnter \"view\" to view the current device list;\nEnter \"start deviceIndex, actionIndex\"  to Start activity,like \"start 0,0\";\nEnter \"stop\" to cancel currnt session\n";
const static std::string INPUTERR_PROMPT = "\Wrong Input!,Enter \"view\" to view the current device list;\nEnter \"start deviceIndex, actionIndex\"  to Start activity,like \"start 0,0\";\nEnter \"stop\" to cancel current session\n";
const static std::string PROCESS_NAME = "DistributedDemo.exe";  // 应用主进程名
const static int APPLICATION_ID = 400100032; // 应用云平台注册的AppId号,三方应用+
const static std::string ACTION_IMAGE_CAPTURE = "android.media.action.IMAGE_CAPTURE"; // 拍照
const static std::string ACTION_VIDEO_CAPTURE = "android.media.action.VIDEO_CAPTURE"; // 录像
const static std::string ACTION_AISCAN_DOC_SCAN = "com.hihonor.lens.action.AISCAN_DOC_SCAN"; // 文档扫描
const static std::string ACTION_AISCAN_TEXT_EXTRACT = "com.hihonor.lens.action.AISCAN_TABLE_EXTRACT"; // 表格提取
const static std::string ACTION_AISCAN_TABLE_EXTRACT = "com.hihonor.lens.action.AISCAN_TEXT_EXTRACT"; // 文字提取
const static std::string ACTION_AISCAN_IMAGE_EXTRACT = "com.hihonor.lens.action.AISCAN_IMAGE_EXTRACT"; // 抠图

bool ParseStringToJson(nlohmann::json& jsonData, const std::string& data)
{
    if (data.empty()) {
        return false;
    }
    if (!nlohmann::json::accept(data)) {
        return false;
    }
    jsonData = nlohmann::json::parse(data.c_str());
    return true;
}

bool GetValueBykey(nlohmann::json jsonData, const std::string& key, nlohmann::json& jsonValue)
{
    if (!jsonData.contains(key)) {
        return false;
    }
    jsonValue = jsonData[key];
    return true;
}

template<typename T>
bool GetJsonValue(nlohmann::json jsonData, const std::string& key, T& value)
{
    nlohmann::json jsonValue;
    if (!GetValueBykey(jsonData, key, jsonValue)) {
        return false;
    }
    try {
        value = jsonValue.get<T>();
    }
    catch (std::exception) {
        return false;
    }
    return true;
}

bool GetJsonArray(nlohmann::json jsonData, const std::string& key, std::vector<nlohmann::json>& arrayValue)
{
    arrayValue.clear();
    nlohmann::json jsonArray;
    if (!GetValueBykey(jsonData, key, jsonArray)) {
        return false;
    }
    if (!jsonArray.is_array()) {
        return false;
    }
    for (auto it = jsonArray.begin(); it != jsonArray.end(); it++) {
        try {
            nlohmann::json jsonValue = (*it).get<nlohmann::json>();
            arrayValue.push_back(jsonValue);
        }
        catch (std::exception) {
            return false;
        }
    }
    return true;
}

HonorDistributedDemo *HonorDistributedDemo::GetInstance()
{
    static HonorDistributedDemo honorHandoffDemo;
    return &honorHandoffDemo;
}

///
/// \brief 获取设备列表
///
void HonorDistributedDemo::GetDeviceList()
{
    LOGGER_WARN("HonorDistributedDemo::GetDeviceList: enter.");
    DeviceInfo* pDeviceList = nullptr;
    int deviceCount;
    // 调用Kit接口 Honor_Distributed_GetDeviceList
    int res = Honor_Distributed_GetDeviceList(&pDeviceList, &deviceCount);
    if (res == ERR_DISTRIBUTED_NONE) {
        if (pDeviceList == nullptr || deviceCount <= 0) {
            LOGGER_WARN("HonorDistributedDemo::GetDeviceList: No Devices Found.");
            std::cout << "No Device Found, Please go to HONORWorkStation to check that there is any device online in Magic Ring" << endl << endl;
            return;
        }
        LOGGER_WARN("GetDeviceList:");
        // 将信息拷贝出来
        for (int i = 0; i < deviceCount; i++) {
            DeviceInfo* curDevice = &pDeviceList[i];
            if (curDevice != nullptr) {
                HonorDeviceInfo deviceInfo;
                deviceInfo.deviceId = curDevice->deviceId;
                deviceInfo.deviceName = curDevice->deviceName;
                deviceInfo.osType = curDevice->osType;
                deviceInfo.osVersion = curDevice->osVersion;
                deviceInfo.deviceType = curDevice->deviceType;
                deviceMap[deviceInfo.deviceId] = deviceInfo;
            }
            LOGGER_WARN("GetDeviceList: index:" << i << ", deviceId:" << curDevice->deviceId << ", deviceName:" << curDevice->deviceName);
        }
    } else {
        LOGGER_WARN("Honor_Distributed_GetDeviceList Failed, result: " << res);
        std::cout << "No Device Found, Please go to HONORWorkStation to check that there is any device online in Magic Ring" << endl << endl;
    }
    // 使用完，pDeviceList需要释放内存
    if (pDeviceList != nullptr) {
        Honor_Distributed_ReleaseDeviceList(pDeviceList);
    }
}

///
/// \brief 查询指定设备支持跨设备调用的action
///
/// \param deviceId: 指定的目标设备id
///
void HonorDistributedDemo::QueryIntentActivities(std::string deviceId)
{
    // 调用Kit接口 Honor_Distributed_QueryIntentActivities
    int ret = Honor_Distributed_QueryIntentActivities(deviceId.c_str());
    if (ret != ERR_DISTRIBUTED_NONE) {
        LOGGER_WARN("call Honor_Distributed_QueryIntentActivities, ret:" << ret);
    }
}

///
/// \brief 查询所有设备支持跨设备调用的action
///
void HonorDistributedDemo::QueryAllDeviceIntentActivities()
{
    for (auto iter = deviceMap.begin(); iter != deviceMap.end(); ++iter) {
        // 调用Kit接口 Honor_Distributed_QueryIntentActivities
        int ret = Honor_Distributed_QueryIntentActivities(iter->second.deviceId.c_str());
        if (ret != ERR_DISTRIBUTED_NONE) {
            LOGGER_WARN("call Honor_Distributed_QueryIntentActivities, ret:" << ret);
        }
    }
}

///
/// \brief 获取bundle中指定key的value
///
std::string DistributedDemo::HonorDistributedDemo::GetBundleString(DistributedBundle * bundle, std::string key)
{
    char outBuffer[4096];
    int pathLen;
    // 调用Kit接口 Honor_Distributed_Bundle_GetString
    int readSize = Honor_Distributed_Bundle_GetString(bundle, key.c_str(), outBuffer, sizeof(outBuffer));
    if (readSize) {
        std::string value(outBuffer, readSize);
        return value;
    }
    LOGGER_WARN("readSize == 0!!!");
    return "";
}

///
/// \brief 打印当前的设备及其action列表
///
void HonorDistributedDemo::ShowDeviceListView()
{
    std::cout << "\n\ShowDeviceListView:" << endl;
    int deviceIndex = 0;
    for (auto iter = deviceMap.begin(); iter != deviceMap.end(); iter++) {
        std::cout << "deviceIndex:" << deviceIndex << ", deviceId:" << iter->first << ", devicename:" << iter->second.deviceName << "\nactionList:" << endl;
        for (int actionIndex = 0; actionIndex < iter->second.actionList.size(); actionIndex++) {
            std::cout << "    actionIndex:" << actionIndex << ", action:" << iter->second.actionList[actionIndex] << endl;
        }
        deviceIndex++;
    }
}

///
/// \brief 发起跨设备服务调用
///
/// \param deviceIndex:指定设备在map中的下标， actionIndex:指定action在vector中的下标
///
void HonorDistributedDemo::StartAction(int deviceIndex, int actionIndex)
{
    if (deviceMap.size() < (deviceIndex + 1)) {
        std::cout << "deviceIndex is out of range" << endl;
        return;
    }
    auto iter = deviceMap.begin();
    std::advance(iter, deviceIndex);
    if (iter->second.actionList.size() < (actionIndex + 1)) {
        std::cout << "actionIndex is out of range" << endl;
        return;
    }
    DistributedBundle* bundle = nullptr;
    int sessionId;
    // 调用Kit接口 Honor_Distributed_StartActivityWithAction
    int result = Honor_Distributed_StartActivityWithAction(iter->first.c_str(), iter->second.actionList[actionIndex].c_str(), bundle, &sessionId);
    if (result == ERR_DISTRIBUTED_NONE) {
        currentSessionId_ = sessionId;
        currentDeviceId_ = iter->first.c_str();
    } else {
        currentSessionId_ = -1;
        currentDeviceId_.clear();
        LOGGER_WARN("StartAction Fail! result：" << result);
    }
    // 使用完需释放bundle
    Honor_Distributed_Bundle_Release(bundle);
}

///
/// \brief 停止当前正在进行的跨设备服务调用
///
void HonorDistributedDemo::StopActivity()
{
    if (currentSessionId_ == -1) {
        std::cout << "No session are currently running" << endl;
        return;
    }
    // 调用Kit接口 Honor_Distributed_StopActivity
    int result = Honor_Distributed_StopActivity(currentSessionId_);
    if (result == ERR_DISTRIBUTED_NONE) {
        currentSessionId_ = -1;
        currentDeviceId_.clear();
        LOGGER_WARN("Close session success，sessionId:" << currentSessionId_);
    } else {
        LOGGER_WARN("Close session failed，sessionId:" << currentSessionId_ << ", result:" << result);
    }
}

///
/// \brief 消息回调函数，注册给Kit后，Kit依此向Demo发消息，消息结构体：DistributedMessage
///
void MsgHandle(const DistributedMessage* msg)
{
    HonorDistributedDemo::GetInstance()->MsgHandler(msg);
}

int HonorDistributedDemo::MsgHandler(const DistributedMessage* msg)
{
    LOGGER_WARN("Receive msg, messageId：" << msg->messageId << "sessionId:" << msg->sessionId << "errorCode:" << msg->errorCode);
    // 可以解析DistributedMessage得到细节数据，不过我们无法知晓bundle中具体有哪些key， 且bundle没有json使用方便
    // 所以我们后来又添加了Honor_Distrubuted_MsgToJson方法将DistributedMessage结构体转换成json string
    std::string msgJsonStr = Honor_Distrubuted_MsgToJson(msg);
    LOGGER_WARN("Receive msg, tojsonstr:" << msgJsonStr);

    switch (msg->messageId) {
        case MSG_DISTRIBUTED_START_ACTIVITY:
        {
            if (!HONOR_DISTRIBUTED_SUCCESS(msg->errorCode)) {
                currentSessionId_ = -1;
                currentDeviceId_.clear();
                std::cout << "Start activity failed! sessionId:" << msg->sessionId << "errorCode:" << msg->errorCode << endl;
                LOGGER_WARN("Start activity failed! sessionId:" << msg->sessionId << "errorCode:" << msg->errorCode);
            } else {
                // 收到安卓端的拍摄或扫描结果
                DistributedBundle* bundle = msg->bundle;
                if (bundle == nullptr) {
                    currentSessionId_ = -1;
                    currentDeviceId_.clear();
                    std::cout << "Start activity failed! bundle is empty, sessionId:" << msg->sessionId << endl;
                    LOGGER_WARN("Start activity failed! bundle is empty, sessionId:" << msg->sessionId);
                    break;
                }
                // 获取拍摄或扫描结果文件路径
                // 文件格式
                // 拍照 : jpg
                // 录像 : mp4
                // 文档扫描 : zip (zip文件中有json和jpg文件，json中记录了此次文档扫描所有jpg的路径)
                // 表格提取 : 安卓设备安装了"荣耀文档"APP  ?  xlsx : html 
                // 文字提取 : txt
                // 抠图 : png
                std::string filePath = GetBundleString(bundle, CONSTANT_FILE_PATH);
                LOGGER_WARN("receive file, path:" << filePath);
                std::cout << "receive file, path:" << filePath << endl;
                ShellExecuteA(NULL, "open", "explorer", filePath.c_str(), NULL, SW_SHOW);
            }
            break;
        }
        case MSG_DISTRIBUTED_QUERY_ACTIVITIES_RESPONSE:
        {
            // 设备能力查询结果返回
            DistributedBundle* bundle = msg->bundle;
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::string bundleStringActivityList = GetBundleString(bundle, CONSTANT_ACTIVITYLIST);
            LOGGER_WARN("bundle bundleStringActivityList:" << bundleStringActivityList);
            nlohmann::json jsonActivities;
            if (!ParseStringToJson(jsonActivities, bundleStringActivityList)) {
                std::cout << "Fail to parse bundleStringActivityList to nlohmann::json." << endl;
                return 0;
            }

            vector<nlohmann::json> actionList;
            GetJsonArray(jsonActivities, CONSTANT_ACTIVITYLIST, actionList);
            deviceMap[deviceId].actionList.clear();
            std::string actionLogStr = "\n";
            for (auto jsonData : actionList) {
                std::string action;
                GetJsonValue(jsonData, CONSTANT_ACTION, action);
                deviceMap[deviceId].actionList.emplace_back(action);
                actionLogStr.append(action).append(",\n");
            }
            LOGGER_WARN("Receive Query Activities Response, deviceId:" << deviceId
                << ",deviceName:" << deviceMap[deviceId].deviceName
                << ",actions:" << actionLogStr);

            ShowDeviceListView();
            break;
        }
        case MSG_DISTRIBUTED_ACTIVITY_RESULT:
        {
            if (!HONOR_DISTRIBUTED_SUCCESS(msg->errorCode)) {
                currentSessionId_ = -1;
                currentDeviceId_.clear();
                std::cout << "Get start activity response failed, sessionId:" << msg->sessionId << "errorCode:" << msg->errorCode << endl;
                LOGGER_WARN("Get start activity response failed, sessionId:" << msg->sessionId << "errorCode:" << msg->errorCode);
            }
            break;
        }
        case MSG_DISTRIBUTED_DEVICE_ONLINE:
        {
            // 设备在PC信任环上线
            DistributedBundle* bundle = msg->bundle;
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::string deviceName = GetBundleString(bundle, CONSTANT_DEVICE_NAME);
            std::string deviceType = GetBundleString(bundle, CONSTANT_DEVICE_TYPE);
            std::cout << "Device Online"
                << ", " << CONSTANT_DEVICE_ID << " : " << deviceId
                << ", " << CONSTANT_DEVICE_NAME << " : " << deviceName
                << ", " << CONSTANT_DEVICE_TYPE << " : " << deviceType << endl;
            HonorDeviceInfo deviceInfo;
            deviceInfo.deviceId = deviceId;
            deviceInfo.deviceName = deviceName;
            deviceInfo.deviceType = deviceType.c_str()[0];
            // 存到deviceMap
            deviceMap[deviceInfo.deviceId] = deviceInfo;
            // 查询该设备支持的action
            QueryIntentActivities(deviceId);
            break;
        }
        case MSG_DISTRIBUTED_DEVICE_OFFLINE:
        {
            // 设备在PC信任环下线
            DistributedBundle* bundle = msg->bundle;
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::cout << "Device Offline" << ", " << CONSTANT_DEVICE_ID << " : " << deviceId << endl;
            deviceMap.erase(deviceId);
            if (currentDeviceId_ == deviceId) {
                // 视为本次调用出现异常，执行清理
                currentSessionId_ = -1;
                currentDeviceId_.clear();
            }
            ShowDeviceListView();
            break;
        }
        case MSG_DISTRIBUTED_FILESENDFAILED:
        {
            // 安卓端文件发送失败
            DistributedBundle* bundle = msg->bundle;
            std::string sessionId = GetBundleString(bundle, CONSTANT_SESSION_ID);
            std::string action = GetBundleString(bundle, CONSTANT_ACTION);
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::string deviceName = GetBundleString(bundle, CONSTANT_DEVICE_NAME);
            std::cout << "File Send Failed!"
                << ", " << CONSTANT_SESSION_ID << " : " << sessionId
                << ", " << CONSTANT_ACTION << " : " << action
                << ", " << CONSTANT_DEVICE_ID << " : " << deviceId
                << ", " << CONSTANT_DEVICE_NAME << " : " << deviceName << endl;
            if (sessionId == to_string(currentSessionId_)) {
                // 视为本次调用出现异常，执行清理
                currentSessionId_ = -1;
                currentDeviceId_.clear();
            }
            break;
        }
        case MSG_DISTRIBUTED_SESSION_CLOSE:
        {
            // 与安卓端的通信断开
            DistributedBundle* bundle = msg->bundle;
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::cout << "Session Close! "
                << CONSTANT_DEVICE_ID << " : " << deviceId;
            LOGGER_WARN("Session Close! deviceId:" << deviceId);
            if (deviceId == currentDeviceId_) {
                // 视为本次调用出现异常，执行清理
                currentSessionId_ = -1;
                currentDeviceId_.clear();
            }
            break;
        }
        case MSG_DISTRIBUTED_SERVICE_START:
        {
            // 应用服务跨设备共享框架服务侧启动，  例如Demo程序先启动，超级工作台应用后启动
            // 收到此消息，需清除之前缓存的设备列表信息，并重新获取
            std::cout << "Distributed Service Start" << endl;
            LOGGER_WARN("Distributed Service Start");
            deviceMap.clear();
            currentSessionId_ = -1;
            currentDeviceId_.clear();
            GetDeviceList();
            QueryAllDeviceIntentActivities();
            break;
        }
        case MSG_DISTRIBUTED_DEVICE_UPDATE:
        {
            // 设备信息更新，例如设备名称变更
            DistributedBundle* bundle = msg->bundle;
            std::string deviceId = GetBundleString(bundle, CONSTANT_DEVICE_ID);
            std::string deviceName = GetBundleString(bundle, CONSTANT_DEVICE_NAME);
            std::string deviceType = GetBundleString(bundle, CONSTANT_DEVICE_TYPE);
            std::cout << "Device Update"
                << ", " << CONSTANT_DEVICE_ID << " : " << deviceId
                << ", " << CONSTANT_DEVICE_NAME << " : " << deviceName
                << ", " << CONSTANT_DEVICE_TYPE << " : " << deviceType;

            HonorDeviceInfo deviceInfo;
            deviceMap[deviceId].deviceName = deviceName;
            deviceMap[deviceId].deviceType = deviceType.c_str()[0];
            break;
        }
        default:
            break;
    }
    return 0;
}

std::string GetQuotedString(std::string str)
{
    std::ostringstream oss;
    oss << std::quoted(str);
    return oss.str();
}

void TestLog(enum::DistributedLogLevel level, const char* tag, const char* format, ...)
{
    char buf[LOG_LEN + 1] = { 0 };
    int pos = sprintf_s(buf, LOG_LEN, "[SDKLOG] %s ", tag);

    va_list ap;
    va_start(ap, format);
    pos = _vsnprintf_s(buf + pos, LOG_LEN - pos, _TRUNCATE, format, ap);
    va_end(ap);
    LOGGER_WARN(buf);
}

int main()
{
    std::cout << PROMPT << endl;
    int retcode;
    auto distributedDemo = HonorDistributedDemo::GetInstance();
    // 向Kit注册log回调函数
    Honor_Distributed_LogRegister(TestLog);
    // 初始化应用服务跨设备共享框架环境
    retcode = Honor_Distributed_Initialize(APPLICATION_ID, PROCESS_NAME.c_str(), PROCESS_NAME.length(), &MsgHandle);
    std::cout << "Initialize ret :" << retcode << endl;
    // 获取Kit版本
    const char *version = Honor_Distrubuted_Version();
    std::cout << "Distributed version :" << version << endl;

    // 获取设备列表
    distributedDemo->GetDeviceList();
    // 获取设备支持跨设备调用的action
    distributedDemo->QueryAllDeviceIntentActivities();

    int Oper = 0;
    std::string userInput;
    // 读取用户指令输入
    while (std::getline(std::cin, userInput)) {
        if (userInput.empty()) {
            continue;
        }

        std::vector<std::string> inputParam;
        std::stringstream ss(userInput);
        std::string item;
        while (std::getline(ss, item, ',')) {
            inputParam.push_back(item);
        }
        if (inputParam.size() < 1) {
            std::cerr << "The input command size is less than 1" << INPUTERR_PROMPT;
            continue;
        }
        std::transform(userInput.begin(), userInput.end(), userInput.begin(), [](unsigned char c) { return std::tolower(c); });
        if (inputParam[0] == COMMAND_VIEW_DEVICE_LIST) {
            distributedDemo->ShowDeviceListView();
        } else if (inputParam[0] == COMMAND_STOP) {
            distributedDemo->StopActivity();
        } else if (inputParam[0] == COMMAND_START) {
            if (inputParam.size() < 3) {
                std::cerr << "The start command parameter is less than 3, " << INPUTERR_PROMPT;
                continue;
            }
            try {
                int deviceIndex = atoi(inputParam[1].c_str());
                int actionIndex = atoi(inputParam[2].c_str());
                distributedDemo->StartAction(deviceIndex, actionIndex);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "invalid_argument, " << INPUTERR_PROMPT;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "out_of_range, " << INPUTERR_PROMPT;
            }
        } else {
            std::cout << INPUTERR_PROMPT;
        }
    }
    // 应用退出前，去注册框架服务
    Honor_Distributed_Release();
}