#pragma once
#include "EnvironmentManager.g.h"


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

        static Microsoft::Process::Environment::EnvironmentManager GetForProcess();
        static Microsoft::Process::Environment::EnvironmentManager GetForCurrentUser();
        static Microsoft::Process::Environment::EnvironmentManager GetForMachine();
        hstring GetEnvironmentVariable(hstring const& name);
        Windows::Foundation::Collections::IMapView<hstring, hstring> GetEnvironmentVariables();
        void SetEnvironmentVariable(hstring const& name, hstring const& value);
        void AppendToPath(hstring const& path);
        void RemoveFromPath(hstring const& path);
        void AppendToPathExt(hstring const& path);
        void RemoveFromPathExt(hstring const& path);

    private :
        Scope m_Scope;
    };
}
namespace winrt::Microsoft::Process::Environment::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
