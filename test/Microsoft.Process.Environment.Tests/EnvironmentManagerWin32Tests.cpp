﻿#include <pch.h>
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
        winrt::hstring environmentValue = environmentManager.GetEnvironmentVariable(L"Hello");

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

        // Get the size of the buffer.
        DWORD sizeNeededInCharacters = ::GetEnvironmentVariable(EV_KEY_NAME, nullptr, 0);

        // If we got an error
        if (sizeNeededInCharacters == 0)
        {
            DWORD lastError = GetLastError();

            std::wstring errorMessage(L"Error getting the environment variable from process scope after setting it with Environment Manager.  The error code is: ");
            errorMessage += std::to_wstring(lastError);
            VERIFY_FAIL(errorMessage.c_str());
        }

        std::wstring environmentVariableValue;

        // Remove the trailing \0 because this will go into an hstring.
        environmentVariableValue.resize(sizeNeededInCharacters - 1);
        DWORD getResult = ::GetEnvironmentVariable(EV_KEY_NAME, &environmentVariableValue[0], sizeNeededInCharacters);

        if (getResult == 0)
        {
            DWORD lastError = GetLastError();

            std::wstring errorMessage(L"Error getting the environment variable from process scope after setting it with Environment Manager.  The error code is: ");
            errorMessage += std::to_wstring(lastError);
            VERIFY_FAIL(errorMessage.c_str());
        }

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentVariableValue);
    }

    void EnvironmentManagerWin32Tests::TestSetEnvironmentVariableForUser()
    {
        EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
        VERIFY_NO_THROW(environmentManager.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME));

        wil::unique_hkey environmentVariablesHKey;
        VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof()));

        DWORD sizeOfEnvironmentValue{};

        // See how big we need the buffer to be
        VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, nullptr, &sizeOfEnvironmentValue));

        wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
        VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue));

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), std::wstring(environmentValue));

    }
}
