﻿#include "pch.h"

//#undef GetEnvironmentVariableW
#include <winrt/Microsoft.Process.Environment.h>

class EnvironmentTests
{
    TEST_CLASS(EnvironmentTests);

    TEST_METHOD(GetEvForProcess);
};

void EnvironmentTests::GetEvForProcess()
{
    const std::wstring EV_NAME = L"MyVariable";
    SetEnvironmentVariable(EV_NAME.c_str(), L"IAmCool");


    MessageBoxEx(NULL, L"In there", L"In there", 0, 0);

    
    auto yolo = winrt::Microsoft::Process::Environment::EnvironmentManager::GetForProcess();
    yolo.GetEnvironmentVariable(EV_NAME);

    SetEnvironmentVariable(L"MyVariable", L"");
    //VERIFY_ARE_EQUAL(EV_NAME.c_str(), std::wstring(environmentVariableValue).c_str());
}