/*
 * LibExportMain.cpp
 *
 *  Created on: Nov 11, 2018
 *      Author: ori
 */

#include <stdio.h>
#include <boost/python.hpp>
#include "wrappers/ExampleWrapper.h"
// ... other imports ...

/*
 * Check that exporting to Python works
 */
void test_export(){
	std::cout << "Hello Test!"<<std::endl;
}

BOOST_PYTHON_MODULE(_features)
{

	def("test",test_export);
// ... other boost def wrappers ...
	BoostDefExampleCalculator();
}

