#include "pch.h"
#include "Microsoft.Process.Environment.CentennialTests.h"

bool EnvironmentManagerCentennialTests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

bool EnvironmentManagerCentennialTests::CentennialTestSetup()
{
    WriteUserEV();
    return true;
}

bool EnvironmentManagerCentennialTests::CentennialTestCleanup()
{
    RemoveUserEV();
    return true;
}

void EnvironmentManagerCentennialTests::CentennialTestGetForProcess()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerCentennialTests::CentennialTestGetForUser()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
    VERIFY_IS_NOT_NULL(environmentManager);
}


void EnvironmentManagerCentennialTests::CentennialTestGetForMachine()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariablesForProcess()
{
    LPWSTR environmentVariables = GetEnvironmentStrings();
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariablesForUser()
{

    EnvironmentManager environmentmanager = EnvironmentManager::GetForUser();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForUser();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariablesForMachine()
{
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForMachine();

    EnvironmentManager environmentmanager = EnvironmentManager::GetForMachine();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariableForProcess()
{
    SetEnvironmentVariable(L"Hello", L"YOLO");

    EnvironmentManager forProcess = EnvironmentManager::GetForProcess();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariableForUser()
{
    EnvironmentManager forProcess = EnvironmentManager::GetForUser();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}
