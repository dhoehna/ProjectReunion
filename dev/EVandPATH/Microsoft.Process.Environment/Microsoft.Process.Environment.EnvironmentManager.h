#pragma once
#include "EnvironmentManager.g.h"

namespace winrt::Microsoft::Process::Environment::implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager>
    {
    public:

        enum Scope
        {
            Process,
            User,
            Machine
        };

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

        Scope m_Scope;

        EnvironmentManager(Scope scope);
    };
}
namespace winrt::Microsoft::Process::Environment::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
