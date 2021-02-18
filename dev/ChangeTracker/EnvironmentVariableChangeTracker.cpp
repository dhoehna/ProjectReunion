#include <pch.h>
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
            std::unique_ptr<PWSTR> packageFullName(new PWSTR[sizeOfBuffer]);
            THROW_HR(HRESULT_FROM_WIN32(::GetCurrentPackageFullName(&sizeOfBuffer, *packageFullName.get())));
            m_PackageFullName = *packageFullName;
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
            std::wstring originalValue = GetOriginalValueOfEV();
            wil::unique_hkey regLocationToWriteChange = GetKeyForTrackingChange();

            if (m_Scope == EnvironmentManager::Scope::Process)
            {
                LSTATUS setResult = RegSetValueEx(regLocationToWriteChange.get()
                    , L"Scope"
                    , 0
                    , REG_SZ
                    , reinterpret_cast<const BYTE*>(L"Process"), 8 * sizeof(wchar_t));

                if (setResult != ERROR_SUCCESS)
                {
                    DWORD lastError = GetLastError();
                    RETURN_HR(HRESULT_FROM_WIN32(lastError));
                }
            }
            else if (m_Scope == EnvironmentManager::Scope::User)
            {
                LSTATUS setResult = RegSetValueEx(regLocationToWriteChange.get()
                    , L"Scope"
                    , 0
                    , REG_SZ
                    , reinterpret_cast<const BYTE*>(L"User"), 5 * sizeof(wchar_t));

                if (setResult != ERROR_SUCCESS)
                {
                    DWORD lastError = GetLastError();
                    RETURN_HR(HRESULT_FROM_WIN32(lastError));
                }
            }
            else
            {
                LSTATUS setResult = RegSetValueEx(regLocationToWriteChange.get()
                    , L"Scope"
                    , 0
                    , REG_SZ
                    , reinterpret_cast<const BYTE*>(L"Machine"), 8 * sizeof(wchar_t));

                if (setResult != ERROR_SUCCESS)
                {
                    DWORD lastError = GetLastError();
                    RETURN_HR(HRESULT_FROM_WIN32(lastError));
                }
            }


            LSTATUS setResult = RegSetValueEx(regLocationToWriteChange.get()
                , L"PreviousValue"
                , 0
                , REG_SZ
                , reinterpret_cast<const BYTE*>(originalValue.c_str()), (originalValue.size() + 1) * sizeof(wchar_t));

            if (setResult != ERROR_SUCCESS)
            {
                DWORD lastError = GetLastError();
                RETURN_HR(HRESULT_FROM_WIN32(lastError));
            }

            setResult = RegSetValueEx(regLocationToWriteChange.get()
                , L"CurrentValue"
                , 0
                , REG_SZ
                , reinterpret_cast<const BYTE*>(m_Value.c_str()), (m_Value.size() + 1) * sizeof(wchar_t));

            if (setResult != ERROR_SUCCESS)
            {
                DWORD lastError = GetLastError();
                RETURN_HR(HRESULT_FROM_WIN32(lastError));
            }


            std::chrono::nanoseconds insertionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

            setResult = RegSetValueEx(regLocationToWriteChange.get()
                , L"InsertionTime"
                , 0
                , REG_QWORD
                , reinterpret_cast<const BYTE*>(insertionTime.count()), sizeof(insertionTime.count()));

            if (setResult != ERROR_SUCCESS)
            {
                DWORD lastError = GetLastError();
                RETURN_HR(HRESULT_FROM_WIN32(lastError));
            }

        }

        return callback();
    }
}
