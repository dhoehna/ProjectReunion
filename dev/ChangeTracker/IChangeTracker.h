#pragma once
#include <string>
#include <functional>
#include <windows.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    class IChangeTracker {
    public:
        virtual HRESULT TrackChange(std::function<HRESULT(void)> callBack) = 0;
    };
}
