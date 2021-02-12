#pragma once
#include <EnvironmentVariableChangeTracker.g.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct EnvironmentVariableChangeTracker : EnvironmentVariableChangeTrackerT<EnvironmentVariableChangeTracker>
    {
        EnvironmentVariableChangeTracker() = default;

        winrt::hstring NameOfItemToTrack();
    };
}
