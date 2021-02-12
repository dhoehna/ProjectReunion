#pragma once
#include "EnvironmentManager.g.h"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::Process::Environment::implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager>
    {
        enum Scope
        {
            Process,
            User,
            Machine
        };

        EnvironmentManager(Scope const& scope);

        static Environment::EnvironmentManager GetForProcess();
        static Environment::EnvironmentManager GetForUser();
        static Environment::EnvironmentManager GetForMachine();

        IMapView<hstring, hstring> GetEnvironmentVariables();
        hstring GetEnvironmentVariable(hstring variableName);

    private:
        Scope m_Scope;

        const LPWSTR USER_EV_REG_LOCATION = L"Environment";
        const LPWSTR MACHINE_EV_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";

        StringMap GetProcessEnvironmentVariables();
        StringMap GetUserOrMachineEnvironmentVariables();
        wil::unique_hkey GetRegHKeyForEVUserAndMachineScope(bool needsWriteAccess = false);
    };
}
namespace winrt::Microsoft::Process::Environment::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
