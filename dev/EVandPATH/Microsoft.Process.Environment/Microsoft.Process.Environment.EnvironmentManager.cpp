#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"
#include "EnvironmentManager.g.cpp"


namespace winrt::Microsoft::Process::Environment::implementation
{
    EnvironmentManager::EnvironmentManager(Scope const& scope)
        : m_Scope(scope) { }

    Microsoft::Process::Environment::EnvironmentManager EnvironmentManager::GetForProcess()
    {
        MessageBoxEx(nullptr, L"In here", L"In here", 0, 0);
        Environment::EnvironmentManager yolo{ nullptr };
        yolo = winrt::make<Environment::implementation::EnvironmentManager>(Scope::Process);

        return yolo;
        //auto yolo{ Microsoft::Process::Environment::implementation::EnvironmentManager() };
        //return yolo;
        // Adding a default constructor did not help.

        //return winrt::make<winrt::Microsoft::Process::Environment::EnvironmentManager>();

        //return winrt::Microsoft::Process::Environment::implementation::EnvironmentManager;
        //return winrt::make<winrt::Microsoft::Process::Environment::implementation::EnvironmentManager>(Scope::Process);
        //return winrt::make<winrt::Microsoft::Process::Environment::EnvironmentManager>(Scope::Process);
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
