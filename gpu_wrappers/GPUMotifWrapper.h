/*
 * ExampleWrapper.h
 *
 *  Created on: Nov 11, 2018
 *      Author: ori
 */

#ifndef WRAPPERS_GPU_MOTIF_WRAPPER_H_
#define WRAPPERS_GPU_MOTIF_WRAPPER_H_

#include "../wrappers/WrapperIncludes.h"
#include "../includes/GPUMotifCalculator.h"

void BoostDefGPUMotifCalculator();
float GPUMotifCalculatorWrapper(dict converted_graph);


#endif /* WRAPPERS_EXAMPLEWRAPPER_H_ */
