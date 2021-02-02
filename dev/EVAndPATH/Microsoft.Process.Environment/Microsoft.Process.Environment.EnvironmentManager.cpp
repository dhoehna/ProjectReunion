#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"
#include "EnvironmentManager.g.cpp"

namespace winrt::Microsoft::Process::Environment::implementation
{
    Environment::EnvironmentManager EnvironmentManager::GetForUser()
    {        
        /*auto environmentManager = winrt::make<EnvironmentManager>();
        return environmentManager.as<Environment::EnvironmentManager>();*/
        //return winrt::make<Microsoft::Process::Environment::EnvironmentManager>();
        /*Environment::implementation::EnvironmentManager environmentManager{ nullptr };
        return environmentManager;*/

        //auto yolo = winrt::make_self<EnvironmentManager>();
        //return yolo.as<EnvironmentManager>();
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>();
        return environmentManager;
    }

    winrt::hstring EnvironmentManager::SayHello()
    {
        return L"Hello.  I am the best";
    }
}
