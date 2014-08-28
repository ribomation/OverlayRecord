
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main() {
    CppUnit::TestFactoryRegistry&	registry = CppUnit::TestFactoryRegistry::getRegistry();
    CppUnit::TextUi::TestRunner 	runner;
    runner.addTest( registry.makeTest() );
    
    bool wasSucessful = runner.run();    
    return wasSucessful ? 0 : 1;
}
