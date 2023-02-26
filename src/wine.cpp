#include "wine.h"

#include <fstream>
#include <vector>

#include <iostream> // delete

namespace WINE_NAMESPACE {

float getA0(const Wine&v) { return v.a0; }
float getA1(const Wine&v) { return v.a1; }
float getA2(const Wine&v) { return v.a2; }
float getA3(const Wine&v) { return v.a3; }
float getA4(const Wine&v) { return v.a4; }
float getA5(const Wine&v) { return v.a5; }
float getA6(const Wine&v) { return v.a6; }
float getA7(const Wine&v) { return v.a7; }
float getA8(const Wine&v) { return v.a8; }
float getA9(const Wine&v) { return v.a9; }
float getA10(const Wine &v) { return v.a10; }
float getA11(const Wine &v) { return v.a11; }
float getA12(const Wine &v) { return v.a12; }
int getResult(const Wine &v) { return v.result; }

std::vector<Wine> load(const std::string &path) {
    std::ifstream i;
    i.open(path, std::ios_base::in);
    if (!i.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    std::vector<Wine> set;
    float a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
          a11, a12;
    int type;
    char buffer[1024];
    while (!i.eof()) {
        i.getline(buffer, sizeof(buffer));
        std::sscanf(buffer, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                    &type,
                    &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10,
                    &a11, &a12);
        Wine tmp = {
            a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
            a11, a12,
            type
        };
        set.push_back(tmp);
    }
    i.close();

    return set;
}

} // namespace WINE_NAMESPACE
