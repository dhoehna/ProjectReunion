#pragma once
#include <string>
#include <sstream>
#include <iostream>

using namespace winrt::Microsoft::ProjectReunion; // for EnvironmentManager
using namespace winrt::Windows::Foundation::Collections; // for IMapView
typedef IMapView<winrt::hstring, winrt::hstring> EnvironmentVariables;

inline const wchar_t* USER_EV_REG_LOCATION = L"Environment";
inline const wchar_t* MACHINE_EV_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";

inline const wchar_t* EV_KEY_NAME = L"Hello";
inline const wchar_t* EV_VALUE_NAME = L"YOLO";
inline const DWORD EV_VALUE_NAME_LENGTH_WITH_NULL = 5;

inline const wchar_t* EV_VALUE_NAME2 = L"YOLO2";
inline const DWORD EV_VALUE_NAME2_LENGTH_WITH_NULL = 6;

inline const wchar_t* PATH_NAME = L"Path";

inline std::wstring GetEnvironmentVariableForProcess(std::wstring key)
{
    // Get the size of the buffer.
    DWORD sizeNeededInCharacters = ::GetEnvironmentVariable(key.c_str(), nullptr, 0);

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
    DWORD getResult = ::GetEnvironmentVariable(key.c_str(), &environmentVariableValue[0], sizeNeededInCharacters);

    if (getResult == 0)
    {
        DWORD lastError = GetLastError();

        std::wstring errorMessage(L"Error getting the environment variable from process scope after setting it with Environment Manager.  The error code is: ");
        errorMessage += std::to_wstring(lastError);
        VERIFY_FAIL(errorMessage.c_str());
    }

    return environmentVariableValue;
}

inline std::wstring GetEnvironmentVariableForUser(std::wstring key)
{
    wil::unique_hkey environmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof()));

    DWORD sizeOfEnvironmentValue{};

    // See how big we need the buffer to be
    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), key.c_str(), 0, nullptr, nullptr, &sizeOfEnvironmentValue));

    wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), key.c_str(), 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue));

    return std::wstring(environmentValue);
}

inline std::wstring GetEnvironmentVariableForMachine(std::wstring key)
{
    wil::unique_hkey environmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof()));

    DWORD sizeOfEnvironmentValue{};

    // See how big we need the buffer to be
    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, nullptr, &sizeOfEnvironmentValue));

    wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
    VERIFY_WIN32_SUCCEEDED(RegQueryValueEx(environmentVariablesHKey.get(), EV_KEY_NAME, 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue));

    return std::wstring(environmentValue);
}

inline EnvironmentVariables GetEnvironmentVariablesFromRegistry(HKEY hKey)
{
    StringMap environmentVariables;


    DWORD sizeOfLongestNameInCharacters;
    DWORD sizeOfLongestValueInCharacters;
    DWORD numberOfValues;

    THROW_IF_FAILED(RegQueryInfoKeyW(
         hKey
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
        LSTATUS enumerationStatus = RegEnumValueW(hKey
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

        environmentVariables.Insert(environmentVariableName.get(), reinterpret_cast<LPWSTR>(environmentVariableValue.get()));

        environmentVariableName.reset(new wchar_t[NAME_LENGTH]);
        environmentVariableValue.reset(new BYTE[VALUE_SIZE_IN_BYTES]);
    }

    return environmentVariables.GetView();
}

inline wil::unique_hkey GetKeyForTrackingChange(bool isUser)
{
    wil::unique_hkey keyToTrackChanges;
    if (isUser)
    {
        THROW_IF_FAILED(HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CURRENT_USER
            , L"Software\\ChangeTracker"
            , 0
            , nullptr
            , REG_OPTION_NON_VOLATILE
            , KEY_ALL_ACCESS
            , nullptr
            , keyToTrackChanges.put()
            , nullptr)));
    }
    else //Machine level scope
    {
        THROW_IF_FAILED(HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_LOCAL_MACHINE
            , L"Software"
            , 0
            , nullptr
            , REG_OPTION_NON_VOLATILE
            , KEY_WRITE
            , nullptr
            , keyToTrackChanges.put()
            , nullptr)));
    }

    return keyToTrackChanges;
}

inline void WriteUserEV()
{
    wil::unique_hkey userEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_WRITE, userEnvironmentVariablesHKey.addressof()));
    VERIFY_WIN32_SUCCEEDED(RegSetValueExW(userEnvironmentVariablesHKey.get(), EV_KEY_NAME, 0, REG_SZ, (LPBYTE)EV_VALUE_NAME, EV_VALUE_NAME_LENGTH_WITH_NULL * sizeof(wchar_t)));
}

inline void RemoveUserEV()
{
    wil::unique_hkey userEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_WRITE, userEnvironmentVariablesHKey.addressof()));
    VERIFY_WIN32_SUCCEEDED(RegDeleteValueW(userEnvironmentVariablesHKey.get(), EV_KEY_NAME));
}

inline void RemoveUserTracking()
{

}

inline std::wstring GetUserPath()
{
    wil::unique_hkey userEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_ALL_ACCESS, userEnvironmentVariablesHKey.addressof()));
    return GetEnvironmentVariableForUser(PATH_NAME);
}

inline void RestoreUserPath(std::wstring originalPath)
{
    wil::unique_hkey userEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_WRITE, userEnvironmentVariablesHKey.addressof()));
    VERIFY_WIN32_SUCCEEDED(RegSetValueEx(userEnvironmentVariablesHKey.get(), PATH_NAME, 0, REG_EXPAND_SZ, (LPBYTE)originalPath.c_str(), (originalPath.size() + 1) * sizeof(wchar_t)));
}

inline void RemoveUserChangeTracking()
{
    wil::unique_hkey hKey = GetKeyForTrackingChange(true);

    VERIFY_WIN32_SUCCEEDED(RegDeleteTree(hKey.get(), nullptr));
}

inline void WriteMachineEV()
{
    wil::unique_hkey machineEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION, 0, KEY_WRITE, machineEnvironmentVariablesHKey.addressof()));
    VERIFY_WIN32_SUCCEEDED(RegSetValueEx(machineEnvironmentVariablesHKey.get(), EV_KEY_NAME, 0, REG_SZ, (LPBYTE)EV_VALUE_NAME, EV_VALUE_NAME_LENGTH_WITH_NULL * sizeof(wchar_t)));
}

inline void RemoveMachineEV()
{
    wil::unique_hkey machineEnvironmentVariablesHKey;
    VERIFY_WIN32_SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION, 0, KEY_WRITE, machineEnvironmentVariablesHKey.addressof()));
    VERIFY_WIN32_SUCCEEDED(RegDeleteValueW(machineEnvironmentVariablesHKey.get(), EV_KEY_NAME));
}

inline void WriteProcessEV()
{
    SetEnvironmentVariable(EV_KEY_NAME, EV_VALUE_NAME);
}

inline void RemoveProcessEV()
{
    SetEnvironmentVariable(EV_KEY_NAME, nullptr);
}


inline void WriteUserAndMachineEV()
{
    WriteUserEV();
    WriteMachineEV();
}

inline void RemoveUserAndMachineEV()
{
    RemoveUserEV();
    RemoveMachineEV();
}

inline EnvironmentVariables GetEnvironmentVariablesForUser()
{
    wil::unique_hkey environmentVariablesHKey;
    THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof())));
    return GetEnvironmentVariablesFromRegistry(environmentVariablesHKey.get());
}

inline EnvironmentVariables GetEnvironmentVariablesForMachine()
{
    wil::unique_hkey environmentVariablesHKey;
    THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION, 0, KEY_READ, environmentVariablesHKey.addressof())));
    return GetEnvironmentVariablesFromRegistry(environmentVariablesHKey.get());
}

inline EnvironmentVariables GetAndFormatEnvironmentVariables(LPWSTR environmentVariableString)
{
    VERIFY_IS_NOT_NULL(environmentVariableString);

    LPTSTR variable;
    variable = (LPTSTR)environmentVariableString;

    StringMap environmentVariables;
    while (*variable)
    {
        std::wstring environmentVariable(variable);
        size_t locationOfNull = environmentVariable.find_last_of(L'=');

        environmentVariables.Insert(environmentVariable.substr(0, locationOfNull), environmentVariable.substr(locationOfNull + 1));

        variable += lstrlen(variable) + 1;
    }

    VERIFY_WIN32_BOOL_SUCCEEDED(FreeEnvironmentStrings(environmentVariableString));

    return environmentVariables.GetView();
}

/// <summary>
/// Compares two IMapView<winrt::hstring, winrt::hstring> collections for
/// 1. Have the same size
/// 2. All the keys in <paramref name="underTest" /> are in <paramref name="real" />
/// 3. All the values in <paramref name="underTest" /> are in <paramref name="real" />
/// </summary>
/// <param name="underTest">The EnvironmentVariables from the WInRT API that is being tested</param>
/// <param name="real">The Environment Variables that came from GetEnvironmentStrings</param>
/// <remarks>
/// This test will alwayse use <paramref name="underTest" /> to compare to <paramref name="real" />.  As long as the size, keys, and values
/// are compared from <paramref name="underTest" /> to <paramref name="real" /> this will fail in the case that any keys are different, or any values are different
/// </remarks>
inline void CompareIMapViews(EnvironmentVariables fromWinRT, EnvironmentVariables fromWin32)
{
    std::wstringstream sizeMessage;
    sizeMessage << L"Size of WinRT Collection: ";
    sizeMessage << fromWinRT.Size() << "\r\n";
    sizeMessage << L"Size of Win32 Collection: ";
    sizeMessage << fromWin32.Size();

    WEX::Logging::Log::Comment(sizeMessage.str().c_str());
    // Make sure the sizes are the same.
    VERIFY_ARE_EQUAL(fromWinRT.Size(), fromWin32.Size());

    if (fromWinRT.Size() == 0)
    {
        return;
    }

    auto winRTIterator = fromWinRT.First();
    do
    {
        // Make sure the same key exists in both collections
        auto winRTKey = winRTIterator.Current().Key();

        std::wstringstream logMessage;
        logMessage << L"Key from WinRT: ";
        logMessage << winRTKey.c_str();

        WEX::Logging::Log::Comment(logMessage.str().c_str());
        
        VERIFY_IS_TRUE(fromWin32.HasKey(winRTKey));

        // Make sure they have the same value
        auto winRTValue = winRTIterator.Current().Value();

        std::wstringstream valueMessage;
        valueMessage << L"Value from test: ";
        valueMessage << winRTValue.c_str() << L"\r\n";
        valueMessage << L"Value from real: " << fromWin32.Lookup(winRTKey).c_str();

        WEX::Logging::Log::Comment(valueMessage.str().c_str());
        VERIFY_ARE_EQUAL(winRTValue, fromWin32.Lookup(winRTKey));
        winRTIterator.MoveNext();
    } while (winRTIterator.HasCurrent());
}
