#include "iris.h"

#include <fstream>
#include <vector>

namespace IRIS_NAMESPACE {

float &getA0(const Iris &v) { return ((Iris &)v).a0; }
float &getA1(const Iris &v) { return ((Iris &)v).a1; }
float &getA2(const Iris &v) { return ((Iris &)v).a2; }
float &getA3(const Iris &v) { return ((Iris &)v).a3; }

std::string &getType(const Iris &v) { return ((Iris &)v).type; }

std::vector<Iris> load(const std::string &path) {
    std::ifstream i;
    i.open(path, std::ios_base::in);
    if (!i.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    std::vector<Iris> set;
    float a0, a1, a2, a3;
    char type[64];
    char buffer[1024];
    while (!i.eof()) {
        i.getline(buffer, sizeof(buffer));
        std::sscanf(buffer, "%f,%f,%f,%f,%s", &a0, &a1, &a2, &a3, type);
        Iris tmp = { a0, a1, a2, a3, std::string(type) };
        set.push_back(tmp);
    }
    i.close();

    return set;
}

} // namespace IRIS_NAMESPACE
