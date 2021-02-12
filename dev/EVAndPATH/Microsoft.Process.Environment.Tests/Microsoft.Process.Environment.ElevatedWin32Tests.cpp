#include "pch.h"
#include "Microsoft.Process.Environment.ElevatedWin32Tests.h"

bool ElevatedEnvironmentManagerWin32Tests::TestSetup()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    return true;
}

bool ElevatedEnvironmentManagerWin32Tests::WriteElevatedWin32MachineEV()
{
    WriteMachineEV();

    return true;
}

bool ElevatedEnvironmentManagerWin32Tests::RemoveElevatedWin32MachineEV()
{
    RemoveMachineEV();

    return true;
}

void ElevatedEnvironmentManagerWin32Tests::TestGetEnvironmentVariableForMachine()
{
    EnvironmentManager forProcess = EnvironmentManager::GetForMachine();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}
