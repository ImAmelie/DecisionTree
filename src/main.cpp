#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include "ai.hpp"
#include "raisin.h"
#include "wdbc.h"
#include "iris.h"
#include "wine.h"
using namespace std;

void raisin() {
    clock_t start, end;

    Model<RAISIN_NAMESPACE::Raisin, string, RAISIN_NAMESPACE::getType, 7, float(*)(const RAISIN_NAMESPACE::Raisin &v)> model;
    model.getFun.push_back(RAISIN_NAMESPACE::getArea);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getMajorAxisLength);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getMinorAxisLength);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getEccentricity);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getConvexArea);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getExtent);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(RAISIN_NAMESPACE::getPerimeter);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);

    std::vector<RAISIN_NAMESPACE::Raisin> trainSet = RAISIN_NAMESPACE::load("../data/Raisin_train.data");
    start = clock();
    model.train(trainSet);
    end = clock();
    trainSet.clear();

    cout << "Raisin:" << endl;
    cout << std::fixed << std::setprecision(2)
        << "训练用时: "
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl;

    std::vector<RAISIN_NAMESPACE::Raisin> testSet = RAISIN_NAMESPACE::load("../data/Raisin_test.data");
    start = clock();
    float accuracy = model.test(testSet);
    end = clock();
    testSet.clear();

    cout << std::fixed << std::setprecision(2)
        << "测试用时："
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl
        << "测试结果准确率: "
        << accuracy * 100
        << "%"
        << endl;
}

void wdbc() {
    clock_t start, end;

    Model<WDBC_NAMESPACE::WDBC, char, WDBC_NAMESPACE::getResult, 30, float(*)(const WDBC_NAMESPACE::WDBC &v)> model;
    model.getFun.push_back(WDBC_NAMESPACE::getA0);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA1);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA2);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA3);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA4);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA5);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA6);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA7);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA8);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA9);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA10);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA11);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA12);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA13);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA14);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA15);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA16);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA17);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA18);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA19);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA20);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA21);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA22);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA23);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA24);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA25);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA26);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA27);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA28);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WDBC_NAMESPACE::getA29);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);

    std::vector<WDBC_NAMESPACE::WDBC> trainSet = WDBC_NAMESPACE::load("../data/wdbc_train.data");
    start = clock();
    model.train(trainSet);
    end = clock();
    trainSet.clear();

    cout << "WDBC:" << endl;
    cout << std::fixed << std::setprecision(2)
        << "训练用时: "
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl;

    std::vector<WDBC_NAMESPACE::WDBC> testSet = WDBC_NAMESPACE::load("../data/wdbc_test.data");
    start = clock();
    float accuracy = model.test(testSet);
    end = clock();
    testSet.clear();

    cout << std::fixed << std::setprecision(2)
        << "测试用时："
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl
        << "测试结果准确率: "
        << accuracy * 100
        << "%"
        << endl;
}

void iris() {
    clock_t start, end;

    Model<IRIS_NAMESPACE::Iris, string, IRIS_NAMESPACE::getType, 4, float(*)(const IRIS_NAMESPACE::Iris &v)> model;
    model.getFun.push_back(IRIS_NAMESPACE::getA0);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(IRIS_NAMESPACE::getA1);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(IRIS_NAMESPACE::getA2);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(IRIS_NAMESPACE::getA3);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);

    cout << "Iris:" << endl;
    std::vector<IRIS_NAMESPACE::Iris> trainSet = IRIS_NAMESPACE::load("../data/iris_train.data");
    start = clock();
    model.train(trainSet);
    end = clock();
    trainSet.clear();

    cout << std::fixed << std::setprecision(2)
        << "训练用时: "
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl;

    std::vector<IRIS_NAMESPACE::Iris> testSet = IRIS_NAMESPACE::load("../data/iris_test.data");
    start = clock();
    float accuracy = model.test(testSet);
    end = clock();
    testSet.clear();

    cout << std::fixed << std::setprecision(2)
        << "测试用时："
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl
        << "测试结果准确率: "
        << accuracy * 100
        << "%"
        << endl;
}

void wine() {
    clock_t start, end;

    Model<WINE_NAMESPACE::Wine, int, WINE_NAMESPACE::getResult, 13, float(*)(const WINE_NAMESPACE::Wine &v)> model;
    model.getFun.push_back(WINE_NAMESPACE::getA0);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA1);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA2);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA3);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA4);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA5);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA6);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA7);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA8);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA9);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA10);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA11);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);
    model.getFun.push_back(WINE_NAMESPACE::getA12);
    model.argTypes.push_back(decltype(model)::BestArgType::Continuous);

    std::vector<WINE_NAMESPACE::Wine> trainSet = WINE_NAMESPACE::load("../data/wine_train.data");
    start = clock();
    model.train(trainSet);
    end = clock();
    trainSet.clear();

    cout << "Wine:" << endl;
    cout << std::fixed << std::setprecision(2)
        << "训练用时: "
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl;

    std::vector<WINE_NAMESPACE::Wine> testSet = WINE_NAMESPACE::load("../data/wine_test.data");
    start = clock();
    float accuracy = model.test(testSet);
    end = clock();
    testSet.clear();

    cout << std::fixed << std::setprecision(2)
        << "测试用时："
        << float(end - start) / CLOCKS_PER_SEC
        << " s"
        << endl
        << "测试结果准确率: "
        << accuracy * 100
        << "%"
        << endl;
}

int main(int argc, char *argv[]) {
    raisin();
    wdbc();
    iris();
    wine();

    return 0;
}
