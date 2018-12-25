/*
 * MotifWrapper.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: ori
 */

#include "MotifWrapper.h"

void BoostDefMotif() {
	def("motif",MotifCalculatorWrapper);
}

void addDictToList(py::list& dict_list,std::map<unsigned int, unsigned int>* map){
	py::dict d;
	auto& mapRef = *map;
	for(auto const& x:mapRef){
		d[x.first] = x.second;
	}
	dict_list.append(d);
}

py::list convertMapVectorToDictList(vector<std::map<unsigned int, unsigned int>*>* vec){
	py::list dict_list;
	for(int i=0;i<vec->size();i++){
		addDictToList(dict_list,vec->at(i));
	}
	return dict_list;
}

py::list MotifCalculatorWrapper(dict converted_dict,int level) {
	bool directed = extract<bool>(converted_dict["directed"]);
	std::cout << directed <<std::endl;
	ConvertedGNXReciever reciever(converted_dict);
	MotifCalculator calc(level,directed);
	calc.setGraph(reciever.getCacheGraph());
	vector<std::map<unsigned int, unsigned int>*>* res = calc.Calculate();
	py::list motif_counters = convertMapVectorToDictList(res);
	delete res;
	return motif_counters;

}
