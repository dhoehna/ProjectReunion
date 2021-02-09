#include "pch.h"
#include "EnvironmentManagerCentennialTests.h"

bool EnvironmentManagerCentennialTests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
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
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForUser();

    EnvironmentManager environmentmanager = EnvironmentManager::GetForUser();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}

void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariablesForMachine()
{
    EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForMachine();

    EnvironmentManager environmentmanager = EnvironmentManager::GetForMachine();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}
