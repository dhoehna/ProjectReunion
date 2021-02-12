#include "pch.h"
#include "Microsoft.Process.Environment.ElevatedUWPTests.h"


bool ElevatedEnvironmentManagerUWPTests::WriteElevatedUWPUserAndMachineEV()
{
    WriteUserAndMachineEV();

    return true;
}

bool ElevatedEnvironmentManagerUWPTests::RemoveElevatedUWPUserAndMachineEV()
{
    RemoveUserAndMachineEV();

    return true;
}

void ElevatedEnvironmentManagerUWPTests::UWPTestGetEnvironmentVariableForUser()
{
    EnvironmentManager forProcess = EnvironmentManager::GetForUser();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}

void ElevatedEnvironmentManagerUWPTests::UWPTestGetEnvironmentVariableForMachine()
{
    EnvironmentManager forProcess = EnvironmentManager::GetForMachine();
    winrt::hstring environmentValue = forProcess.GetEnvironmentVariable(EV_KEY_NAME);
    VERIFY_ARE_EQUAL(std::wstring(EV_VALUE_NAME), environmentValue);
}
