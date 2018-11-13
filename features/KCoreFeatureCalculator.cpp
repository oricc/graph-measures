/*
 * KCoreFeatureCalculator.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: ori
 */

#include "../includes/KCoreFeatureCalculator.h"

KCoreFeatureCalculator::KCoreFeatureCalculator() {
	// TODO Auto-generated constructor stub

}

std::vector<unsigned short> KCoreFeatureCalculator::Calculate() {

	return mGraph->ComputeKCore();

}

KCoreFeatureCalculator::~KCoreFeatureCalculator() {
	// TODO Auto-generated destructor stub
}

