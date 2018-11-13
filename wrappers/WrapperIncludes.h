/*
 * WrapperIncludes.h
 *
 *  Created on: Nov 12, 2018
 *      Author: ori
 */

#ifndef WRAPPERS_WRAPPERINCLUDES_H_
#define WRAPPERS_WRAPPERINCLUDES_H_

#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include "../includes/ConvertedGNXReciever.h"
#include "../includes/FeatureCalculator.h"

#include <boost/python/list.hpp>
#include <vector>

using namespace boost::python;

namespace py = boost::python;

template<class T>
py::list std_vector_to_py_list(const std::vector<T>& v)
{
    py::object get_iter = py::iterator<std::vector<T> >();
    py::object iter = get_iter(v);
    py::list l(iter);
    return l;
}


#endif /* WRAPPERS_WRAPPERINCLUDES_H_ */
