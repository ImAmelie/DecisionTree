#ifndef AI_H
#define AI_H

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <utility>
#include <stdexcept>
#include <fstream>

// 函数原型

// 返回set集合中样本数最多的类
template<typename T,typename RT, RT (*F)(const T &)>
RT mostType(const std::vector<T> &set);

// 类定义

/**
 * 决策树模型类
 * 模板参数：
 *   T: 数据集合中数据的类型
 *   RT: 数据的分类标签类型
 *   F: 指向 获取单个数据分类标签的函数 的指针
 *   ArgNum: 数据集合中数据的元素个数
 *   GetArgFun: 获取单个数据属性值的函数 的函数类型
 */
template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
class Model {
private:
    class Node {
        friend class Model<T, RT, F, ArgNum, GetArgFun>;
    private:
        enum Type { Leaf, Middle };
        Type type;
        // type is Middle
        std::pair<int, float> bestArg;
        std::vector<std::shared_ptr<Node>> subNode;
        // type is Leaf
        RT result;
    };
    std::vector<int> curArgs;
    std::shared_ptr<Node> root;

    // 工具成员函数
    inline std::pair<int, float> selectArg(const std::vector<T> &set, const std::vector<int> &curArgs);
    inline float Ent(const std::vector<T> &set);
    inline float Ent(const std::vector<std::pair<float, RT>> &set);

public:
    std::vector<GetArgFun> getFun;

    void train(const std::vector<T> &set, std::shared_ptr<Node> parent);
    float test(const std::vector<T> &set);
    RT result(const T &value);

    // 把模型保存到本地文件
    void save(const std::string &path);
    // 从本地文件加载模型
    void load(const std::string &path);
};

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::train(const std::vector<T> &set, std::shared_ptr<Node> parent) {
    std::shared_ptr<Node> node = std::make_shared<Node>();

    if (parent.get() != nullptr || parent.use_count() != 0) {
        parent->subNode.push_back(node);
    }

    node->type = Node::Middle;
    if (root.use_count() == 0) {
        for (int i = 0; i < ArgNum; ++i) {
            curArgs.push_back(i);
        }
        root = node;
    }

    // set.size()永不为0，所以这个块不会执行，是屎山
    if (set.size() == 0) {
        node->type = Node::Leaf;
        node->result = parent->result;
        return;
    }

    auto cbegin = set.cbegin();

    if (count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
        return *cbegin == v;
    }) == set.size()) {
        node->type = Node::Leaf;
        node->result = mostType<T, RT, F>(set);
        return;
    }

    if (set.size() == count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
        return F(*cbegin) == F(v);
    })) {
        node->type = Node::Leaf;
        node->result = F(*cbegin);
        return;
    }

    node->bestArg = selectArg(set, curArgs);
    int index = node->bestArg.first;
    float point = node->bestArg.second;

    std::vector<T> left, right;
    for (const auto &v : set) {
        if (getFun[index](v) < point) {
            left.push_back(v);
        } else {
            right.push_back(v);
        }
    }

    int size = set.size();

    if (left.size() == size || right.size() == size) {
        node->type = Node::Leaf;
        node->result = mostType<T, RT, F>(set);
        return;
    }

    const_cast<std::vector<T> &>(set).clear();

    train(left, node);
    train(right, node);
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::test(const std::vector<T> &set) {
    if (set.size() == 0) {
        throw new std::runtime_error("test set member num is 0");
    }

    float TP, FP, TN, FN;
    TP = FP = TN = FN = 0;

    RT trueType = F(set.at(0)); // 设第一个元素的分类为正例

    for (const auto &v : set) {
        RT realType = F(v);
        RT testResultType = result(v);
        if (realType == trueType) { // 真实结果为正例
            if (testResultType == realType) { // 预测结果与真实结果一致
                TP++;
            } else {
                FN++;
            }
        } else { // 真实结果为反例
            if (testResultType == realType) { // 预测结果与真实结果一致
                TN++;
            } else {
                FP++;
            }
        }
    }

    return (TP + TN) / (TP + TN + FP + FN);
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
RT Model<T, RT, F, ArgNum, GetArgFun>::result(const T &value) {
    if (root.use_count() == 0) {
        throw new std::runtime_error("There is no model");
    }
    std::shared_ptr<Node> curNode = root;
    while (1) {
        if (curNode->type == Node::Leaf) {
            return curNode->result;
        }
        if (getFun[curNode->bestArg.first](value) < curNode->bestArg.second) {
            curNode = curNode->subNode[0];
        } else {
            curNode = curNode->subNode[1];
        }
    }
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::save(const std::string &path) {
    std::ofstream o;
    o.open(path, std::ios_base::binary | std::ios_base::out);
    if (!o.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    // TODO
    o.close();
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::load(const std::string &path) {
    std::ifstream i;
    i.open(path, std::ios_base::binary | std::ios_base::in);
    if (!i.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    // TODO
    i.close();
}

// 工具函数

// 返回set集合中样本数最多的类
template<typename T,typename RT, RT (*F)(const T &)>
RT mostType(const std::vector<T> &set) {
    std::map<RT, int> map;
    int max = 0;
    int val;
    RT result;
    for (const T &v : set) {
        val = ++map[F(v)];
        if (val > max) {
            max = val;
            result = F(v);
        }
    }
    return result;
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
std::pair<int, float> Model<T, RT, F, ArgNum, GetArgFun>::selectArg(const std::vector<T> &set, const std::vector<int> &curArgs) {
    int index = curArgs[0];
    float point; // 划分点
    float gani = -1e20;

    float ent_set = Ent(set);

    for (int i = 0; i < curArgs.size(); ++i) {
        int curIndex = curArgs.at(i);
        std::vector<std::pair<float, RT>> a; // a 是属性值
        for (const auto &v : set) {
            a.push_back(std::make_pair(getFun[curIndex](v), F(v)));
        }
        std::sort(a.begin(), a.end(), [](const auto &a, const auto &b) {
            return a.first < b.first;
        });
        std::vector<float> tSet;
        for (auto it = a.cbegin(); it != a.cend() - 1; ++it) {
            tSet.push_back((it->first + (it + 1)->first) / 2);
        }
        float tmpPoint = 0; // 临时划分点
        float t_max = -1e20;
        for (float t : tSet) {
            std::vector<std::pair<float, RT>> minus, plus;
            for (const auto &v : a) {
                if (v.first < t) {
                    minus.push_back(v);
                } else {
                    plus.push_back(v);
                }
            }
            float gani_t = ent_set - ((((float)minus.size() / a.size()) * Ent(minus)) + (((float)plus.size() / a.size()) * Ent(plus)));
            if (gani_t > t_max) {
                t_max = gani_t;
                tmpPoint = t;
            }
        }
        if (t_max > gani) {
            gani = t_max;
            index = i;
            point = tmpPoint;
        }
    }

    return std::make_pair(index, point);
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::Ent(const std::vector<T> &set) {
    // 获得 Ent(D)
    auto cbegin = set.cbegin();
    float p1 = std::count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
        return F(*cbegin) == F(v);
    });
    float p2 = set.size() - p1;
    p1 /= set.size();
    p2 /= set.size();
    float ent_d = -(p1 * log2f(p1) + p2 * log2f(p2));
    return ent_d;
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::Ent(const std::vector<std::pair<float, RT>> &set) {
    // 获得 Ent(D)
    auto cbegin = set.cbegin();
    float p1 = std::count_if(set.cbegin(), set.cend(), [cbegin](const auto &v) {
        return cbegin->second == v.second;
    });
    float p2 = set.size() - p1;
    p1 /= set.size();
    p2 /= set.size();
    float ent_d = -(p1 * log2f(p1) + p2 * log2f(p2));
    return ent_d;
}

#endif
