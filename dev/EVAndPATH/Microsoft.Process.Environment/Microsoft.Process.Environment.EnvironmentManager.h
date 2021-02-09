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

    private:
        Scope m_Scope;

        StringMap GetProcessEnvironmentVariables();
        StringMap GetUserOrMachineEnvironmentVariables();
    };
}
namespace winrt::Microsoft::Process::Environment::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
