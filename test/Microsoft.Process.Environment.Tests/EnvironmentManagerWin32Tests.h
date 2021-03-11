#pragma once
#include <Helper.h>

namespace ProjectReunionCppTest
{
    class EnvironmentManagerWin32Tests {
        BEGIN_TEST_CLASS(EnvironmentManagerWin32Tests)
            TEST_CLASS_PROPERTY(L"ActivationContext", L"CppTest.dll.manifest")
            //TEST_CLASS_PROPERTY(L"RunFixtureAs", L"Elevated")
        END_TEST_CLASS()

            TEST_METHOD_SETUP(TestSetup);

            TEST_METHOD(TestGetForProcess);
            TEST_METHOD(TestGetForUser);
            TEST_METHOD(TestGetForMachine);
            TEST_METHOD(TestGetEnvironmentVariablesForProcess);
            TEST_METHOD(TestGetEnvironmentVariablesForUser);
            TEST_METHOD(TestGetEnvironmentVariablesForMachine);
            TEST_METHOD(TestGetEnvironmentVariableForProcess);
            TEST_METHOD(TestGetEnvironmentVariableForUser);
            TEST_METHOD(TestSetEnvironmentVariableForProcess);
            TEST_METHOD(TestSetEnvironmentVariableForUser);
            TEST_METHOD(TestAppendToPathForProcess);
            TEST_METHOD(TestAppendToPathForUser);
    };
}
