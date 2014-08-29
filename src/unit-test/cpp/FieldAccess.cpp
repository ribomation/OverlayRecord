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
		CPPUNIT_TEST( read_write_of_binary_fields_should_work );
    CPPUNIT_TEST_SUITE_END();


    void read_write_of_text_fields_should_work() {
    	struct R : public Record {
    		Text<4>		first = {this};
    		Text<5>		last = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	CPPUNIT_ASSERT_EQUAL(4U, (unsigned)r.first.value().size());
    	CPPUNIT_ASSERT_EQUAL(string("    "), r.first.value());

    	r.first = "Anna";
    	r.last  = "Conda";
    	CPPUNIT_ASSERT_EQUAL(string("Anna"), r.first.value());
    	CPPUNIT_ASSERT_EQUAL(string("Conda"), r.last.value());

    	r.first = "Very Long Payload";
    	CPPUNIT_ASSERT_EQUAL(string("Very"), r.first.value());
    }


    void read_write_of_numeric_text_fields_should_work() {
    	struct R : public Record {
    		TextInteger<4>		f1 = {this};
    		TextFloat<6>		f2 = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;
    	CPPUNIT_ASSERT_EQUAL(0, r.f1.value());
    	CPPUNIT_ASSERT_EQUAL(0.0F, r.f2.value());

    	r.f1 = 2014;
    	r.f2 = 3.141592654;
    	CPPUNIT_ASSERT_EQUAL(2014, r.f1.value());
    	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1415, r.f2.value(), 0.00001);
    }


    void read_write_of_binary_fields_should_work() {
    	struct R : public Record {
    		Integer		f1 = {this};
    		Short		f2 = {this};
    		Double		f3 = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;
    	r.f1 = 1234567890;
    	r.f2 = 65000;
    	r.f3 = 12345.67890;
    	CPPUNIT_ASSERT_EQUAL(1234567890, r.f1.value());
    	CPPUNIT_ASSERT_EQUAL(65000, r.f2.value());
    	CPPUNIT_ASSERT_DOUBLES_EQUAL(12345.6789, r.f3.value(), 0.00001);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( FieldAccess );

















