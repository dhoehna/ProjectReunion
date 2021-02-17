#include <pch.h>
#include <EnvironmentVariableChangeTracker.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    EnvironmentVariableChangeTracker::EnvironmentVariableChangeTracker(std::wstring const& key, std::wstring const& valueToSet, int16_t scope)
    {
        if (key.empty())
        {
            THROW_HR(E_INVALIDARG);
        }

        EnvironmentManager::Scope scopeOfChange = static_cast<EnvironmentManager::Scope>(scope);

        m_Scope = scopeOfChange;
        m_Key = key;
        m_Value = valueToSet;
    }

    HRESULT EnvironmentVariableChangeTracker::TrackChange(std::function<HRESULT(void)> callback)
    {
        wil::unique_hkey keyToTrackChanges;
        if (m_Scope == EnvironmentManager::Scope::Process || m_Scope == EnvironmentManager::Scope::User)
        {
            RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CURRENT_USER
            , L"Software\\ChangeTracker\\EnvironmentVariables"
            , 0
            , nullptr
            , REG_OPTION_NON_VOLATILE
            , KEY_WRITE
            , nullptr
            , keyToTrackChanges.addressof()
            , nullptr)));
        }
        else //Machine level scope
        {
            RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_LOCAL_MACHINE
                , L"Software\\ChangeTracker\\EnvironmentVariables"
                , 0
                , nullptr
                , REG_OPTION_NON_VOLATILE
                , KEY_WRITE
                , nullptr
                , keyToTrackChanges.addressof()
                , nullptr)));
        }

        return S_OK;
    }
}
