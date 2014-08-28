/*
 * FieldAccess.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: jens
 */

#include <cppunit/extensions/HelperMacros.h>

#include "Record.hpp"
using namespace ribomation;
using namespace std;

struct FieldAccess : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( FieldAccess );
		CPPUNIT_TEST( read_write_of_text_fields_should_work );
		CPPUNIT_TEST( read_write_of_numeric_text_fields_should_work );
    CPPUNIT_TEST_SUITE_END();

    void read_write_of_text_fields_should_work() {
    	struct R : public Record {
    		Text<4>		first = {this};
    		Text<5>		last = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	CPPUNIT_ASSERT_EQUAL(4U, (unsigned)r.first.value().size());

    	r.first = "Anna";
    	r.last  = "Conda";
    	CPPUNIT_ASSERT_EQUAL(string("Anna"), r.first.value());
    	CPPUNIT_ASSERT_EQUAL(string("Conda"), r.last.value());

    	r.first = "Very Long Payload";
    	CPPUNIT_ASSERT_EQUAL(r.first.value(), string("Very"));
    }

    void read_write_of_numeric_text_fields_should_work() {
    	struct R : public Record {
    		TextInteger<4>		year = {this};
    		TextFloat<6>		pi = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	r.year = 2014;
    	r.pi   = 3.141592654;
    	CPPUNIT_ASSERT_EQUAL(2014, r.year.value());
    	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1415, r.pi.value(), 0.00001);

    }
};
CPPUNIT_TEST_SUITE_REGISTRATION( FieldAccess );

