# 荣耀ServiceSharing Kit接入示例代码

[![Apache-2.0](https://img.shields.io/badge/license-Apache-blue)](http://www.apache.org/licenses/LICENSE-2.0)
[![Open Source Love](https://img.shields.io/static/v1?label=Open%20Source&message=%E2%9D%A4%EF%B8%8F&color=green)](https://developer.hihonor.com/demos/)
[![C++ Language](https://img.shields.io/badge/language-c++-blue.svg)](https://isocpp.org/)
[English](README.md) | 中文

## 目录

- [荣耀ServiceSharing Kit接入示例代码](#荣耀ServiceSharing-kit接入示例代码)
  - [目录](#目录)
  - [简介](#简介)
  - [环境要求](#环境要求)
  - [硬件要求](#硬件要求)
  - [开发准备](#开发准备)
  - [安装](#安装)
  - [技术支持](#技术支持)
  - [授权许可](#授权许可)

## 简介

本示例代码中，你将使用已创建的代码工程来调用荣耀应用服务跨设备共享服务（Honor ServiceSharing Kit）的接口。通过该工程，你将：
1.	了解适配ServiceSharing Kit具体如何配置。
2.	了解适配ServiceSharing Kit之后如何进行数据的收发。

更多内容，可以登陆[荣耀开发者服务平台](https://developer.hihonor.com/cn/)，点击**产品 - 开放能力 - 应用服务跨设备共享**进行了解

## 环境要求

推荐使用Visual Studio 2017 (v141)及以上

## 硬件要求

支持荣耀信任环能力的荣耀笔记本电脑

支持荣耀信任环能力的荣耀手机或荣耀平板一台

更多信息，请参阅
[MagicRing]([Enjoy Smart Life with HONOR Connect](https://www.honor.com/za/support/content/en-us15853065/)) 

## 开发准备

1.	注册荣耀帐号，成为荣耀开发者。
2.	打开示例代码工程： app/DistributedDemo.vcxproj。
3.	构建本示例代码


## 安装
* 将bin目录下所有文件拷贝至笔记本电脑
* 以管理员权限运行Configure.bat
* 运行DistributedDemo.exe （由于应用服务跨设备共享服务会校验程序的AppId和签名，所以只有bin目录下的DistributedDemo.exe 可以注册成功，您手动编译出的运行程序无法注册成功，这个问题我们会在后续版本更新解决）


## 技术支持

如果您对该示例代码还处于评估阶段，可在[荣耀开发者社区](https://developer.hihonor.com/cn/forum/?navation=dh11614886576872095748%2F1)获取关于ServiceSharing Kit的最新讯息，并与其他开发者交流见解。

如果您对使用该示例代码有疑问，请尝试：
- 开发过程遇到问题上[Stack Overflow](https://stackoverflow.com/questions/tagged/honor-developer-services?tab=Votes)，在`honor-developer-services`标签下提问，有荣耀研发专家在线一对一解决您的问题。
- 访问[荣耀开发者论坛](https://developer.hihonor.com/cn/forum/?navation=dh11614886576872095748%2F1)中的开发者服务平台专区，与其他开发者进行交流。

如果您在尝试示例代码中遇到问题，请向仓库提交[issue](https://github.com/HONORDevelopers/ServiceSharing-demo/issues)，也欢迎您提交[Pull Request](https://github.com/HONORDevelopers/ServiceSharing-demo/pulls)。

## 授权许可

该示例代码经过[Apache 2.0授权许可](http://www.apache.org/licenses/LICENSE-2.0)。
