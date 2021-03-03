#include <pch.h>
#include <EnvironmentManagerCentennialTests.h>

namespace ProjectReunionCppTest
{
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
        WriteProcessEV();

        EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
        winrt::hstring environmentValue = environmentManager.GetEnvironmentVariable(EV_KEY_NAME);

        RemoveProcessEV();

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }

    void EnvironmentManagerCentennialTests::CentennialTestGetEnvironmentVariableForUser()
    {
        WriteUserEV();
        
        EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
        winrt::hstring environmentValue = environmentManager.GetEnvironmentVariable(EV_KEY_NAME);

        RemoveUserEV();

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
    }


    void EnvironmentManagerCentennialTests::CentennialTestSetEnvironmentVariableForProcess()
    {
        EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
        VERIFY_NO_THROW(environmentManager.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME));

        // Get the size of the buffer.
        DWORD sizeNeededInCharacters = ::GetEnvironmentVariable(EV_KEY_NAME, nullptr, 0);

        // If we got an error
        if (sizeNeededInCharacters == 0)
        {
            DWORD lastError = GetLastError();

            std::wstring errorMessage(L"Error getting Size of environment variable from process scope after setting it with Environment Manager.  The error code is: ");
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

        RemoveProcessEV();
        RemoveUserChangeTrackerRegEdits(true);
        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentVariableValue);
    }

    void EnvironmentManagerCentennialTests::CentennialTestSetEnvironmentVariableForUser()
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

        RemoveUserEV();
        RemoveUserChangeTrackerRegEdits(true);

        VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), std::wstring(environmentValue));

    }

    //void EnvironmentManagerCentennialTests::CentennialTestSetEnvironmentVariableForMachine()
    //{
    //    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
    //    VERIFY_NO_THROW(environmentManager.SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME));

    //    wil::unique_hkey environmentVariablesHKey;
    //    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof()));

    //    DWORD sizeOfEnvironmentValue{};

    //    // See how big we need the buffer to be
    //    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, nullptr, &sizeOfEnvironmentValue));

    //    wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
    //    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue));

    //    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), std::wstring(environmentValue));

    //}
}
