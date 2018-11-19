/*
 * BFSMomentsWrapper.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: ori
 */

#include "BFSMomentsWrapper.h"

void BoostDefBFSMoments() {
	def("bfs_moments",BFSMomentWrapper);
}

py::list BFSMomentWrapper(dict converted_dict) {

	ConvertedGNXReciever reciever(converted_dict);
	BfsMomentsCalculator calc;
	calc.setGraph(reciever.getCacheGraph());
//	std::cout<<"After conversion"<<std::endl;
	std::vector<std::tuple<float, float>> resVec = calc.Calculate();
	return vectorToPythonList(resVec);

}
