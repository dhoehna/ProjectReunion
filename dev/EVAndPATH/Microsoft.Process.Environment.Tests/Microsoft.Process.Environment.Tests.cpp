#include "pch.h"

using namespace winrt::Microsoft::Process::Environment; // for EnvironmentManager
using namespace winrt::Windows::Foundation::Collections; // for IMapView
typedef IMapView<winrt::hstring, winrt::hstring> EnvironmentVariables;

EnvironmentVariables GetAndFormatEnvironmentVariables(LPWSTR environmentVariableString)
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

/// <summary>
/// Compares two IMapView<winrt::hstring, winrt::hstring> collections for
/// 1. Have the same size
/// 2. All the keys in <paramref name="underTest" /> are in <paramref name="real" />
/// 3. All the values in <paramref name="underTest" /> are in <paramref name="real" />
/// </summary>
/// <param name="underTest">The EnvironmentVariables from the WInRT API that is being tested</param>
/// <param name="real">The Environment Variables that came from GetEnvironmentStrings</param>
/// <remarks>
/// This test will alwayse use <paramref name="underTest" /> to compare to <paramref name="real" />.  As long as the size, keys, and values
/// are compared from <paramref name="underTest" /> to <paramref name="real" /> this will fail in the case that any keys are different, or any values are different
/// </remarks>
void CompareIMapViews(EnvironmentVariables underTest, EnvironmentVariables real)
{
    // Make sure the sizes are the same.
    VERIFY_ARE_EQUAL(underTest.Size(), real.Size());

    if (underTest.Size() == 0)
    {
        return;
    }

    // loop-and-a-half problem.
    // We need to loop through every item, but we need to do the test on the
    // first item outside of the loop in order to prime the loop.
    auto underTestIterator = underTest.First();
    auto underTestEnvironmentVariable = underTestIterator.Current();
    auto value = underTestEnvironmentVariable.Value();

    VERIFY_IS_TRUE(real.HasKey(underTestEnvironmentVariable.Key()));
    VERIFY_ARE_EQUAL(underTestEnvironmentVariable.Value(), real.Lookup(underTestEnvironmentVariable.Key()));

    while (underTestIterator.MoveNext())
    {
        underTestEnvironmentVariable = underTestIterator.Current();
        value = underTestEnvironmentVariable.Value();

        VERIFY_IS_TRUE(real.HasKey(underTestEnvironmentVariable.Key()));
        VERIFY_ARE_EQUAL(underTestEnvironmentVariable.Value(), real.Lookup(underTestEnvironmentVariable.Key()));
    }
}

class EnvironmentManagerTests   {
   // Declare this class as a TestClass, and supply metadata if necessary.
    BEGIN_TEST_CLASS(EnvironmentManagerTests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        END_TEST_CLASS()

        TEST_METHOD_SETUP(TestSetup);
 
   TEST_METHOD(TestGetForProcess);
   TEST_METHOD(TestGetForUser);
   TEST_METHOD(TestGetForMachine);
   TEST_METHOD(TestGetEnvironmentVariablesForProcess);
};

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

void EnvironmentManagerTests::TestGetEnvironmentVariablesForProcess()
{
    LPWSTR environmentVariables = GetEnvironmentStrings();
    EnvironmentVariables environmentVariablesFromWinRTAPI = GetAndFormatEnvironmentVariables(environmentVariables);

    EnvironmentManager environmentmanager = EnvironmentManager::GetForProcess();
    EnvironmentVariables environmentVariablesFromWindowsAPI = environmentmanager.GetEnvironmentVariables();

    CompareIMapViews(environmentVariablesFromWinRTAPI, environmentVariablesFromWindowsAPI);
    
}
