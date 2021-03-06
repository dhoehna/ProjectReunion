﻿#pragma once
#include "pch.h"
#include "Helper.h"

class EnvironmentManagerCentennialTests {
    BEGIN_TEST_CLASS(EnvironmentManagerCentennialTests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
        TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
        TEST_CLASS_PROPERTY(L"UAP:Host", L"PackagedCwa")
        TEST_CLASS_PROPERTY(L"UAP:AppXManifest", L"CentennialAppxManifest.pkg.xml")
    END_TEST_CLASS()


    TEST_CLASS_SETUP(CentennialTestSetup);
    TEST_CLASS_CLEANUP(CentennialTestCleanup);

    TEST_METHOD_SETUP(TestSetup);

    TEST_METHOD(CentennialTestGetForProcess);
    TEST_METHOD(CentennialTestGetForUser);
    TEST_METHOD(CentennialTestGetForMachine);
    TEST_METHOD(CentennialTestGetEnvironmentVariablesForProcess);
    TEST_METHOD(CentennialTestGetEnvironmentVariablesForUser);
    TEST_METHOD(CentennialTestGetEnvironmentVariablesForMachine);
    TEST_METHOD(CentennialTestGetEnvironmentVariableForProcess);
    TEST_METHOD(CentennialTestGetEnvironmentVariableForUser);
};
