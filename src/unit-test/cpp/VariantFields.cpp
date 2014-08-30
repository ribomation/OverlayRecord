/*
 * VariantFields.cpp
 *
 *  Created on: Aug 29, 2014
 *      Author: jens
 */

#include <cppunit/extensions/HelperMacros.h>

#include "Record.hpp"
using namespace ribomation;
using namespace std;

struct VariantFields : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( VariantFields );
		CPPUNIT_TEST( few_overlaid_fields_should_work );
    CPPUNIT_TEST_SUITE_END();

    void few_overlaid_fields_should_work() {
    	struct R : public Record {
    		Text<12>	all = {this};

    		Text<6>		half1 = {this, all};
    		Text<6>		half2 = {this};

    		Text<4>		third1 = {this, all};
    		Text<4>		third2 = {this};
    		Text<4>		third3 = {this};

    		Text<2>		tail1  = {this, third3};
    		Text<2>		tail2  = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	CPPUNIT_ASSERT_EQUAL(12U, r.size());

    	CPPUNIT_ASSERT_EQUAL(string(12, ' '), r.all.value());
    	CPPUNIT_ASSERT_EQUAL(string( 6, ' '), r.half2.value());
    	CPPUNIT_ASSERT_EQUAL(string( 4, ' '), r.third1.value());
    	CPPUNIT_ASSERT_EQUAL(string( 2, ' '), r.tail2.value());

    	r.all = "aabbccddeeff";
    	CPPUNIT_ASSERT_EQUAL(string("ddeeff"), r.half2.value());
    	CPPUNIT_ASSERT_EQUAL(string("ccdd")  , r.third2.value());
    	CPPUNIT_ASSERT_EQUAL(string("ee")    , r.tail1.value());
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( VariantFields );
