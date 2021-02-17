#pragma once
#include <EnvironmentManager.h>
#include <IChangeTracker.h>
#include <wil/registry.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct EnvironmentVariableChangeTracker : public IChangeTracker
    {
        EnvironmentVariableChangeTracker(std::wstring const& key, std::wstring const& valueToSet, int16_t scope);
        HRESULT TrackChange(std::function<HRESULT(void)> callBack);

    private:
        EnvironmentManager::Scope m_Scope;
        std::wstring m_Key;
        std::wstring m_Value;
    };
}
