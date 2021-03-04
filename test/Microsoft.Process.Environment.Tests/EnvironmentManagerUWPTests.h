#pragma once
#include <Helper.h>

namespace ProjectReunionCppTest
{
    class EnvironmentManagerUWPTests {
        BEGIN_TEST_CLASS(EnvironmentManagerUWPTests)
            TEST_CLASS_PROPERTY(L"ActivationContext", L"CppTest.dll.manifest")
            TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
            TEST_CLASS_PROPERTY(L"UAP:AppXManifest", L"AppxManifest.pkg.xml")
            END_TEST_CLASS()

        TEST_METHOD(UWPTestGetForProcess);
        TEST_METHOD(UWPTestGetForUser);
        TEST_METHOD(UWPTestGetForMachine);
        TEST_METHOD(UWPTestGetEnvironmentVariablesForProcess);
        TEST_METHOD(UWPTestGetEnvironmentVariablesForUser);
        TEST_METHOD(UWPTestGetEnvironmentVariablesForMachine);
        TEST_METHOD(UWPTestGetEnvironmentVariableForProcess);
        //TEST_METHOD(UWPTestGetEnvironmentVariableForUser);
        //TEST_METHOD(UWPTestGetEnvironmentVariableForMachine);
    };
}
