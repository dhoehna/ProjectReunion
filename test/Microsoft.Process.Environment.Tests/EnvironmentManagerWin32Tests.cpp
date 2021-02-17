#include <pch.h>
#include <EnvironmentManagerWin32Tests.h>

namespace ProjectReunionCppTest
{
    bool EnvironmentManagerWin32Tests::TestSetup()
    {
        winrt::init_apartment(winrt::apartment_type::single_threaded);
        return true;
    }

    void EnvironmentManagerWin32Tests::TestGetForProcess()
    {
        winrt::Microsoft::ProjectReunion::EnvironmentManager environmentManager = winrt::Microsoft::ProjectReunion::EnvironmentManager::GetForProcess();
        VERIFY_IS_NOT_NULL(environmentManager);
    }

    bool EnvironmentManagerWin32Tests::WriteWin32UserUV()
    {
        WriteUserEV();

        return true;
    }

    bool EnvironmentManagerWin32Tests::RemoveWin32UserUV()
    {
        RemoveUserEV();

        return true;
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

    void EnvironmentManagerWin32Tests::TestGetEnvironmentVariableForProcess()
    {
        SetEnvironmentVariable(L"Hello", L"YOLO");

        EnvironmentManager forProcess = EnvironmentManager::GetForProcess();
        winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(L"Hello");
        VERIFY_ARE_EQUAL(std::wstring(L"YOLO"), environmentValue);
    }

    void EnvironmentManagerWin32Tests::TestGetEnvironmentVariableForUser()
    {
        EnvironmentManager forProcess = EnvironmentManager::GetForUser();
        winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }
}
