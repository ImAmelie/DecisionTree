#ifndef WINE_H
#define WINE_H

#include <vector>
#include <string>
#include <cmath>

namespace WINE_NAMESPACE {

struct Wine {
    float a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
          a11, a12;
    int result;
    bool operator==(const Wine &v) const {
        return fabs(a0 - v.a0) < 0.0001 &&
               fabs(a1 - v.a1) < 0.0001 &&
               fabs(a2 - v.a2) < 0.0001 &&
               fabs(a3 - v.a3) < 0.0001 &&
               fabs(a4 - v.a4) < 0.0001 &&
               fabs(a5 - v.a5) < 0.0001 &&
               fabs(a6 - v.a6) < 0.0001 &&
               fabs(a7 - v.a7) < 0.0001 &&
               fabs(a8 - v.a8) < 0.0001 &&
               fabs(a9 - v.a9) < 0.0001 &&
               fabs(a10 - v.a10) < 0.0001 &&
               fabs(a11 - v.a11) < 0.0001 &&
               fabs(a12 - v.a12) < 0.0001;
    }
};

float &getA0(const Wine &v);
float &getA1(const Wine &v);
float &getA2(const Wine &v);
float &getA3(const Wine &v);
float &getA4(const Wine &v);
float &getA5(const Wine &v);
float &getA6(const Wine &v);
float &getA7(const Wine &v);
float &getA8(const Wine &v);
float &getA9(const Wine &v);
float &getA10(const Wine &v);
float &getA11(const Wine &v);
float &getA12(const Wine &v);
int &getResult(const Wine &v);

std::vector<Wine> load(const std::string &path);

} // namespace WINE_NAMESPACE

#endif
