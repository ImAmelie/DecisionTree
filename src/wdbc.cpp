#include "wdbc.h"

#include <fstream>
#include <vector>

namespace WDBC_NAMESPACE {

float getA0(const WDBC &v) { return v.a0; }
float getA1(const WDBC &v) { return v.a1; }
float getA2(const WDBC &v) { return v.a2; }
float getA3(const WDBC &v) { return v.a3; }
float getA4(const WDBC &v) { return v.a4; }
float getA5(const WDBC &v) { return v.a5; }
float getA6(const WDBC &v) { return v.a6; }
float getA7(const WDBC &v) { return v.a7; }
float getA8(const WDBC &v) { return v.a8; }
float getA9(const WDBC &v) { return v.a9; }
float getA10(const WDBC &v) { return v.a10; }
float getA11(const WDBC &v) { return v.a11; }
float getA12(const WDBC &v) { return v.a12; }
float getA13(const WDBC &v) { return v.a13; }
float getA14(const WDBC &v) { return v.a14; }
float getA15(const WDBC &v) { return v.a15; }
float getA16(const WDBC &v) { return v.a16; }
float getA17(const WDBC &v) { return v.a17; }
float getA18(const WDBC &v) { return v.a18; }
float getA19(const WDBC &v) { return v.a19; }
float getA20(const WDBC &v) { return v.a20; }
float getA21(const WDBC &v) { return v.a21; }
float getA22(const WDBC &v) { return v.a22; }
float getA23(const WDBC &v) { return v.a23; }
float getA24(const WDBC &v) { return v.a24; }
float getA25(const WDBC &v) { return v.a25; }
float getA26(const WDBC &v) { return v.a26; }
float getA27(const WDBC &v) { return v.a27; }
float getA28(const WDBC &v) { return v.a28; }
float getA29(const WDBC &v) { return v.a29; }
char getResult(const WDBC &v) { return v.result; }

std::vector<WDBC> load(const std::string &path) {
    std::ifstream i;
    i.open(path, std::ios_base::in);
    if (!i.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    std::vector<WDBC> set;
    float a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
          a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
          a21, a22, a23, a24, a25, a26, a27, a28, a29;
    char type;
    char buffer[1024];
    while (!i.eof()) {
        i.getline(buffer, sizeof(buffer));
        std::sscanf(buffer, "%*f,%c,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                    &type,
                    &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10,
                    &a11, &a12, &a13, &a14, &a15, &a16, &a17, &a18, &a19, &a20,
                    &a21, &a22, &a23, &a24, &a25, &a26, &a27, &a28, &a29);
        WDBC tmp = {
            a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
            a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
            a21, a22, a23, a24, a25, a26, a27, a28, a29,
            type
        };
        set.push_back(tmp);
    }
    i.close();

    return set;
}

} // namespace WDBC_NAMESPACE
