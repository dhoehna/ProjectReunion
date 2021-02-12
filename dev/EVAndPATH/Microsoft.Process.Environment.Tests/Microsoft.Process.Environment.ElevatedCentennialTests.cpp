#include "pch.h"
#include "Microsoft.Process.Environment.ElevatedCentennialTests.h"

bool ElevatedEnvironmentManagerCentennialTests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

bool ElevatedEnvironmentManagerCentennialTests::ElevatedCentennialTestSetup()
{
    WriteMachineEV();
    return true;
}

bool ElevatedEnvironmentManagerCentennialTests::ElevatedCentennialTestCleanup()
{
    RemoveMachineEV();
    return true;
}


void ElevatedEnvironmentManagerCentennialTests::ElevatedCentennialTestGetEnvironmentVariableForMachine()
{
    EnvironmentManager forProcess = EnvironmentManager::GetForMachine();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}
