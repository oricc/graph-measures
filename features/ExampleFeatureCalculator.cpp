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
 * For the example feature, we would just like to know that the graph has been passed down correctly into a CacheGraph instance,
 * and so we print out the nodes and neighbor lists as a validation.
 * This also has the added benefit of making sure that our standard code templates can run without errors.
 */
float ExampleFeatureCalculator::Calculate() {
	return (float) mGraph->GetNumberOfNodes();
}

ExampleFeatureCalculator::~ExampleFeatureCalculator() {
	// Don't forget to release any memory you may have allocated!
}

