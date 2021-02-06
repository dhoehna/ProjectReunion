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
    EnvironmentVariables environmentVariablesFromWinRTAPI = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    EnvironmentVariables environmentVariablesFromWindowsAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

}
