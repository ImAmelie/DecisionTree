#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "ai.hpp"
#include "raisin.h"
#include "wdbc.h"
using namespace std;

void raisin() {
    clock_t start, end;

    Model<RAISIN_NAMESPACE::Raisin, string, RAISIN_NAMESPACE::getType, 7, float(*)(const RAISIN_NAMESPACE::Raisin &v)> model;
    model.getFun.push_back(RAISIN_NAMESPACE::getArea);
    model.getFun.push_back(RAISIN_NAMESPACE::getMajorAxisLength);
    model.getFun.push_back(RAISIN_NAMESPACE::getMinorAxisLength);
    model.getFun.push_back(RAISIN_NAMESPACE::getEccentricity);
    model.getFun.push_back(RAISIN_NAMESPACE::getConvexArea);
    model.getFun.push_back(RAISIN_NAMESPACE::getExtent);
    model.getFun.push_back(RAISIN_NAMESPACE::getPerimeter);

    std::vector<RAISIN_NAMESPACE::Raisin> trainSet = RAISIN_NAMESPACE::load("../data/Raisin_train.data");
    start = clock();
    model.train(trainSet, nullptr);
    end = clock();
    trainSet.clear();

    cout << std::fixed << std::setprecision(2)
         << "训练用时: "
         << float(end - start) / CLOCKS_PER_SEC
         << " s"
         << endl;

     std::vector<RAISIN_NAMESPACE::Raisin> testSet = RAISIN_NAMESPACE::load("../data/Raisin_test.data");
     float accuracy = model.test(testSet);
     testSet.clear();

     cout << std::fixed << std::setprecision(2)
          << "测试结果准确率: "
          << accuracy * 100
          << "%"
          << endl;
}

void wdbc() {
    clock_t start, end;

    Model<WDBC_NAMESPACE::WDBC, char, WDBC_NAMESPACE::getResult, 30, float(*)(const WDBC_NAMESPACE::WDBC &v)> model;
    model.getFun.push_back(WDBC_NAMESPACE::getA0);
    model.getFun.push_back(WDBC_NAMESPACE::getA1);
    model.getFun.push_back(WDBC_NAMESPACE::getA2);
    model.getFun.push_back(WDBC_NAMESPACE::getA3);
    model.getFun.push_back(WDBC_NAMESPACE::getA4);
    model.getFun.push_back(WDBC_NAMESPACE::getA5);
    model.getFun.push_back(WDBC_NAMESPACE::getA6);
    model.getFun.push_back(WDBC_NAMESPACE::getA7);
    model.getFun.push_back(WDBC_NAMESPACE::getA8);
    model.getFun.push_back(WDBC_NAMESPACE::getA9);
    model.getFun.push_back(WDBC_NAMESPACE::getA10);
    model.getFun.push_back(WDBC_NAMESPACE::getA11);
    model.getFun.push_back(WDBC_NAMESPACE::getA12);
    model.getFun.push_back(WDBC_NAMESPACE::getA13);
    model.getFun.push_back(WDBC_NAMESPACE::getA14);
    model.getFun.push_back(WDBC_NAMESPACE::getA15);
    model.getFun.push_back(WDBC_NAMESPACE::getA16);
    model.getFun.push_back(WDBC_NAMESPACE::getA17);
    model.getFun.push_back(WDBC_NAMESPACE::getA18);
    model.getFun.push_back(WDBC_NAMESPACE::getA19);
    model.getFun.push_back(WDBC_NAMESPACE::getA20);
    model.getFun.push_back(WDBC_NAMESPACE::getA21);
    model.getFun.push_back(WDBC_NAMESPACE::getA22);
    model.getFun.push_back(WDBC_NAMESPACE::getA23);
    model.getFun.push_back(WDBC_NAMESPACE::getA24);
    model.getFun.push_back(WDBC_NAMESPACE::getA25);
    model.getFun.push_back(WDBC_NAMESPACE::getA26);
    model.getFun.push_back(WDBC_NAMESPACE::getA27);
    model.getFun.push_back(WDBC_NAMESPACE::getA28);
    model.getFun.push_back(WDBC_NAMESPACE::getA29);

    std::vector<WDBC_NAMESPACE::WDBC> trainSet = WDBC_NAMESPACE::load("../data/wdbc_train.data");
    start = clock();
    model.train(trainSet, nullptr);
    end = clock();
    trainSet.clear();

    cout << std::fixed << std::setprecision(2)
         << "训练用时: "
         << float(end - start) / CLOCKS_PER_SEC
         << " s"
         << endl;

     std::vector<WDBC_NAMESPACE::WDBC> testSet = WDBC_NAMESPACE::load("../data/wdbc_test.data");
     float accuracy = model.test(testSet);
     testSet.clear();

     cout << std::fixed << std::setprecision(2)
          << "测试结果准确率: "
          << accuracy * 100
          << "%"
          << endl;
}

int main(int, char **) {
    wdbc();

    return 0;
}
