/*
 * AttractionBasinCalculator.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: ori
 */

#include "../AttractionBasinCalculator.h"

AttractionBasinCalculator::AttractionBasinCalculator(int alpha) :
		alpha(alpha), ab_in_dist(NULL), ab_out_dist(NULL), average_out_per_dist(
		NULL), average_in_per_dist(NULL),features(NULL) {
}

std::vector<double>* AttractionBasinCalculator::Calulate() {


	return this->features;
}

void AttractionBasinCalculator::calc_attraction_basin_dists() {
}

void AttractionBasinCalculator::calc_average_per_dist() {
}

AttractionBasinCalculator::~AttractionBasinCalculator() {
	// TODO Auto-generated destructor stub

	delete ab_in_dist;
	delete ab_out_dist;

	delete average_out_per_dist;
	delete average_in_per_dist;

}
