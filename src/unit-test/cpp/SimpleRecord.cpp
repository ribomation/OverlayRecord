
#include <cppunit/extensions/HelperMacros.h>

#include "Record.hpp"
using namespace overlay_record;


struct DummyTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( DummyTest );
		CPPUNIT_TEST( size_of_sequential_fields_should_be_8 );
		CPPUNIT_TEST( size_of_parallel_fields_should_be_4 );
		CPPUNIT_TEST( size_of_array_field_should_be_12 );
    CPPUNIT_TEST_SUITE_END();

    void size_of_sequential_fields_should_be_8();
    void size_of_parallel_fields_should_be_4();
    void size_of_array_field_should_be_12();
};
CPPUNIT_TEST_SUITE_REGISTRATION( DummyTest );


void DummyTest::size_of_sequential_fields_should_be_8() {
	struct Dummy : public Record {
	    Text<4>     text1 = {this};
	    Text<4>     text2 = {this};

	    Dummy() {
	        allocateDynamicBuffer();
	    }
	};

    Dummy  d;
    CPPUNIT_ASSERT_EQUAL(8U, d.size());
}

void DummyTest::size_of_parallel_fields_should_be_4() {
	struct Dummy : public Record {
	    Text<4>     text1 = {this};
	    Text<4>     text2 = {this, text1};

	    Dummy() {
	        allocateDynamicBuffer();
	    }
	};
    
    Dummy  d;
    CPPUNIT_ASSERT_EQUAL(4U, d.size());
}

void DummyTest::size_of_array_field_should_be_12() {
	struct Dummy : public Record {
	    Array<Text<4>, 3>     arr = {this};

	    Dummy() {
	        allocateDynamicBuffer();
	    }
	};

    Dummy  d;
    CPPUNIT_ASSERT_EQUAL(12U, d.size());
}

