/*
 * Embed_Test.cpp
 *
 *  Created on: Aug 30, 2014
 *      Author: jens
 */


#include <cppunit/extensions/HelperMacros.h>
#include "Record.hpp"
using namespace overlay_record;
using namespace std;

struct Embed_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( Embed_Test );
		CPPUNIT_TEST( accessing_embedded_records_should_work );
    CPPUNIT_TEST_SUITE_END();

    void accessing_embedded_records_should_work() {
    	struct E : public Record {
    		Text<4>				txt = {this};
    		TextInteger<4>		num = {this};
    		Double				val = {this};
    	};

    	struct R : public Record {
    		Embed<E>	e = {this};
    		Integer		n = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	const unsigned  expectedSize = 4 + 4 + sizeof(double) + sizeof(int);
		CPPUNIT_ASSERT_EQUAL(expectedSize, r.size());




    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( Embed_Test );
