#ifndef RAISIN_H
#define RAISIN_H

#include <vector>
#include <string>
#include <cmath>

namespace RAISIN_NAMESPACE {

struct Raisin {
    float Area; // int
    float MajorAxisLength;
    float MinorAxisLength;
    float Eccentricity;
    float ConvexArea; // int
    float Extent;
    float Perimeter;
    std::string type;
    bool operator==(const Raisin &v) const {
        return fabs(Area - v.Area) < 0.0001 &&
               fabs(MajorAxisLength - v.MajorAxisLength) < 0.0001 &&
               fabs(MinorAxisLength - v.MinorAxisLength) < 0.0001 &&
               fabs(Eccentricity - v.Eccentricity) < 0.0001 &&
               fabs(ConvexArea - v.ConvexArea) < 0.0001 &&
               fabs(Extent - v.Extent) < 0.0001 &&
               fabs(Perimeter - v.Perimeter) < 0.0001 &&
               type == v.type;
    }
};

float getArea(const Raisin &v);
float getMajorAxisLength(const Raisin &v);
float getMinorAxisLength(const Raisin &v);
float getEccentricity(const Raisin &v);
float getConvexArea(const Raisin &v);
float getExtent(const Raisin &v);
float getPerimeter(const Raisin &v);
std::string getType(const Raisin &v);

std::vector<Raisin> load(const std::string &path);

} // namespace RAISIN_NAMESPACE

#endif
