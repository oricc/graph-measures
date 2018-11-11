/*
 * ExampleWrapper.h
 *
 *  Created on: Nov 11, 2018
 *      Author: ori
 */

#ifndef WRAPPERS_EXAMPLEWRAPPER_H_
#define WRAPPERS_EXAMPLEWRAPPER_H_


#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include "../includes/ConvertedGNXReciever.h"
#include "../includes/ExampleFeatureCalculator.h"
#include "../includes/FeatureCalculator.h"


using namespace boost::python;
void BoostDefExampleCalculator();
void ExampleCalculatorWrapper(dict converted_graph);


#endif /* WRAPPERS_EXAMPLEWRAPPER_H_ */
