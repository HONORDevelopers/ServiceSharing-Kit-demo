# ServiceSharing Kit Sample Code (Windows)
[![Apache-2.0](https://img.shields.io/badge/license-Apache-blue)](http://www.apache.org/licenses/LICENSE-2.0)
[![Open Source Love](https://img.shields.io/static/v1?label=Open%20Source&message=%E2%9D%A4%EF%B8%8F&color=green)](https://developer.hihonor.com/demos/)
[![C++ Language](https://img.shields.io/badge/language-java-green.svg)](https://www.java.com/en/)

English | [中文](README_ZH.md)

## Contents

- [ServiceSharing Kit Sample Code (Android)](#servicesharing-kit-sample-code-windows)
  - [Contents](#contents)
  - [Introduction](#introduction)
  - [Environment Requirements](#environment-requirements)
  - [Hardware Requirements](#hardware-requirements)
  - [Preparations](#preparations)
  - [Installation](#installation)
  - [Technical Support](#technical-support)
  - [License](#license)

## Introduction

In this sample code, you will use the created demo project to call APIs of ServiceSharing Kit. Through the demo project, you will:
1.	Learn how to configure when adapting ServiceSharing Kit.	
2.	Learn how to use ServiceSharing Kit to send and receive data.

For more information, please refer to
[Service Introduction](https://developer.hihonor.com/cn/kitdoc?category=MagicRing&kitId=11031&navigation=guides&docId=introduction.md&token=).

## Environment Requirements

Visual Studio 2017 (v141) or later are recommended.

## Hardware Requirements

A Honor laptop that supports Honor Magic Ring.
A Honor phone that supports Honor Magic Ring.

For more information, please refer to 
[MagicRing]([Enjoy Smart Life with HONOR Connect](https://www.honor.com/za/support/content/en-us15853065/)) 

## Preparations
1.	Register as a Honor developer.
2.	Open the demo code project: app/DistributedDemo.vcxproj.
3.	Build the project.


## Installation
* Copy all files in the bin directory to the laptop.
* Run Configure.bat with administrator privileges.
* Run DistributedDemo.exe (Since the ServiceSharing service  will verify the AppId and signature of the program, only the DistributedDemo.exe in the bin directory can be registered successfully. The running program you manually compiled cannot be registered successfully. We will solve this problem in the subsequent version update).

## Technical Support

If you have any questions about the sample code, try the following:
- Visit [Stack Overflow](https://stackoverflow.com/questions/tagged/honor-developer-services?tab=Votes), submit your questions, and tag them with `honor-developer-services`. Honor experts will answer your questions.
- Visit the HONOR Developer section in the [Honor Developer Forum](https://developer.hihonor.com/cn/forum/?navation=dh11614886576872095748%2F1) and communicate with other developers.

If you encounter any issues when using the sample code, submit your [issues](https://github.com/HONORDevelopers/ServiceSharing-demo/issues) or submit a [pull request](https://github.com/HONORDevelopers/ServiceSharing-demo/pulls).

## License
The sample code is licensed under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).