#include <pch.h>
#include <EnvironmentVariableChangeTracker.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    EnvironmentVariableChangeTracker::EnvironmentVariableChangeTracker(std::wstring const& key, std::wstring const& valueToSet, EnvironmentManager::Scope scope)
    {

        /*
        * Because we added the UTC time we don't need to probe for the EV
        * at insertion time.  Just keep track of the old variable when writing.
        *
        * At uninstall time is when we need to check UTC time to restore the
        * last-written value.
        *
        * However, we do need to worry about updating.
        *
        * However, we can't just update the UTC time.  What if another app
        * changed the value before we updated it?
        *
        * App A adds Hello:YOLO
        * App B changes Hello:YOLO2
        * App A changes Hello:YOLO3
        *
        * If App A is uninstalled Hello goes back to YOLO2.
        * If, instead, App B is uninstalled, the value stays at YOLO3.
        *
        * Hmm.
        * App A Adds Hello:YOLO
        * App A Changes Hello:YOLO2
        *
        * App A is uninstalled.  Hello is removed.
        *
        * I do believe, if an app is upadating a value it has written then we
        * just update the UTC time.  Keep the original value so we can go backwards
        * when app A in uninstalled
        *
        * So, if the key exists already update
        * 1. UTC time
        * 2. Current value
        *
        * For Process it would be
        * HKCU\Software\ChangeTracker\EnvironmentVariable\PFN\[EVKey]\Process
        *
        * For User it would be
        * HKCU\Software\ChangeTracker\EnvironmentVariable\PFN\[EVKey]\User
        */
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
        MessageBoxEx(NULL, L"In track change", L"In track change", 0, 0);
        if (m_ShouldTrackChange)
        {
            // It is possible that the same package in the same scope
            // will update an EV.  If this is the case we don't need to store
            // the previous value since we already did that.
            // All we need to do is record the new value and a new insertion time.
            bool isCreatingEVForPackage = false;
            wil::unique_hkey regLocationToWriteChange = GetKeyForTrackingChange(isCreatingEVForPackage);

            if (isCreatingEVForPackage)
            {
                std::wstring originalValue = GetOriginalValueOfEV();
                RETURN_IF_FAILED(HRESULT_FROM_WIN32(RegSetValueEx(regLocationToWriteChange.get()
                    , L"PreviousValue"
                    , 0
                    , REG_SZ
                    , reinterpret_cast<const BYTE*>(originalValue.c_str()), (originalValue.size() + 1) * sizeof(wchar_t))));

            }

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
