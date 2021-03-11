#pragma once
#include <EnvironmentManager.h>
#include "IChangeTracker.h"
#include <wil/registry.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct EnvironmentVariableChangeTracker : public IChangeTracker
    {
        EnvironmentVariableChangeTracker(std::wstring const& key, std::wstring const& valueToSet, EnvironmentManager::Scope scope);
        HRESULT TrackChange(std::function<HRESULT(void)> callBack);

    private:
        const std::wstring USER_EV_REG_LOCATION = L"Environment";
        const std::wstring MACHINE_EV_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";


        EnvironmentManager::Scope m_Scope;
        std::wstring m_Key;
        std::wstring m_Value;
        bool m_ShouldTrackChange = false;
        std::wstring m_PackageFullName;
        bool m_IsPathOrPathExt = false;

        wil::unique_hkey GetRegHKeyForEVUserAndMachineScope()
        {
            assert(m_Scope != EnvironmentManager::Scope::Process);
            REGSAM registrySecurity = KEY_READ;

            wil::unique_hkey environmentVariablesHKey;
            if (m_Scope == EnvironmentManager::Scope::User)
            {
                THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CURRENT_USER, USER_EV_REG_LOCATION.c_str(), 0, KEY_READ, environmentVariablesHKey.addressof())));
            }
            else //Scope is Machine
            {
                THROW_IF_FAILED(HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, MACHINE_EV_REG_LOCATION.c_str(), 0, KEY_READ, environmentVariablesHKey.addressof())));
            }

            return environmentVariablesHKey;
        }

        wil::unique_hkey GetKeyForTrackingChange()
        {
            std::wstringstream subKeyStream;
            subKeyStream << L"Software\\ChangeTracker\\EnvironmentVariables\\";
            subKeyStream << m_PackageFullName;
            subKeyStream << "\\";
            subKeyStream << m_Key;
            subKeyStream << "\\";

            if (m_Scope == EnvironmentManager::Scope::User)
            {
                subKeyStream << L"User";
            }
            else
            {
                subKeyStream << L"Machine";
            }

            subKeyStream << "\\";

            auto subKeyIntermediate = subKeyStream.str();
            auto subKey = subKeyIntermediate.c_str();

            wil::unique_hkey keyToTrackChanges;
            if (m_Scope == EnvironmentManager::Scope::User)
            {
                LSTATUS getResult = RegCreateKeyEx(HKEY_CURRENT_USER
                    , subKey
                    , 0
                    , nullptr
                    , REG_OPTION_NON_VOLATILE
                    , KEY_ALL_ACCESS
                    , nullptr
                    , keyToTrackChanges.put()
                    , nullptr);

                if (getResult != ERROR_SUCCESS)
                {
                    THROW_HR(HRESULT_FROM_WIN32(getResult));
                }
            }
            else //Machine level scope
            {
                THROW_IF_FAILED(HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_LOCAL_MACHINE
                    , subKeyStream.str().c_str()
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

        bool IsEVBeingCreated(HKEY keyToChangeTracker)
        {
            LSTATUS queryStatus = RegQueryValueEx(keyToChangeTracker, L"PreviousValue", 0, nullptr, nullptr, nullptr);

            if (queryStatus == ERROR_FILE_NOT_FOUND)
            {
                return true;
            }
            else if (queryStatus == ERROR_SUCCESS)
            {
                return false;
            }
            else
            {
                THROW_HR(HRESULT_FROM_WIN32(queryStatus));
            }


        }

        std::wstring GetPathOrPathExtChangesFromRegistry()
        {
            wil::unique_hkey regLocationToWriteChange = GetKeyForTrackingChange();
            DWORD sizeOfPathOrPathExtChanges;

            // See how big we need the buffer to be
            LSTATUS queryResult = RegQueryValueEx(regLocationToWriteChange.get(), L"AppendedValues", 0, nullptr, nullptr, &sizeOfPathOrPathExtChanges);

            if (queryResult == ERROR_FILE_NOT_FOUND)
            {
                return L"";
            }

            if (queryResult != ERROR_SUCCESS)
            {

                THROW_HR(HRESULT_FROM_WIN32((queryResult)));
            }


            wchar_t* pathOrPathExtChanges = new wchar_t[sizeOfPathOrPathExtChanges];
            THROW_IF_FAILED(HRESULT_FROM_WIN32((RegQueryValueEx(regLocationToWriteChange.get(), L"AppendedValues", 0, nullptr, (LPBYTE)pathOrPathExtChanges, &sizeOfPathOrPathExtChanges))));

            return std::wstring(pathOrPathExtChanges);
        }

        std::wstring GetOriginalValueOfEV()
        {
            wil::unique_hkey environmentVariableHKey = GetRegHKeyForEVUserAndMachineScope();

            DWORD sizeOfEnvironmentValue;

            // See how big we need the buffer to be
            LSTATUS queryResult = RegQueryValueEx(environmentVariableHKey.get(), m_Key.c_str(), 0, nullptr, nullptr, &sizeOfEnvironmentValue);

            if (queryResult != ERROR_SUCCESS)
            {
                if (queryResult == ERROR_FILE_NOT_FOUND)
                {
                    return L"";
                }

                THROW_HR(HRESULT_FROM_WIN32((queryResult)));
            }


            wchar_t* environmentValue = new wchar_t[sizeOfEnvironmentValue];
            THROW_IF_FAILED(HRESULT_FROM_WIN32((RegQueryValueEx(environmentVariableHKey.get(), m_Key.c_str(), 0, nullptr, (LPBYTE)environmentValue, &sizeOfEnvironmentValue))));

            return environmentValue;
        }
    };
}
