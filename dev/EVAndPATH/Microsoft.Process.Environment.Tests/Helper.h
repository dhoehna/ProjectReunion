#pragma once
#include "pch.h"

using namespace winrt::Microsoft::Process::Environment; // for EnvironmentManager
using namespace winrt::Windows::Foundation::Collections; // for IMapView
typedef IMapView<winrt::hstring, winrt::hstring> EnvironmentVariables;

inline EnvironmentVariables GetAndFormatEnvironmentVariables(LPWSTR environmentVariableString)
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
inline void CompareIMapViews(EnvironmentVariables underTest, EnvironmentVariables real)
{
    // Make sure the sizes are the same.
    VERIFY_ARE_EQUAL(underTest.Size(), real.Size());

    if (underTest.Size() == 0)
    {
        return;
    }

    //MessageBoxEx(NULL, L"In here", L"In here", 0, 0);
    auto underTestIterator = underTest.First();
    do
    {
        // Make sure the same key exists in both collections
        auto keyFromTest = underTestIterator.Current().Key();

        std::wstringstream logMessage;
        logMessage << L"Key that is being tested: ";
        logMessage << keyFromTest.c_str();

        WEX::Logging::Log::Comment(logMessage.str().c_str());
        
        VERIFY_IS_TRUE(real.HasKey(keyFromTest));

        // Make sure they have the same value
        auto valueFromTest = underTestIterator.Current().Value();

        std::wstringstream valueMessage;
        valueMessage << L"Value from test: ";
        valueMessage << valueFromTest.c_str() << L"\r\n";
        valueMessage << L"Value from real: " << real.Lookup(keyFromTest).c_str();

        WEX::Logging::Log::Comment(valueMessage.str().c_str());
        VERIFY_ARE_EQUAL(valueFromTest, real.Lookup(keyFromTest));
        underTestIterator.MoveNext();
    } while (underTestIterator.HasCurrent());
}
