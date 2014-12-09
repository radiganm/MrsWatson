#ifndef MrsWatsonTest_TestRunner_h
#define MrsWatsonTest_TestRunner_h

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "base/PlatformUtilities.h"
#include "base/CharString.h"
#include "base/FileUtilities.h"
#include "logging/LogPrinter.h"

#if UNIX
#include <unistd.h>
#endif

#ifndef __func__
#define __func__ __FUNCTION__
#endif

typedef enum {
    kTestLogEventSection,
    kTestLogEventPass,
    kTestLogEventFail,
    kTestLogEventSkip,
    kTestLogEventReset,
    kTestLogEventInvalid
} TestLogEventType;
const LogColor getLogColor(TestLogEventType eventType);

typedef int (*TestCaseExecFunc)(void);
typedef void (*TestCaseSetupFunc)(void);
typedef void (*TestCaseTeardownFunc)(void);

typedef struct {
    char *name;
    char *filename;
    int lineNumber;
    TestCaseExecFunc testCaseFunc;
} TestCaseMembers;
typedef TestCaseMembers *TestCase;

typedef struct {
    char *name;
    int numSuccess;
    int numFail;
    int numSkips;
    LinkedList testCases;
    TestCaseSetupFunc setup;
    TestCaseTeardownFunc teardown;
    boolByte onlyPrintFailing;
    boolByte keepFiles;
} TestSuiteMembers;
typedef TestSuiteMembers *TestSuite;

void addTestToTestSuite(TestSuite testSuite, TestCase testCase);
void runTestSuite(void *testSuitePtr, void *extraData);
void runTestCase(void *item, void *extraData);
void printTestName(const char *testName);
void printTestSuccess(void);
void printTestFail(void);

TestSuite newTestSuite(char *name, TestCaseSetupFunc setup, TestCaseTeardownFunc teardown);
TestCase newTestCase(char *name, char *filename, int lineNumber, TestCaseExecFunc testCaseFunc);

void freeTestCase(TestCase self);
void freeTestSuite(TestSuite self);

#define addTest(testSuite, name, testCaseFunc) { \
        addTestToTestSuite(testSuite, newTestCase(name, __FILE__, __LINE__, testCaseFunc)); \
    }

#define assert(_result) { \
        if(!(_result)) { \
            fprintf(stderr, "\nAssertion failed at %s:%d. ", getFileBasename(__FILE__), __LINE__); \
            return 1; \
        } \
    }

#define assertFalse(_result) assert((_result) == false)
#define assertIsNull(_result) assert((_result) == NULL)
#define assertNotNull(_result) assert((_result) != NULL)

#define assertIntEquals(expected, _result) { \
        int _resultInt = _result; \
        if(_resultInt != expected) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected %d, got %d. ", getFileBasename(__FILE__), __LINE__, expected, _resultInt); \
            return 1; \
        } \
    }

#define assertLongEquals(expected, _result) { \
        long _resultLong = _result; \
        if(_resultLong != expected) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected %lu, got %lu. ", getFileBasename(__FILE__), __LINE__, expected, _resultLong); \
            return 1; \
        } \
    }

#define ZERO_UNSIGNED_LONG (unsigned long)0
#define assertUnsignedLongEquals(expected, _result) { \
        unsigned long _resultULong = _result; \
        if(_resultULong != expected) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected %ld, got %ld. ", getFileBasename(__FILE__), __LINE__, expected, _resultULong); \
            return 1; \
        } \
    }

#define assertSizeEquals(expected, _result) { \
        size_t _resultSize = _result; \
        if(_result != expected) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected %zu, got %zu. ", getFileBasename(__FILE__), __LINE__, expected, _resultSize); \
            return 1; \
        } \
    }

#define TEST_DEFAULT_TOLERANCE 0.01
#define TEST_EXACT_TOLERANCE 0.0

#define assertDoubleEquals(expected, _result, tolerance) { \
        double resultRounded = floor(_result * 100.0) / 100.0; \
        double expectedRounded = floor(expected * 100.0) / 100.0; \
        double _resultDiff = fabs(resultRounded - expectedRounded); \
        if(_resultDiff > tolerance) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected %g, got %g. ", getFileBasename(__FILE__), __LINE__, expectedRounded, resultRounded); \
            return 1; \
        } \
    }

#define assertTimeEquals(expected, _result, tolerance) { \
        double resultRounded = floor(_result * 100.0) / 100.0; \
        double expectedRounded = floor(expected * 100.0) / 100.0; \
        double _resultDiff = fabs(resultRounded - expectedRounded); \
        if(_resultDiff > tolerance) { \
            fprintf(stderr, "Warning: timing assertion failed at %s:%d. Expected %g, got %g. ", getFileBasename(__FILE__), __LINE__, expectedRounded, resultRounded); \
            return 0; \
        } \
    }

#define assertCharStringEquals(expected, _result) { \
        if(!charStringIsEqualToCString(_result, expected, false)) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected '%s', got '%s'. ", getFileBasename(__FILE__), __LINE__, expected, _result->data); \
            return 1; \
        } \
    }

#define assertCharStringContains(expected, _result) { \
        if(strstr(_result->data, expected) == NULL) { \
            fprintf(stderr, "Assertion failed at %s:%d. Expected '%s' to contain '%s'. ", getFileBasename(__FILE__), __LINE__, _result->data, expected); \
            return 1; \
        } \
    }

#endif
