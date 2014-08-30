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
    		Text<4>		first = {this};
    		Text<5>		last = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;



    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( VariantFields );
