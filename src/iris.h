#ifndef IRIS_H
#define IRIS_H

#include <vector>
#include <string>
#include <cmath>

namespace IRIS_NAMESPACE {

struct Iris {
    float a0, a1, a2, a3;
    std::string type;
    bool operator==(const Iris &v) const {
        return fabs(a0 - v.a0) < 0.0001 &&
               fabs(a1 - v.a1) < 0.0001 &&
               fabs(a2 - v.a2) < 0.0001 &&
               fabs(a3 - v.a3) < 0.0001;
    }
};

float &getA0(const Iris &v);
float &getA1(const Iris &v);
float &getA2(const Iris &v);
float &getA3(const Iris &v);

std::string &getType(const Iris &v);

std::vector<Iris> load(const std::string &path);

} // namespace IRIS_NAMESPACE

#endif
