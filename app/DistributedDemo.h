/*
 * Copyright(c) Honor Device Co., Ltd. 2024 - 2027. All rights reserved.
 * Description: ServiceSharing Kit Demo
 * Author: 
 * Create: 2024-10-12
 */
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef HONOR_DISTRIBUTED_DEMO_H
#define HONOR_DISTRIBUTED_DEMO_H

#include <string>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <process.h>
#include <set>
#include "include/json.hpp"
#include "include/DistributedKit.h"
#include "include/DistributedErr.h"

#define CONSTANT_MSG_TYPE "msgType"
#define CONSTANT_DEVICE_NAME "deviceName"
#define CONSTANT_DEVICE_ID "deviceId"
#define CONSTANT_DEVICE_TYPE "deviceType"
#define CONSTANT_OS_TYPE "osType"
#define CONSTANT_OS_VERSION "osVersion"
#define CONSTANT_DEVICE_LIST "deviceInfoList"
#define CONSTANT_ACTION "action"
#define CONSTANT_PKG_NAME "packageName"
#define CONSTANT_COMPONENT_NAME "componentName"
#define CONSTANT_OUT_PATH "outpath"
#define CONSTANT_APP_NAME "srcAppName"
#define CONSTANT_APP_PROCESS_ID "srcAppProcessId"
#define CONSTANT_ERROR_CODE "errorCode"
#define CONSTANT_REQUEST_CODE "requestCode"
#define CONSTANT_SESSION_ID "sessionId"
#define CONSTANT_FILE_PATH "filePath"
#define CONSTANT_RESOLVELIST "resolveInfoList"
#define CONSTANT_ACTIVITYLIST "activityList"

namespace DistributedDemo {
    struct HonorDeviceInfo {
        std::string deviceName;
        std::string deviceId;
        int deviceType; // 0:Tv; 1:Phone; 2:Watch; 3:PC; 4:Pad; 5:Unkonwn
        std::string osType;
        std::string osVersion;
        std::vector<std::string> actionList;
    };

    class Logger;
    class HonorDistributedDemo {
    public:
        static HonorDistributedDemo *GetInstance();
        HonorDistributedDemo() {};
        ~HonorDistributedDemo() {};
        int MsgHandler(const DistributedMessage* msg);
        void StartAction(int deviceIndex, int actionIndex);
        void StopActivity();
        void GetDeviceList();
        void QueryIntentActivities(std::string deviceId);
        void QueryAllDeviceIntentActivities();
        std::string GetBundleString(DistributedBundle* bundle, std::string key);
        void ShowDeviceListView();
    private:
        int currentSessionId_ = -1;
        std::string currentDeviceId_ = "";
        std::map<std::string, HonorDeviceInfo> deviceMap;
    };

    class Logger {
    public:
        Logger(const std::string& filename) : logFile(filename, std::ios::app) {
            if (!logFile.is_open()) {
                std::cerr << "Can not open log file: " << filename << std::endl;
            }
        }

        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        template<typename T>
        Logger& operator<<(const T& message) {
            std::lock_guard<std::mutex> lock(mutex_); // thread safety
            logStream << message;
            return *this;
        }

        void flush(const char* functionName) {
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                auto now_c = std::chrono::system_clock::to_time_t(now);
                std::tm now_tm;
                localtime_s(&now_tm, &now_c);
                std::thread::id threadId = std::this_thread::get_id();
                logFile << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
                    << "[" << threadId << "] "
                    << logStream.str() << "(" << functionName << ")" << std::endl;
                logStream.str("");
                logStream.clear();
            }
        }

    private:
        std::ofstream logFile;
        std::ostringstream logStream;
        std::mutex mutex_;
    };

    Logger logger("log.txt");
}

#define LOGGER_WARN(msg) logger << "[WARN] - " << msg; logger.flush(__FUNCTION__);

#endif