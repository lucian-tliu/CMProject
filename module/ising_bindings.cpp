#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "isingspins.hpp"
#include "flip_algorithm.hpp"

namespace py = pybind11;

static std::vector<double> ndarray_to_vector(const py::array_t<double>& arr) {
    py::buffer_info buf = arr.request();
    if (buf.ndim != 1)
        throw std::runtime_error("data must be a 1D numpy array");
    const double* ptr = static_cast<const double*>(buf.ptr);
    return std::vector<double>(ptr, ptr + buf.shape[0]);
}

py::array_t<double> vector_to_ndarray(const std::vector<double>& vec) {
    py::array_t<double> arr(vec.size());

    auto buf = arr.mutable_unchecked<1>();

    for (size_t i = 0; i < vec.size(); i++) {
        buf(i) = vec[i];
    }
    return arr;
}

PYBIND11_MODULE(ising_model, m) {
    py::class_<IsingSpins>(m, "IsingSpins")
        .def(py::init<size_t, size_t, double, std::string, int>(), 
             py::arg("rows"), py::arg("cols"), py::arg("J"), py::arg("init_mode") = "hot", py::arg("seed") = -1)
        .def("getJ", &IsingSpins::getJ)
        .def("meanEnergy", &IsingSpins::meanEnergy)
        .def("meanMagnetization", &IsingSpins::meanMagnetization)
        .def("flipSpins", &IsingSpins::flipSpins)
        .def("to_numpy", [](IsingSpins& spins) -> py::array_t<int> {
            size_t rows = spins.getNumRows();
            size_t cols = spins.getNumCols();

            auto arr = py::array_t<int>({static_cast<py::ssize_t>(rows), 
                                        static_cast<py::ssize_t>(cols)});
            auto r = arr.mutable_unchecked<2>();
            
            #pragma omp parallel for simd
            for (size_t n = 0; n < rows*cols; n++) {
                size_t i = n / cols;
                size_t j = n % cols;
                r(i, j) = spins(i, j);
            }
            
            return arr;
        })
        .def("__str__", [](IsingSpins& spins) {
            std::cout << "mean energy: " << spins.meanEnergy() << std::endl;
            std::cout << "mean magnetization: " << spins.meanMagnetization() << std::endl;
            std::cout << spins << std::endl;
            return "";
        });

    m.doc() = "Ising model simulation bindings";

    m.def("metropolisStep", &metropolisStep, py::arg("s"), py::arg("kBT"));
    m.def("wolffStep", [&](IsingSpins& s, double kBT) {
        return wolffStep(s, kBT);
    }, py::arg("s"), py::arg("kBT"));
    m.def("multipleSteps", [](IsingSpins& s, double kBT, size_t n_steps, const std::string& algorithm, bool record, size_t lag) {
        auto result = multipleSteps(s, kBT, n_steps, algorithm, record, lag);
        if (result.has_value()) {
            auto [energies, magnetizations, mean_cluster_size] = result.value();
            return py::make_tuple(vector_to_ndarray(energies), vector_to_ndarray(magnetizations), mean_cluster_size);
        } else {
            return py::make_tuple(py::none(), py::none(), py::none());
        }
    }, py::arg("s"), py::arg("kBT"), py::arg("n_steps"), py::arg("algorithm"), py::arg("record") = false, py::arg("lag") = 1);
}