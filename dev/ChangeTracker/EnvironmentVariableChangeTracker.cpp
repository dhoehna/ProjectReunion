﻿#include <pch.h>
#include <EnvironmentVariableChangeTracker.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    EnvironmentVariableChangeTracker::EnvironmentVariableChangeTracker(std::wstring const& key, std::wstring const& valueToSet, EnvironmentManager::Scope scope)
    {        
        if (key.empty())
        {
            THROW_HR(E_INVALIDARG);
        }

        // Check if we need to track the changes.
        // If we do need to track the changes get the Package Full Name
        UINT32 sizeOfBuffer{};
        long fullNameResult = ::GetCurrentPackageFullName(&sizeOfBuffer, nullptr);

        if (fullNameResult == APPMODEL_ERROR_NO_PACKAGE)
        {
            m_ShouldTrackChange = false;
        }
        else if (fullNameResult == ERROR_INSUFFICIENT_BUFFER)
        {
            std::unique_ptr<WCHAR> packageFullName(new WCHAR[sizeOfBuffer]);

            LONG getNameResult = ::GetCurrentPackageFullName(&sizeOfBuffer, packageFullName.get());
            THROW_IF_FAILED(HRESULT_FROM_WIN32(getNameResult));
            m_PackageFullName = std::wstring(packageFullName.get());
            m_ShouldTrackChange = true;
        }
        else
        {
            THROW_HR(HRESULT_FROM_WIN32(fullNameResult));
        }

        m_Scope = scope;
        m_Key = key;
        m_Value = valueToSet;
    }

    HRESULT EnvironmentVariableChangeTracker::TrackChange(std::function<HRESULT(void)> callback)
    {
        if (m_ShouldTrackChange)
        {
            wil::unique_hkey regLocationToWriteChange = GetKeyForTrackingChange();

            std::wstring originalValue = GetOriginalValueOfEV();
            RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegSetValueEx(regLocationToWriteChange.get()
                , L"PreviousValue"
                , 0
                , REG_SZ
                , reinterpret_cast<const BYTE*>(originalValue.c_str()), (originalValue.size() + 1) * sizeof(wchar_t))));

            RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegSetValueEx(regLocationToWriteChange.get()
                , L"CurrentValue"
                , 0
                , REG_SZ
                , reinterpret_cast<const BYTE*>(m_Value.c_str()), (m_Value.size() + 1) * sizeof(wchar_t))));

            std::chrono::nanoseconds insertionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            long long nanoSecondTicks = insertionTime.count();
            RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegSetValueEx(regLocationToWriteChange.get()
                , L"InsertionTime"
                , 0
                , REG_QWORD
                , reinterpret_cast<const BYTE*>(&nanoSecondTicks), sizeof(long long))));
        }

        return callback();
    }
}
