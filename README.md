OverlayRecord
=============

This C++ library defines one primary class that is intended to be used as an overlay of binary (file) record oriented data.

Installation
============

Download
--------

Either clone this GitHub repo or just click "Download ZIP" (see the right margin).


Prerequisites
-------------

In order to compile and execute the unit-tests the following requirements are needed.

*	A C++11 decent compliant C++ compiler, such as GCC C++ version 4.8.x
*	C++ library CppUnit installed

Verify the GCC version is sufficent

	g++ --version
	
It should output something similar to

	g++ (GCC) 4.8.2 20140120 (Red Hat 4.8.2-15)

You can install CppUnit, with a command similar to

	sudo apt-get install cppunit   # Ubuntu
	sudo yum install cppunit   # CentOS / RedHat

Compilation
-----------

This library is using [Gradle](www.gradle.org) and its wrapper as the build tool. Thta means, there is no need to install yet another tool. just run the provided `gradlew` shell-script. 

The library consists of only one HPP file, named `Record.hpp` and can be found in [src/main/incl](./src/main/incl/)

### Compile the unit-tests

Run the following command

	./gradlew assemble

### Execute the unit-tests

Run the following command, which both builds and runs the test suite

	./gradlew test

Alternatively, you can run the executable directly

	./build/unit-tests

You can find all unit test sources in [src/unit-test/cpp](./src/unit-test/cpp/)


Usage
=====

Simple record
-------------

	struct R : public overlay_record::Record {
		Text<4>           txt  = {this};
		TextInteger<4>    num1 = {this};
		TextFloat<6>      num2 = {this};
		Integer           val1 = {this};
		Double            val2 = {this};
		
		R() { allocateDynamicBuffer(); }
	};
	
	R    r;
	r.txt = "abcd";
	r.num1 = 42;
	r.num2 = 3.14;
	r.val1 = 420;
	r.val2 = 31.415;

Overlapping fields
------------------

	struct R : public overlay_record::Record {
		Text<12>         all = {this};
	
	    Text<6>         half1 = {this, all}; //align with filed 'all'
	    Text<6>         half2 = {this};
	    
	    R() { allocateDynamicBuffer(); }
	};
	
	R    r;
	r.txt = "aaaabbbbccccdddd";












