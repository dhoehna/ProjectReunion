#include "pch.h"
#include "Microsoft.Process.Environment.Win32Tests.h"

bool EnvironmentManagerWin32Tests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

//void EnvironmentManagerWin32Tests::TestGetForProcess()
//{
//    EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
//    VERIFY_IS_NOT_NULL(environmentManager);
//}
//
//void EnvironmentManagerWin32Tests::TestGetForUser()
//{
//    EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
//    VERIFY_IS_NOT_NULL(environmentManager);
//}
//
//
//void EnvironmentManagerWin32Tests::TestGetForMachine()
//{
//    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
//    VERIFY_IS_NOT_NULL(environmentManager);
//}
//
//void EnvironmentManagerWin32Tests::TestGetEnvironmentVariablesForProcess()
//{
//    LPWSTR environmentVariables = GetEnvironmentStrings();
//    EnvironmentVariables environmentVariablesFromWinRTAPI = GetAndFormatEnvironmentVariables(environmentVariables);
//
//    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
//    EnvironmentVariables environmentVariablesFromWindowsAPI = environmentmanager.GetEnvironmentVariables();
//
//    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);
//    
//}

void EnvironmentManagerWin32Tests::TestGetEnvironmentVariablesForUser()
{
    StringMap environmentVariablesFromWindowsAPI;

    wil::unique_hkey environmentVariablesHKey;
    THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CURRENT_USER, L"Environment", 0, KEY_READ, environmentVariablesHKey.addressof())));

    DWORD sizeOfLongestNameInCharacters;
    DWORD sizeOfLongestValueInCharacters;
    DWORD numberOfValues;

    THROW_IF_FAILED(RegQueryInfoKeyW(
        environmentVariablesHKey.get()
        , nullptr // lpClass
        , nullptr // lpcchClass
        , nullptr // lpReserved
        , nullptr // lpcSubKeys
        , nullptr // lpcbMaxSubKeyLen
        , nullptr // lpcbMaxCLassLen
        , &numberOfValues //lpcValues
        , &sizeOfLongestNameInCharacters
        , &sizeOfLongestValueInCharacters
        , nullptr // lpcmSecurityDescriptor
        , nullptr)); // lpftLastWriteTime

    // +1 for null character
    const DWORD NAME_LENGTH = sizeOfLongestNameInCharacters + 1;
    const DWORD VALUE_SIZE_IN_BYTES = sizeOfLongestValueInCharacters * sizeof(WCHAR);

    std::unique_ptr<wchar_t[]> environmentVariableName(new wchar_t[NAME_LENGTH]);
    std::unique_ptr<BYTE[]> environmentVariableValue(new BYTE[VALUE_SIZE_IN_BYTES]);

    for (int valueIndex = 0; valueIndex < numberOfValues; valueIndex++)
    {
        DWORD nameLength = NAME_LENGTH;
        DWORD valueSize = VALUE_SIZE_IN_BYTES;
        LSTATUS enumerationStatus = RegEnumValueW(environmentVariablesHKey.get()
            , valueIndex
            , environmentVariableName.get()
            , &nameLength
            , nullptr
            , nullptr
            , environmentVariableValue.get()
            , &valueSize);

        // An empty name indicates the default value.
        if (nameLength == 0)
        {
            continue;
        }

        // If there was an error getting the value
        if (enumerationStatus != ERROR_SUCCESS && enumerationStatus != ERROR_NO_MORE_ITEMS)
        {
            THROW_HR(HRESULT_FROM_WIN32(enumerationStatus));
        }

        environmentVariablesFromWindowsAPI.Insert(environmentVariableName.get(), reinterpret_cast<LPWSTR>(environmentVariableValue.get()));

        environmentVariableName.reset(new wchar_t[NAME_LENGTH]);
        environmentVariableValue.reset(new BYTE[VALUE_SIZE_IN_BYTES]);
    }

    EnvironmentManager environmentmanager = EnvironmentManager::GetForUser();
    EnvironmentVariables environmentVariablesFromWinRTAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI.GetView());

}

