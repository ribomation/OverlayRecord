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
		CPPUNIT_TEST( accessing_simple_embedded_record_should_work );
		CPPUNIT_TEST( accessing_overlaid_embedded_record_should_work );
		CPPUNIT_TEST( using_nested_embedded_records_should_worl_as_well );
    CPPUNIT_TEST_SUITE_END();

    void accessing_simple_embedded_record_should_work() {
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

		CPPUNIT_ASSERT_EQUAL(string(4, ' '), r.e->txt.value());

		r.e->txt.value("abcd");
		r.e->num = 42;
		r.e->val = 3.1415926;

		CPPUNIT_ASSERT_EQUAL(string("abcd"), r.e->txt.value());
		CPPUNIT_ASSERT_EQUAL(42, r.e->num.value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14159, r.e->val.value(), 0.0001);
    }

    void accessing_overlaid_embedded_record_should_work() {
    	struct E : public Record {
    		Text<4>				txt = {this};
    		TextInteger<4>		num = {this};
    	};

    	struct R : public Record {
    		Text<8>		all = {this};
    		Embed<E>	e   = {this, all};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;
    	CPPUNIT_ASSERT_EQUAL(8U, r.size());

    	r.all = "abcd0042";
    	CPPUNIT_ASSERT_EQUAL(string("abcd"), r.e->txt.value());
    	CPPUNIT_ASSERT_EQUAL(42            , r.e->num.value());
    }

    void using_nested_embedded_records_should_worl_as_well() {
    	struct X : public Record {
    		Text<4>				txt = {this};
    		TextInteger<4>		num = {this};
    	};

    	struct E : public Record {
    		Text<4>				txt = {this};
    		Embed<X>			x   = {this};
    		TextInteger<4>		num = {this};
    	};

    	struct R : public Record {
    		Text<16>	all = {this};
    		Embed<E>	e   = {this, all};

    		R() { allocateDynamicBuffer(); }
    	};

    	R  r;
    	CPPUNIT_ASSERT_EQUAL(16U, r.size());

    	r.all = "ABCDabcd00420017";

//    	cout << "r.all = ["<<r.all.value()<<"]"<<endl;
//    	cout << "r.e.txt = ["<<r.e->txt.value()<<"]"<<endl;
//    	cout << "r.e.x.txt = ["<<r.e->x->txt.value()<<"]"<<endl;
//    	cout << "r.e.x.num = ["<<r.e->x->num.value()<<"]"<<endl;
//    	cout << "r.e.num = ["<<r.e->num.value()<<"]"<<endl;

    	CPPUNIT_ASSERT_EQUAL(string("ABCD"), r.e->txt.value());

    	CPPUNIT_ASSERT_EQUAL(string("abcd"), r.e->x->txt.value());
    	CPPUNIT_ASSERT_EQUAL(42            , r.e->x->num.value());

    	CPPUNIT_ASSERT_EQUAL(17            , r.e->num.value());
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( Embed_Test );




