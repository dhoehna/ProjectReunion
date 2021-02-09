#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"
#include "EnvironmentManager.g.cpp"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::Process::Environment::implementation
{

    EnvironmentManager::EnvironmentManager(Scope const& scope)
        : m_Scope(scope) { }

    Environment::EnvironmentManager EnvironmentManager::GetForProcess()
    {        
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Process);
        return environmentManager;
    }

    Environment::EnvironmentManager EnvironmentManager::GetForUser()
    {
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::User);
        return environmentManager;
    }

    Environment::EnvironmentManager EnvironmentManager::GetForMachine()
    {
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Machine);
        return environmentManager;
    }

    Windows::Foundation::Collections::IMapView<hstring, hstring> EnvironmentManager::GetEnvironmentVariables()
    {
        Windows::Foundation::Collections::StringMap environmentVariables;

        if (m_Scope == Scope::Process)
        {
            environmentVariables = GetAndFormatEnvironmentVariables();
        }
        else if (m_Scope == Scope::User)
        {
            // While this way of calculating the max size of the names,
            // values, and total number of entries includes two calls
            // to the registry, I believe this is superior to
            // using a do/while or a while with a prime
            // because there is no chance of the loop iterating more than
            // in needed AND the memory footprint is smaller.
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

                environmentVariables.Insert(environmentVariableName.get(), reinterpret_cast<LPWSTR>(environmentVariableValue.get()));

                environmentVariableName.reset(new wchar_t[NAME_LENGTH]);
                environmentVariableValue.reset(new BYTE[VALUE_SIZE_IN_BYTES]);
            }

        }
        else //Scope is machine
        {
            //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment


        }

        return environmentVariables.GetView();
    }

    StringMap EnvironmentManager::GetAndFormatEnvironmentVariables()
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
}
