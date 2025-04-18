#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <pocketsphinx/logmath.h>

#define EPSILON 0.01
#define TEST_ASSERT(x) if (!(x)) { fprintf(stderr, "FAIL: %s\n", #x); exit(1); }
#define TEST_EQUAL(a,b) TEST_ASSERT((a) == (b))
#define TEST_EQUAL_FLOAT(a,b) TEST_ASSERT(fabs((a) - (b)) < EPSILON)
#ifdef FIXED_POINT
#define TEST_EQUAL_MFCC(a,b) TEST_EQUAL(a,b)
#else
#define TEST_EQUAL_MFCC(a,b) TEST_ASSERT(fabs((a) - (b)) < EPSILON)
#endif
#define TEST_EQUAL_STRING(a,b) TEST_ASSERT(0 == strcmp((a), (b)))
#define LOG_EPSILON 200
#define TEST_EQUAL_LOG(a,b) TEST_ASSERT(abs((a) - (b)) < LOG_EPSILON)
#define MODELDIR "/home/hah/Downloads/pocketsphinx-master/model"
#define DATADIR "/home/hah/Downloads/pocketsphinx-master/test/data"
#define TESTDATADIR "/home/hah/Downloads/pocketsphinx-master/test/regression"
