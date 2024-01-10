#include <climits>
#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

// 简单的C++函数定义, 包含默认参数, 重载
int add(int i, int j = 1) { return i + j; }
float add(float a, float b) { return a + b - 1.0f; }

// class定义,后续可以学习继承以及静态变量与方法的写法
class Pet {
  std::string name;

public:
  std::string _name;
  const std::string readOn = "efr3r";
  static char Char;
  static const char Char1;
  Pet(const std::string &str) : name(str){};
  const std::string &getName() const { return name; }
  void setName(std::string str) { name = str; }
};

char Pet::Char = 1;
const char Pet::Char1 = 'a';
//

PYBIND11_MODULE(example, module) {
  module.doc() = "pybind11 example plugin"; // optional module docstring
  module.def("add", (float (*)(float, float))(&add),
             "A function that adds two float numbers", py::arg("i"),
             py::arg("j"));
  module.def("add", (int (*)(int, int))(&add),
             "A function that adds two numbers", py::arg("i"),
             py::arg("j") = 1);
  py::class_<Pet>(module, "Pet",
                  py::dynamic_attr()) // 这里是为了跟python一样能够使用静态特性
      .def(py::init<const std::string &>())
      .def("setName", &Pet::setName)
      .def("getName", &Pet::getName)
      .def_readwrite("_name", &Pet::_name)       // 暴露public fileds _name
      .def_readonly("ReadOn", &Pet::readOn)      //暴露const fileds
      .def_readwrite_static("Char", &Pet::Char)  // 可读可写的静态变量
      .def_readonly_static("Char1", &Pet::Char1) // 可读不可写的静态变量
      .def("__repr__", [](const Pet &a) {
        return "<example.Pet named '>" + a.getName() +
               " char: " + std::to_string(a.Char) + " char1: " + a.Char1;
      });
}