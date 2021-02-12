#pragma once
#include "pch.h"
#include "Helper.h"

class ElevatedEnvironmentManagerCentennialTests {
    BEGIN_TEST_CLASS(ElevatedEnvironmentManagerCentennialTests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
        TEST_CLASS_PROPERTY(L"RunFixtureAs", L"Elevated")
        TEST_CLASS_PROPERTY(L"UAP:Host", L"PackagedCwa")
        TEST_CLASS_PROPERTY(L"UAP:AppXManifest", L"CentennialAppxManifest.pkg.xml")
    END_TEST_CLASS()


    TEST_CLASS_SETUP(ElevatedCentennialTestSetup);
    TEST_CLASS_CLEANUP(ElevatedCentennialTestCleanup);

    TEST_METHOD_SETUP(TestSetup);

    TEST_METHOD(ElevatedCentennialTestGetEnvironmentVariableForMachine);
};
