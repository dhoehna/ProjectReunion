#include "pch.h"
#include "Microsoft.Process.Environment.UWPTests.h"

void EnvironmentManagerUWPTests::UWPTestGetForProcess()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerUWPTests::UWPTestGetForUser()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
    VERIFY_IS_NOT_NULL(environmentManager);
}


void EnvironmentManagerUWPTests::UWPTestGetForMachine()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerUWPTests::UWPTestGetEnvironmentVariablesForProcess()
{
    LPWSTR environmentVariables = GetEnvironmentStrings();
    EnvironmentVariables environmentVariablesFromWinRTAPI = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    EnvironmentVariables environmentVariablesFromWindowsAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}
