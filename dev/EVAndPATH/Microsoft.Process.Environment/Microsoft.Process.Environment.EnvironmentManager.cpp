#include "pch.h"
#include "Microsoft.Process.Environment.EnvironmentManager.h"
#include "EnvironmentManager.g.cpp"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::Process::Environment::implementation
{

    EnvironmentManager::EnvironmentManager(Scope const& scope)
        : m_Scope(scope) { }

    Environment::EnvironmentManager EnvironmentManager::GetForProcess()
    {        
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Process);
        return environmentManager;
    }

    Environment::EnvironmentManager EnvironmentManager::GetForUser()
    {
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::User);
        return environmentManager;
    }

    Environment::EnvironmentManager EnvironmentManager::GetForMachine()
    {
        Environment::EnvironmentManager environmentManager{ nullptr };
        environmentManager = winrt::make<implementation::EnvironmentManager>(Scope::Machine);
        return environmentManager;
    }

    Windows::Foundation::Collections::IMapView<hstring, hstring> EnvironmentManager::GetEnvironmentVariables()
    {
        Windows::Foundation::Collections::StringMap environmentVariables;

        environmentVariables = GetAndFormatEnvironmentVariables();

        return environmentVariables.GetView();
    }

    StringMap EnvironmentManager::GetAndFormatEnvironmentVariables()
    {
        //Get the pointer to the process block
        LPWSTR environmentVariablesString = GetEnvironmentStrings();
        THROW_HR_IF_NULL(E_POINTER, environmentVariablesString);

        // parse the block into a map
        LPTSTR variable;
        variable = (LPTSTR)environmentVariablesString;

        StringMap environmentVariables;
        while (*variable)
        {
            std::wstring environmentVariable(variable);
            size_t locationOfNull = environmentVariable.find_last_of(L'=');

            environmentVariables.Insert(environmentVariable.substr(0, locationOfNull), environmentVariable.substr(locationOfNull + 1));

            variable += lstrlen(variable) + 1;
        }

        THROW_IF_WIN32_BOOL_FALSE(FreeEnvironmentStrings(environmentVariablesString));

        return environmentVariables;
    }
}
