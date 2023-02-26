#include "raisin.h"

#include <fstream>
#include <string>

namespace RAISIN_NAMESPACE {

float &getArea(const Raisin &v) { return ((Raisin &)v).Area; }
float &getMajorAxisLength(const Raisin &v) { return ((Raisin &)v).MajorAxisLength; }
float &getMinorAxisLength(const Raisin &v) { return ((Raisin &)v).MinorAxisLength; }
float &getEccentricity(const Raisin &v) { return ((Raisin &)v).Eccentricity; }
float &getConvexArea(const Raisin &v) { return ((Raisin &)v).ConvexArea; }
float &getExtent(const Raisin &v) { return ((Raisin &)v).Extent; }
float &getPerimeter(const Raisin &v) { return ((Raisin &)v).Perimeter; }
std::string &getType(const Raisin &v) { return ((Raisin &)v).type; }

std::vector<Raisin> load(const std::string &path) {
    std::ifstream i;
    i.open(path, std::ios_base::in);
    if (!i.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    std::vector<Raisin> set;
    float a0, a1, a2, a3, a4, a5, a6;
    char type[64];
    char buffer[1024];
    while (!i.eof()) {
        i.getline(buffer, sizeof(buffer));
        std::sscanf(buffer, "%f,%f,%f,%f,%f,%f,%f,%s",
                    &a0, &a1, &a2, &a3, &a4, &a5, &a6, type);
        Raisin tmp = { a0, a1, a2, a3, a4, a5, a6, std::string(type) };
        set.push_back(tmp);
    }
    i.close();

    return set;
}

} // namespace RAISIN_NAMESPACE
