#include <pch.h>
#include <EnvironmentManagerUWPTests.h>


namespace ProjectReunionCppTest
{
    //bool EnvironmentManagerUWPTests::ClassSetup()
    //{
    //    WriteUserEV();
    //    return true;
    //}

    //bool EnvironmentManagerUWPTests::ClassCleanup()
    //{
    //    RemoveUserEV();
    //    return true;
    //}

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
        EnvironmentVariables environmentVariablesFromWindowsAPI = GetAndFormatEnvironmentVariables(environmentVariables);

        EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
        EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

        CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

    }

    void EnvironmentManagerUWPTests::UWPTestGetEnvironmentVariablesForUser()
    {
        EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForUser();

        EnvironmentManager environmentmanager = EnvironmentManager::GetForUser();
        EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

        CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

    }

    void EnvironmentManagerUWPTests::UWPTestGetEnvironmentVariablesForMachine()
    {
        EnvironmentVariables environmentVariablesFromWindowsAPI = GetEnvironmentVariablesForMachine();

        EnvironmentManager environmentmanager = EnvironmentManager::GetForMachine();
        EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

        CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);

    }

    void EnvironmentManagerUWPTests::UWPTestGetEnvironmentVariableForProcess()
    {
        WriteProcessEV();

        EnvironmentManager environmentMananger = EnvironmentManager::GetForProcess();
        winrt::hstring environmentValue = environmentMananger.GetEnvironmentVariable(EV_KEY_NAME);

        RemoveProcessEV();

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }

    //void EnvironmentManagerUWPTests::UWPTestGetEnvironmentVariableForUser()
    //{
    //    WriteProcessEV();

    //    EnvironmentManager environmentMananger = EnvironmentManager::GetForProcess();
    //    winrt::hstring environmentValue = environmentMananger.GetEnvironmentVariable(EV_KEY_NAME);

    //    RemoveProcessEV();

    //    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    //}

    void EnvironmentManagerUWPTests::UWPTestSetEnvironmentVariableForProcess()
    {

        EnvironmentManager environmentMananger = EnvironmentManager::GetForProcess();
        environmentMananger.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME);
        winrt::hstring environmentValue = environmentMananger.GetEnvironmentVariable(EV_KEY_NAME);


        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }
}
