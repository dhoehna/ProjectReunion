#pragma once
#include "pch.h"
#include "Helper.h"

class ElevatedEnvironmentManagerWin32Tests {
    BEGIN_TEST_CLASS(ElevatedEnvironmentManagerWin32Tests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        TEST_CLASS_PROPERTY(L"RunFixtureAs", L"Elevated")
    END_TEST_CLASS()

    TEST_METHOD_SETUP(TestSetup);
    TEST_CLASS_SETUP(WriteElevatedWin32MachineEV);
    TEST_CLASS_CLEANUP(RemoveElevatedWin32MachineEV);

    TEST_METHOD(TestGetEnvironmentVariableForMachine);
};
