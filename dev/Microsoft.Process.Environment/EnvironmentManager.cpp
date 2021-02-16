#include <pch.h>
#undef GetEnvironmentVariableW
#include <EnvironmentManager.h>
#include <EnvironmentManager.g.cpp>

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::ProjectReunion::implementation
{
    EnvironmentManager::EnvironmentManager(Scope const& scope)
        : m_Scope(scope) { }

    ProjectReunion::EnvironmentManager EnvironmentManager::GetForProcess()
    {
        ProjectReunion::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Process);
        return environmentManager;
    }

    Microsoft::ProjectReunion::EnvironmentManager EnvironmentManager::GetForUser()
    {
        ProjectReunion::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::User);
        return environmentManager;
    }

    Microsoft::ProjectReunion::EnvironmentManager EnvironmentManager::GetForMachine()
    {
        ProjectReunion::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Machine);
        return environmentManager;
    }

    Windows::Foundation::Collections::IMapView<hstring, hstring> EnvironmentManager::GetEnvironmentVariables()
    {
        Windows::Foundation::Collections::StringMap environmentVariables;

        if (m_Scope == Scope::Process)
        {
            environmentVariables = GetProcessEnvironmentVariables();
        }
        else
        {
            environmentVariables = GetUserOrMachineEnvironmentVariables();
        }

        return environmentVariables.GetView();
    }

    hstring EnvironmentManager::GetEnvironmentVariable(hstring const& variableName)
    {
        if (variableName.empty())
        {
            THROW_HR(E_INVALIDARG);
        }

        if (m_Scope == Scope::Process)
        {
            // Get the size of the buffer.
            DWORD sizeNeededInCharacters = ::GetEnvironmentVariable(variableName.c_str(), nullptr, 0);

            // If we got an error
            if (sizeNeededInCharacters == 0)
            {
                DWORD lastError = GetLastError();

                if (lastError == ERROR_ENVVAR_NOT_FOUND)
                {
                    return L"";
                }
                else
                {
                    THROW_HR(HRESULT_FROM_WIN32(lastError));
                }
            }

            std::wstring environmentVariableValue;

            // Remove the trailing \0 because this will go into an hstring.
            environmentVariableValue.resize(sizeNeededInCharacters - 1);
            DWORD getResult = ::GetEnvironmentVariable(variableName.c_str(), &environmentVariableValue[0], sizeNeededInCharacters);

            if (getResult == 0)
            {
                THROW_HR(HRESULT_FROM_WIN32(GetLastError()));
            }

            return hstring(environmentVariableValue);
        }
        else
        {
            wil::unique_hkey environmentVariableHKey = GetRegHKeyForEVUserAndMachineScope();

            DWORD sizeOfEnvironmentValue;

            // See how big we need the buffer to be
            LSTATUS queryResult = RegQueryValueEx(environmentVariableHKey.get(), variableName.c_str(), 0, nullptr, nullptr, &sizeOfEnvironmentValue);

            if (queryResult != ERROR_SUCCESS)
            {
                if (queryResult == ERROR_FILE_NOT_FOUND)
                {
                    return L"";
                }

                THROW_HR(HRESULT_FROM_WIN32((queryResult)));
            }


            wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
            THROW_IF_FAILED(HRESULT_FROM_WIN32((RegQueryValueEx(environmentVariableHKey.get(), variableName.c_str(), 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue))));

            return hstring(environmentValue);
        }
    }


    void EnvironmentManager::SetEnvironmentVariable(hstring const& name, hstring const& value)
    {
        throw hresult_not_implemented();
    }

    void EnvironmentManager::AppendToPath(hstring const& path)
    {
        throw hresult_not_implemented();
    }

    void EnvironmentManager::RemoveFromPath(hstring const& path)
    {
        throw hresult_not_implemented();
    }

    void EnvironmentManager::AddExecutableFileExtension(hstring const& pathExt)
    {
        throw hresult_not_implemented();
    }

    void EnvironmentManager::RemoveExecutableFileExtension(hstring const& pathExt)
    {
        throw hresult_not_implemented();
    }

    StringMap EnvironmentManager::GetProcessEnvironmentVariables()
    {
        //Get the pointer to the process block
        LPWSTR environmentVariablesString = GetEnvironmentStrings();
        THROW_HR_IF_NULL(E_POINTER, environmentVariablesString);

        // parse the block into a map
        LPTSTR variable;
        variable = (LPTSTR)environmentVariablesString;

        StringMap environmentVariables;
        while (*variable)
        {
            std::wstring environmentVariable(variable);
            size_t locationOfNull = environmentVariable.find_last_of(L'=');

            environmentVariables.Insert(environmentVariable.substr(0, locationOfNull), environmentVariable.substr(locationOfNull + 1));

            variable += lstrlen(variable) + 1;
        }

        THROW_IF_WIN32_BOOL_FALSE(FreeEnvironmentStrings(environmentVariablesString));

        return environmentVariables;
    }

    /// <summary>
    /// Uses the stores m_Scope to figure out wheather to get the environment
    /// variables for the user, or for the machine.
    /// </summary>
    /// <returns></returns>
    StringMap EnvironmentManager::GetUserOrMachineEnvironmentVariables()
    {
        StringMap environmentVariables;
        wil::unique_hkey environmentVariablesHKey = GetRegHKeyForEVUserAndMachineScope();

        // While this way of calculating the max size of the names,
        // values, and total number of entries includes two calls
        // to the registry, I believe this is superior to
        // using a do/while or a while with a prime
        // because there is no chance of the loop iterating more than
        // is needed AND the size of the name and value arrays are
        // only as big as the bigges name or value.

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

            environmentVariables.Insert(environmentVariableName.get(), reinterpret_cast<LPWSTR>(environmentVariableValue.get()));

            environmentVariableName.reset(new wchar_t[NAME_LENGTH]);
            environmentVariableValue.reset(new BYTE[VALUE_SIZE_IN_BYTES]);
        }

        return environmentVariables;
    }

    wil::unique_hkey EnvironmentManager::GetRegHKeyForEVUserAndMachineScope(bool needsWriteAccess)
    {
        assert(m_Scope != Scope::Process);
        REGSAM registrySecurity = KEY_READ;

        if (needsWriteAccess)
        {
            registrySecurity &= KEY_SET_VALUE;
        }

        wil::unique_hkey environmentVariablesHKey;
        if (m_Scope == Scope::User)
        {
            THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION.c_str(), 0, KEY_READ, environmentVariablesHKey.addressof())));
        }
        else //Scope is Machine
        {
            THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION.c_str(), 0, KEY_READ, environmentVariablesHKey.addressof())));
        }

        return environmentVariablesHKey;
    }
}
