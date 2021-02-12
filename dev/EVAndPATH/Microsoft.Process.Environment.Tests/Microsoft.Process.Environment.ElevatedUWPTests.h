#pragma once
#include "pch.h"
#include "Helper.h"

class ElevatedEnvironmentManagerUWPTests {
    BEGIN_TEST_CLASS(ElevatedEnvironmentManagerUWPTests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
        TEST_CLASS_PROPERTY(L"RunFixtureAs", L"Elevated")
        TEST_CLASS_PROPERTY(L"UAP:AppXManifest", L"AppxManifest.pkg.xml")
    END_TEST_CLASS()

    TEST_CLASS_SETUP(WriteElevatedUWPUserAndMachineEV);
    TEST_CLASS_CLEANUP(RemoveElevatedUWPUserAndMachineEV);

    TEST_METHOD(UWPTestGetEnvironmentVariableForUser);
    TEST_METHOD(UWPTestGetEnvironmentVariableForMachine);
};
