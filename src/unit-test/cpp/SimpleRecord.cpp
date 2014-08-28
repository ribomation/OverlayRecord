
#include <cppunit/extensions/HelperMacros.h>

#include "Record.hpp"
using namespace ribomation;

struct Dummy : public Record {
    Text<4>     text1 = {this};
    Text<4>     text2 = {this};
    
    Dummy() {
        allocateDynamicBuffer();
    }
};

struct DummyTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( DummyTest );
        CPPUNIT_TEST( testSize );
    CPPUNIT_TEST_SUITE_END();

    void testSize();
};
CPPUNIT_TEST_SUITE_REGISTRATION( DummyTest );


void DummyTest::testSize() {
    Dummy  dummy;
    
    CPPUNIT_ASSERT_EQUAL(8U, dummy.size());
}


