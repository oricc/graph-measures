/*
 * ExampleFeatureCalculator.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: ori
 */

#include "../includes/ExampleFeatureCalculator.h"

ExampleFeatureCalculator::ExampleFeatureCalculator() {
	// If there is any additional preprocessing you would like to perform,
	// this is the place for it

}

/**
 * The actual calculation goes here
 */
float ExampleFeatureCalculator::Calculate() {
	return (float) mGraph->GetNumberOfNodes();
}

ExampleFeatureCalculator::~ExampleFeatureCalculator() {
	// Don't forget to release any memory you may have allocated!
}

