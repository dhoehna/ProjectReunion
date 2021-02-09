#pragma once
#include "pch.h"
#include "Helper.h"

class EnvironmentManagerWin32Tests {
    BEGIN_TEST_CLASS(EnvironmentManagerWin32Tests)
        TEST_CLASS_PROPERTY(L"ActivationContext", L"Microsoft.Process.Environment.Tests.exe.manifest")
    END_TEST_CLASS()

    TEST_METHOD_SETUP(TestSetup);

    //TEST_METHOD(TestGetForProcess);
    //TEST_METHOD(TestGetForUser);
    //TEST_METHOD(TestGetForMachine);
    //TEST_METHOD(TestGetEnvironmentVariablesForProcess);
    TEST_METHOD(TestGetEnvironmentVariablesForUser);
};
