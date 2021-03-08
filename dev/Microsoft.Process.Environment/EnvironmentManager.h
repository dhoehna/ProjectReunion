#pragma once
#include <EnvironmentManager.g.h>

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager>
    {
        enum Scope
        {
            Process = 0,
            User = 1,
            Machine = 2
        };

        EnvironmentManager(Scope const& scope);

        static Microsoft::ProjectReunion::EnvironmentManager GetForProcess();
        static Microsoft::ProjectReunion::EnvironmentManager GetForUser();
        static Microsoft::ProjectReunion::EnvironmentManager GetForMachine();
        Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::hstring> GetEnvironmentVariables();
        hstring GetEnvironmentVariable(hstring const& name);
        void SetEnvironmentVariable(hstring const& name, hstring const& value);
        void AppendToPath(hstring const& path);
        void RemoveFromPath(hstring const& path);
        void AddExecutableFileExtension(hstring const& pathExt);
        void RemoveExecutableFileExtension(hstring const& pathExt);
    private:
        Scope m_Scope;

        std::wstring USER_EV_REG_LOCATION = L"Environment";
        std::wstring MACHINE_EV_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";

        StringMap GetProcessEnvironmentVariables();
        StringMap GetUserOrMachineEnvironmentVariables();
        wil::unique_hkey GetRegHKeyForEVUserAndMachineScope(bool needsWriteAccess = false);

        bool IsCurrentOS20H2OrLower()
        {
            OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
            DWORDLONG const dwlConditionMask = VerSetConditionMask(0, VER_BUILDNUMBER, VER_LESS_EQUAL);
            osvi.dwBuildNumber = 19042;

            BOOL versionVerification = VerifyVersionInfoW(&osvi, VER_BUILDNUMBER, dwlConditionMask);

            // If the OS the API is running on is 20H2 or lower
            if (versionVerification != 0)
            {
                return true;
            }
            else
            {
                // Either the OS is higher than 20H2 or there is an error.
                DWORD error = GetLastError();

                if (error == ERROR_OLD_WIN_VERSION)
                {
                    return false;
                }
                else
                {
                    THROW_HR(HRESULT_FROM_WIN32(error));
                }

            }
        }
    };
}
namespace winrt::Microsoft::ProjectReunion::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
