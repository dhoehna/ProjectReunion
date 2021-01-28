﻿#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"
#include "EnvironmentManager.g.cpp"


namespace winrt::Microsoft::Process::Environment::implementation
{
    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForProcess()
    {
        throw hresult_not_implemented();
    }
    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForCurrentUser()
    {
        throw hresult_not_implemented();
    }
    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForMachine()
    {
        throw hresult_not_implemented();
    }
    hstring EnvironmentManager::GetEnvironmentVariable(hstring const& name)
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::Collections::IMapView<hstring, hstring> EnvironmentManager::GetEnvironmentVariables()
    {
        throw hresult_not_implemented();
    }
    void EnvironmentManager::SetEnvironmentVariable(hstring const& name, hstring const& value)
    {
        throw hresult_not_implemented();
    }
    void EnvironmentManager::AppendToPath(hstring const& path)
    {
        throw hresult_not_implemented();
    }
    void EnvironmentManager::RemoveFromPath(hstring const& path)
    {
        throw hresult_not_implemented();
    }
    void EnvironmentManager::AppendToPathExt(hstring const& path)
    {
        throw hresult_not_implemented();
    }
    void EnvironmentManager::RemoveFromPathExt(hstring const& path)
    {
        throw hresult_not_implemented();
    }
}
