#include "pch.h"

using namespace winrt::Microsoft::Process::Environment; // for EnvironmentManager
using namespace winrt::Windows::Foundation::Collections; // for IMapView

class EnvironmentManagerTests   {
   // Declare this class as a TestClass, and supply metadata if necessary.
    BEGIN_TEST_CLASS(EnvironmentManagerTests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        END_TEST_CLASS()

        TEST_METHOD_SETUP(TestSetup);
 
   TEST_METHOD(TestGetForProcess);
   TEST_METHOD(TestGetForUser);
   TEST_METHOD(TestGetForMachine);
   TEST_METHOD(TestGetEnvironmentVariables);
};

IMapView<winrt::hstring, winrt::hstring> GetAndFormatEnvironmentVariables(LPWSTR environmentVariableString)
{

    VERIFY_IS_NOT_NULL(environmentVariableString);

    LPTSTR variable;
    variable = (LPTSTR)environmentVariableString;

    StringMap environmentVariables;
    while (*variable)
    {
        std::wstring environmentVariable(variable);
        size_t locationOfNull = environmentVariable.find_last_of(L'=');

        environmentVariables.Insert(environmentVariable.substr(0, locationOfNull), environmentVariable.substr(locationOfNull + 1));

        variable += lstrlen(variable) + 1;
    }

    VERIFY_WIN32_BOOL_SUCCEEDED(FreeEnvironmentStrings(environmentVariableString));

    return environmentVariables.GetView();
}


bool EnvironmentManagerTests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

void EnvironmentManagerTests::TestGetForProcess()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForProcess();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerTests::TestGetForUser()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForUser();
    VERIFY_IS_NOT_NULL(environmentManager);
}


void EnvironmentManagerTests::TestGetForMachine()
{
    EnvironmentManager environmentManager = EnvironmentManager::GetForMachine();
    VERIFY_IS_NOT_NULL(environmentManager);
}

void EnvironmentManagerTests::TestGetEnvironmentVariables()
{
    LPWSTR environmentVariables = GetEnvironmentStrings();
    IMapView<winrt::hstring, winrt::hstring> environmentVariablesExpected = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    IMapView<winrt::hstring, winrt::hstring> environmentVariablesResult = environmentmanager.GetEnvironmentVariables();

    MessageBoxEx(NULL, L"In here", L"In here", 0, 0);
    VERIFY_ARE_EQUAL(environmentVariablesExpected.Size(), environmentVariablesResult.Size());
}
