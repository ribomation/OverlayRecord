

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


int main() {
    CppUnit::Test*  suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite );
    bool wasSucessful = runner.run();    

    return wasSucessful ? 0 : 1;
}

