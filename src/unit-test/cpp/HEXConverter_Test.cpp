/*
 * HEXConverter_Test.cpp
 *
 *  Created on: Aug 30, 2014
 *      Author: jens
 */


#include <cppunit/extensions/HelperMacros.h>
#include "Record.hpp"
using namespace std;
using namespace overlay_record;

struct HEXConverter_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( HEXConverter_Test );
	CPPUNIT_TEST( fromStorage_should_return_twice_sized_string );
	CPPUNIT_TEST( toStorage_should_produce_half_sized_payload );
    CPPUNIT_TEST_SUITE_END();

    void fromStorage_should_return_twice_sized_string() {
    	unsigned char	payload[] = {0x00, 0x0F, 0xF0, 0xFF};
    	string			expected  = "000FF0FF";

    	Record::HEXConverter	hex;
    	string	actual = hex.fromStorage(payload, sizeof(payload));

		CPPUNIT_ASSERT_EQUAL(2 * sizeof(payload), actual.size());
		CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void toStorage_should_produce_half_sized_payload() {
    	string			payload    = "000FF0FF";
    	unsigned char	expected[] = {0x00, 0x0F, 0xF0, 0xFF};

    	unsigned char  actual[] = {0,0,0,0};
    	const size_t   N        = sizeof(actual) / sizeof(unsigned char);
    	for (size_t k=0; k<N; ++k)
    		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>(0x00), actual[k]);

    	Record::HEXConverter	hex;
    	hex.toStorage(payload, actual, N);
    	for (size_t k=0; k<N; ++k)
    	    CPPUNIT_ASSERT_EQUAL(expected[k], actual[k]);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( HEXConverter_Test );
