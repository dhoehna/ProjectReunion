#pragma once
#include "EnvironmentManager.g.h"

namespace winrt::Microsoft::Process::Environment::implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager>
    {
        EnvironmentManager() = default;
        static Environment::EnvironmentManager GetForUser();
        winrt::hstring SayHello();
    };
}
namespace winrt::Microsoft::Process::Environment::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
