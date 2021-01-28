#include "pch.h"

using namespace winrt::Microsoft::Process::Environment;

class EnvironmentTests
{
    TEST_CLASS(EnvironmentTests);

    TEST_METHOD(GetEvForProcess);
};

void EnvironmentTests::GetEvForProcess()
{
    const std::wstring EV_NAME = L"MyVariable";
    SetEnvironmentVariable(EV_NAME.c_str(), L"IAmCool");

    //MessageBoxEx(NULL, L"In there", L"In here", 0, 0);
    auto yolo = EnvironmentManager::GetForProcess();
    yolo.GetEnvironmentVariable(EV_NAME);

    SetEnvironmentVariable(L"MyVariable", L"");
    //VERIFY_ARE_EQUAL(EV_NAME.c_str(), std::wstring(environmentVariableValue).c_str());
}
