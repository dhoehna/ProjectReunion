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
        WriteProcessEV();

        EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
        winrt::hstring environmentValue = environmentManager.GetEnvironmentVariable(EV_KEY_NAME);

        RemoveProcessEV();

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }

    void EnvironmentManagerWin32Tests::TestGetEnvironmentVariableForUser()
    {
        WriteUserEV();

        EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
        winrt::hstring environmentValue = environmentManager.GetEnvironmentVariable(EV_KEY_NAME);

        RemoveUserEV();

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }

    void EnvironmentManagerWin32Tests::TestSetEnvironmentVariableForProcess()
    {
        EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
        VERIFY_NO_THROW(environmentManager.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME));

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), GetEnvironmentVariableForProcess(EV_KEY_NAME));
    }

    void EnvironmentManagerWin32Tests::TestSetEnvironmentVariableForUser()
    {
        EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
        VERIFY_NO_THROW(environmentManager.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME));

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), GetEnvironmentVariableForUser(EV_KEY_NAME));

        RemoveUserEV();
        RemoveUserChangeTracking();

    }

    void EnvironmentManagerWin32Tests::TestAppendToPathForProcess()
    {
        std::wstring originalPath = GetEnvironmentVariableForProcess(PATH_NAME);
        EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
        VERIFY_NO_THROW(environmentManager.AppendToPath(EV_VALUE_NAME));

        std::wstring alteredPath = GetEnvironmentVariableForProcess(PATH_NAME);

        VERIFY_ARE_EQUAL(originalPath.append(EV_VALUE_NAME).append(L";"), alteredPath);
    }

    void EnvironmentManagerWin32Tests::TestAppendToPathForUser()
    {
        std::wstring originalPath = GetEnvironmentVariableForUser(PATH_NAME);
        EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
        VERIFY_NO_THROW(environmentManager.AppendToPath(EV_VALUE_NAME));

        std::wstring alteredPath = GetEnvironmentVariableForUser(PATH_NAME);

        RestoreUserPath(originalPath);
        RemoveUserTracking();

        VERIFY_ARE_EQUAL(originalPath.append(EV_VALUE_NAME).append(L";"), alteredPath);
    }
}
