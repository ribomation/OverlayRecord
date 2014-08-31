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
*	Doxygen HTML docs generator (optional)

Verify the GCC version is sufficent

	g++ --version
	
It should output something similar to

	g++ (GCC) 4.8.2 20140120 (Red Hat 4.8.2-15)

You can install CppUnit, with a command similar to

	sudo apt-get install cppunit   # Ubuntu
	sudo yum install cppunit   # CentOS / RedHat

Build
-----

This library is using [Gradle](http://www.gradle.org) and its wrapper as the build tool. That means, there is no need to install yet another tool. Just run the provided `gradlew` shell-script it will do the rest. 

The library consists of only one HPP file, named `Record.hpp` and can be found in [src/main/incl](./src/main/incl/)

### Compile the unit-tests

Run the following command to compile all unit-tests in [src/unit-test/cpp](./src/unit-test/cpp/)

	./gradlew assemble


### Execute the unit-tests

Run the following command, which (both builds and) runs the test suite

	./gradlew test

Alternatively, you can run the executable directly

	./build/unit-tests

### Generate docs

Run the following command to generate doxygen HTMl docs in `build/docs/html`

	./gradlew docs

The view the HTML docs using a command similar to

	firefox ./build/docs/html/index.html

If you don't have doxygen installed, you can install it with a command similar to

	sudo apt-get install doxygen  # Ubuntu
	sudo yum install doxygen      # CentOS/RedHat

Class Overview
============

Class Record
--------

An overlaid record is defined as a subclass of class Record, having one or more field members. A field can be scalar or composite by an array of scalars or by an embedded record.

	#include "Record.hpp"
	using namespace overlay_record;
	struct R : public Record {
		Text<4>            txt  = {this};
		TextInteger<5>     num  = {this};
		Integer            val  = {this};
		Array<Text<3>, 4>  arr  = {this};
		Embed<X>           xrec = {this};
		Blob<36>           all  = {this, txt}; //overlay from txt
		
		R() { allocateDynamicBuffer(); }		
		R(char* buf, unsigned bufsiz) { assignStaticBuffer(buf, bufsiz); }
	};
	
	R  r;
	r.txt = "abcd";
	r.val = 42;
	r.num = 10 * r.val;
	r.arr[1] = "xyz";
	r.xrec->some_field = "qwerty";

Backing storage
-----------

Before a record can be used it must have a backing storage defined. There are two alternatives provided:

*  `assignStaticBuffer(buffer, size)` - Use the storage provided in buffer. The `size` value if checked so it at least fits within the record's size.
*  `allocateDynamicBuffer` - Use heap allocated storage.

The proper place for invocation of any of these methods is in the subclass' constructor, although it possible to invoke on an existing record object.


Class Field
--------

A field type is a template instantion of class Field. There is a set of pre-defined field type aliases listed in the table below, but it also easy to define new a field type using a proper template instantiation of an application *type*, a *size* and a *converter*. Read more in the [Architecture](#architecture) section.

Pre-defined field type aliases
---------

Field Type Alias | Application Type | Description
---------------- | ---------------- | -----------
`Text<N>` | `std::string` | Sequence of N ASCII characters
`TextInteger<N>` | `int` | Sequence of N ASCII characters representing an integral number
`TextFloat<N>` | `float` | Sequence of N ASCII characters representing a floating-point number
`Integer` | `int` | Binary representaiton of an int
`Float` | `float` | Binary representation of a float
`Blob<N>` | `std::string` | Sequence of N binary bytes, which are converted into a HEX string when accessed

Value access
----------

A field is read using the its method `value()` and written using the overloaded method `value(ValueType v)`.

	MyRecord  r;
	r.aTextField.value("Hello");
	std::string  s = r.aTextField.value();
	r.anIntField.value(42);
	int  n = r.anIntField.value();

For convenience, a type-conversion operator and an assignment operator is defined, using the value() functions.

	MyRecord  r;
	r.aTextField.value = "Hello";
	std::string  s = r.aTextField;
	r.anIntField.value = 42;
	int  n = r.anIntField;

N.B. depending on the syntactic context, the compiler might sometimes *not* be able to figure out the proper type. In that case, just revert back to the using the `value()` methods above.

Array field
----------


Embedded record
---------


Code examples
=====

 


Simple record
-------------

	#include "Record.hpp"
	using namespace overlay_record;
	
	struct R : public Record {
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

	struct R : public Record {
		Text<12>         all = {this};
	
	    Text<6>         half1 = {this, all}; //align start with field 'all'
	    Text<6>         half2 = {this};
	    
	    Text<3>         quarter3 = {this, half2}; //align with start of half2
	    Text<3>         quarter4 = {this};
	    
	    R() { allocateDynamicBuffer(); }
	};
	
	R    r;
	r.txt = "aaaabbbbccccdddd";
	assert(12U, r.size());


Array field 
--------------


Embedded record field
-----------


Sliding record
--------------


Record stream I/O
------------



Architecture
============


A subclass of class Record, has one ore more members of template class Field. Field depends on three template parameters and is defined as

	template<typename ValueType, unsigned Size, typename Converter> Field {. . .};

`ValueTYpe` is the type used in the application, i.e. from and to the underlying buffer (backing storage) of a record. The `Size` defines the number of bytes the field spans.  










 


