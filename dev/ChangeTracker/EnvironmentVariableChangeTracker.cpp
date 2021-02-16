#include "pch.h"
#include <EnvironmentVariableChangeTracker.h>
#include <EnvironmentVariableChangeTracker.g.cpp>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    winrt::hstring EnvironmentVariableChangeTracker::NameOfItemToTrack()
    {
        return winrt::hstring(L"Environment");
    }
}
