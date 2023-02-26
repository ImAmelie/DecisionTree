#ifndef WDBC_H
#define WDBC_H

#include <vector>
#include <string>
#include <cmath>

namespace WDBC_NAMESPACE {

struct WDBC {
    float a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
          a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
          a21, a22, a23, a24, a25, a26, a27, a28, a29;
    char result;
    bool operator==(const WDBC &v) const {
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
               fabs(a12 - v.a12) < 0.0001 &&
               fabs(a13 - v.a13) < 0.0001 &&
               fabs(a14 - v.a14) < 0.0001 &&
               fabs(a15 - v.a15) < 0.0001 &&
               fabs(a16 - v.a16) < 0.0001 &&
               fabs(a17 - v.a17) < 0.0001 &&
               fabs(a18 - v.a18) < 0.0001 &&
               fabs(a19 - v.a19) < 0.0001 &&
               fabs(a20 - v.a20) < 0.0001 &&
               fabs(a21 - v.a21) < 0.0001 &&
               fabs(a22 - v.a22) < 0.0001 &&
               fabs(a23 - v.a23) < 0.0001 &&
               fabs(a24 - v.a24) < 0.0001 &&
               fabs(a25 - v.a25) < 0.0001 &&
               fabs(a26 - v.a26) < 0.0001 &&
               fabs(a27 - v.a27) < 0.0001 &&
               fabs(a28 - v.a28) < 0.0001 &&
               fabs(a29 - v.a29) < 0.0001;
    }
};

float &getA0(const WDBC &v);
float &getA1(const WDBC &v);
float &getA2(const WDBC &v);
float &getA3(const WDBC &v);
float &getA4(const WDBC &v);
float &getA5(const WDBC &v);
float &getA6(const WDBC &v);
float &getA7(const WDBC &v);
float &getA8(const WDBC &v);
float &getA9(const WDBC &v);
float &getA10(const WDBC &v);
float &getA11(const WDBC &v);
float &getA12(const WDBC &v);
float &getA13(const WDBC &v);
float &getA14(const WDBC &v);
float &getA15(const WDBC &v);
float &getA16(const WDBC &v);
float &getA17(const WDBC &v);
float &getA18(const WDBC &v);
float &getA19(const WDBC &v);
float &getA20(const WDBC &v);
float &getA21(const WDBC &v);
float &getA22(const WDBC &v);
float &getA23(const WDBC &v);
float &getA24(const WDBC &v);
float &getA25(const WDBC &v);
float &getA26(const WDBC &v);
float &getA27(const WDBC &v);
float &getA28(const WDBC &v);
float &getA29(const WDBC &v);
char &getResult(const WDBC &v);

std::vector<WDBC> load(const std::string &path);

} // namespace WDBC_NAMESPACE

#endif
