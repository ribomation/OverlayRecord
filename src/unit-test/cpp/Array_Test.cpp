/*
 * Array_Test.cpp
 *
 *  Created on: Aug 30, 2014
 *      Author: jens
 */


#include <cppunit/extensions/HelperMacros.h>
#include "Record.hpp"
using namespace overlay_record;
using namespace std;

struct Array_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( Array_Test );
		CPPUNIT_TEST( indexing_field_arrays_should_work );
    CPPUNIT_TEST_SUITE_END();

    void indexing_field_arrays_should_work() {
    	struct R : public Record {
    		Text<12>			txtAll = {this};
    		Array<Text<4>, 3>	txt    = {this, txtAll};
    		Array<Integer, 4>	ip     = {this};
    		Array<Double, 3>	vec    = {this};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;

    	const unsigned  expectedSize = 12U + 4*sizeof(int) + 3*sizeof(double);
		CPPUNIT_ASSERT_EQUAL(expectedSize, r.size());

		r.txtAll = "aaaabbbbcccc";
		for (int k=0; k<3; ++k)
			CPPUNIT_ASSERT_EQUAL(string(4, 'a' + k), r.txt[k].value());

		for (int k=0; k<4; ++k)
			CPPUNIT_ASSERT_EQUAL(0, r.ip[k].value());

		r.ip = {100, 200, 300, 400};
		for (int k=0; k<4; ++k)
			CPPUNIT_ASSERT_EQUAL(100 * (k+1), r.ip[k].value());

		{	int k=1;
			for (int v : r.ip)
				CPPUNIT_ASSERT_EQUAL(100 * k++, v);
		}

		r.vec = {1.25, 1.75, 2.25};
		for (int k=0; k<3; ++k)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25 + k*0.5, r.vec[k].value(), 0.0001);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( Array_Test );
