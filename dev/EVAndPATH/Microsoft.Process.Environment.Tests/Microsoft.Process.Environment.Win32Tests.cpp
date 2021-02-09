#include "pch.h"
#include "Microsoft.Process.Environment.Win32Tests.h"

bool EnvironmentManagerWin32Tests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

void EnvironmentManagerWin32Tests::TestGetForProcess()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerWin32Tests::TestGetForUser()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
    VERIFY_IS_NOT_NULL(environmentManager);
}


void EnvironmentManagerWin32Tests::TestGetForMachine()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerWin32Tests::TestGetEnvironmentVariablesForProcess()
{
    LPWSTR environmentVariables = GetEnvironmentStrings();
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerWin32Tests::TestGetEnvironmentVariablesForUser()
{

    EnvironmentManager environmentmanager = EnvironmentManager::GetForUser();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForUser();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerWin32Tests::TestGetEnvironmentVariablesForMachine()
{
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForMachine();

    EnvironmentManager environmentmanager = EnvironmentManager::GetForMachine();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}
