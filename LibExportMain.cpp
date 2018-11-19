/*
 * LibExportMain.cpp
 *
 *  Created on: Nov 11, 2018
 *      Author: ori
 */

#include <stdio.h>
#include <boost/python.hpp>

#include "wrappers/ClusteringCoefficientCalculatorWrapper.h"
#include "wrappers/ExampleWrapper.h"
#include "wrappers/KCoreWrapper.h"
#include "wrappers/NodePageRankWrapper.h"
#include "wrappers/BFSMomentsWrapper.h"
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
	BoostDefKCore();
	BoostDefNodePageRank();
	BoostDefClusteringCoefficient();
	BoostDefBFSMoments();
}

