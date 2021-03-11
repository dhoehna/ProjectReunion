#pragma once
#include <Helper.h>

namespace ProjectReunionCppTest
{
    class EnvironmentManagerCentennialTests {
        BEGIN_TEST_CLASS(EnvironmentManagerCentennialTests)
            TEST_CLASS_PROPERTY(L"ActivationContext", L"CppTest.dll.manifest")
            TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
            TEST_CLASS_PROPERTY(L"UAP:Host", L"PackagedCwa")
            TEST_CLASS_PROPERTY(L"UAP:AppXManifest", L"CentennialAppxManifest.pkg.xml")
        END_TEST_CLASS()

        TEST_METHOD(CentennialTestGetForProcess);
        TEST_METHOD(CentennialTestGetForUser);
        TEST_METHOD(CentennialTestGetForMachine);
        TEST_METHOD(CentennialTestGetEnvironmentVariablesForProcess);
        TEST_METHOD(CentennialTestGetEnvironmentVariablesForUser);
        TEST_METHOD(CentennialTestGetEnvironmentVariablesForMachine);
        TEST_METHOD(CentennialTestGetEnvironmentVariableForProcess);
        TEST_METHOD(CentennialTestGetEnvironmentVariableForUser);
        TEST_METHOD(CentennialTestSetEnvironmentVariableForProcess);
        TEST_METHOD(CentennialTestSetEnvironmentVariableForUser);
        //TEST_METHOD(CentennialTestSetEnvironmentVariableForMachine);
        TEST_METHOD(CentennialTestAppendToPathForProcess);
        TEST_METHOD(CentennialTestAppendToPathForUser);
    };

}
