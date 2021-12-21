/**
 * @file py_maxclique.cpp
 * @brief Python bindings for maximum clique solver
 * @author Parker Lusk <plusk@mit.edu>
 * @date 20 December 2021
 */

#include <cstdint>
#include <sstream>

#include <Eigen/Dense>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include "maxclique/maxclique.h"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(maxclique, m)
{
  m.doc() = "Wrapper for maximum clique solver";
  m.attr("__version__") = PROJECT_VERSION;

  py::enum_<maxclique::Method>(m, "Method")
    .value("EXACT", maxclique::Method::EXACT)
    .value("HEU", maxclique::Method::HEU)
    .value("KCORE", maxclique::Method::KCORE);

  py::class_<maxclique::Params>(m, "Params")
    .def(py::init<>())
    .def("__repr__", [](const maxclique::Params &params) {
       std::ostringstream repr;
       repr << "<Parameters for maximum clique solver>";
       return repr.str();
    })
    .def_readwrite("method", &maxclique::Params::method)
    .def_readwrite("threads", &maxclique::Params::threads)
    .def_readwrite("time_limit", &maxclique::Params::time_limit)
    .def_readwrite("verbose", &maxclique::Params::verbose);

  m.def("solve", &maxclique::solve,
    "A"_a.noconvert(), "params"_a = maxclique::Params{});
}