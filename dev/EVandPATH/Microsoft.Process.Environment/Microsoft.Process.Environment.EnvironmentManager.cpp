#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"

namespace winrt::Microsoft::Process::Environment::implementation
{
    EnvironmentManager::EnvironmentManager(Scope scope) : m_Scope(scope) {}

    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForProcess()
    {
        return winrt::make<EnvironmentManager>(Scope::Process);
    }

    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForCurrentUser()
    {
        return winrt::make<implementation::EnvironmentManager>(Scope::User);
    }

    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForMachine()
    {
        return winrt::make<implementation::EnvironmentManager>(Scope::Machine);
    }

    hstring EnvironmentManager::GetEnvironmentVariable(hstring const& name)
    {
        if (m_Scope == Scope::Process)
        {
            // Get number of characters needed
            DWORD numberOfCharacters = ::GetEnvironmentVariableW(name.c_str(), nullptr, 0);

            // An error happened
            if (numberOfCharacters == 0)
            {
                DWORD error = GetLastError();

                if (error == ERROR_ENVVAR_NOT_FOUND)
                {
                    return winrt::hstring{};
                }
                else
                {
                    THROW_IF_WIN32_ERROR(error);
                }
            }

            WCHAR* environmentVariableValue = new WCHAR[numberOfCharacters];

            numberOfCharacters = ::GetEnvironmentVariableW(name.c_str(), environmentVariableValue, numberOfCharacters);

            // An error occurred
            if (numberOfCharacters == 0)
            {
                DWORD error = GetLastError();
                THROW_IF_WIN32_ERROR(error);
            }

            // At this point, we have a valid environment variable value.
            return winrt::hstring(environmentVariableValue);

        }
        else if (m_Scope == Scope::User)
        {
            WCHAR* environmentVariableValue = new WCHAR[MAX_PATH];
            THROW_IF_WIN32_ERROR(RegGetValue(HKEY_CURRENT_USER, L"Environment", name.c_str(), RRF_RT_ANY, nullptr, environmentVariableValue, nullptr));

            return winrt::hstring(environmentVariableValue);
        }
        else if(m_Scope == Scope::Machine)
        {
            WCHAR* environmentVariableValue = new WCHAR[MAX_PATH];
            THROW_IF_WIN32_ERROR(RegGetValue(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", name.c_str(), RRF_RT_ANY, nullptr, environmentVariableValue, nullptr));

            return winrt::hstring(environmentVariableValue);
        }

        return winrt::hstring{};
    }

    Windows::Foundation::Collections::IMapView<hstring, hstring> EnvironmentManager::GetEnvironmentVariables()
    {
        throw hresult_not_implemented();
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

    void EnvironmentManager::AppendToPathExt(hstring const& path)
    {
        throw hresult_not_implemented();
    }

    void EnvironmentManager::RemoveFromPathExt(hstring const& path)
    {
        throw hresult_not_implemented();
    }
}
