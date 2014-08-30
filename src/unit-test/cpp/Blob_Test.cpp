/*
 * Blob_Test.cpp
 *
 *  Created on: Aug 30, 2014
 *      Author: jens
 */


#include <cppunit/extensions/HelperMacros.h>
#include "Record.hpp"
using namespace overlay_record;
using namespace std;

struct Blob_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( Blob_Test );
		CPPUNIT_TEST( blob_content_should_be_viewable_as_hex );
		CPPUNIT_TEST( hex_payload_should_be_blob_content );
    CPPUNIT_TEST_SUITE_END();

    void blob_content_should_be_viewable_as_hex() {
    	struct R : public Record {
    		Blob<4>    data = {this};

    		R(unsigned char* buf, unsigned sz) { assignStaticBuffer(reinterpret_cast<char*>(buf), sz); }
    	};

    	unsigned char  payload[] = {0x0F, 0x1F, 0x2F, 0x3F};
    	R  r(payload, sizeof(payload) / sizeof(unsigned char));

		CPPUNIT_ASSERT_EQUAL(4U, r.size());

		//cout << "r.data = ["<<r.data.value()<<"]"<<endl;
		CPPUNIT_ASSERT_EQUAL(string("0F1F2F3F"), r.data.value());
    }

    void hex_payload_should_be_blob_content() {
    	struct R : public Record {
    		Blob<4>    data = {this};

    		R(unsigned char* buf, unsigned sz) { assignStaticBuffer(reinterpret_cast<char*>(buf), sz); }
    	};

    	unsigned char  buf[4];
    	const size_t   N = sizeof(buf) / sizeof(unsigned char);
    	memset(buf, 0, N);
    	R  r(buf, N);

    	string	payload = "0F1F2F3F";
    	r.data = payload;

    	unsigned char  expected[] = {0x0F, 0x1F, 0x2F, 0x3F};
    	for (size_t k=0; k<N; ++k)
    		CPPUNIT_ASSERT_EQUAL(expected[k], buf[k]);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION( Blob_Test );
