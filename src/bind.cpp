#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include "fast_bpe.h"

class MyClass {
public:
    MyClass(int value) : value_(value) {}
    int get_value() const { return value_; }
    void set_value(int value) { value_ = value; }
private:
    int value_;
};

namespace py = pybind11;

PYBIND11_MODULE(_Tokenizer, m) {
    py::class_<Tokenizer>(m, "Tokenizer")
        .def(py::init<std::string>())
        .def("encode", &Tokenizer::encode);
}