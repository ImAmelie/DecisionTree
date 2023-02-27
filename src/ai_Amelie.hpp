/**
 * 采用 Amelie 自研算法
 * 目前只能计算连续属性的数据集
 */

#ifndef AI_H
#define AI_H

#include <type_traits>
#include <vector>
#include <array>
#include <tuple>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <utility>
#include <stdexcept>
#include <fstream>
#include <cmath>

// 函数原型

// 返回set集合中样本数最多的类
template<typename T,typename RT, RT &(*F)(const T &)>
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
template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
class Model {
public:
    // 数据集中各属性的类型，目前用不到，保留
    // 如果编译出错，说明模板参数 GetArgFun 不是函数类型
    using ArgType = typename std::remove_reference<decltype(GetArgFun()(T()))>::type;

private:
    // 节点类，代表模型中的节点
    class Node {
        friend class Model;
    public:
        enum BestArgType { Discrete, Continuous };
    private:
        enum Type { Leaf, Middle } type; // 节点类型，叶节点或中间节点
        // type is Middle
        std::vector<std::shared_ptr<Node>> subNode;
        // type is Leaf
        RT result; // 如果该节点是叶节点，此对象保存分类结果

        std::array<T, 2> points;
    };
    std::shared_ptr<Node> root;

public:
    using BestArgType = typename Node::BestArgType;

    std::vector<GetArgFun> getFun;
    std::vector<BestArgType> argTypes;

    int minLeafSize = 0; // 最小叶生成尺寸

    float Ent(const std::vector<T> &set);
    float Gini(const std::vector<T> &set);

    void train(const std::vector<T> &set, std::shared_ptr<Node> parent = nullptr);
    float test(const std::vector<T> &set);
    RT result(const T &value);

    // 把模型保存到本地文件
    void save(const std::string &path);
    // 从本地文件加载模型
    void load(const std::string &path);
};

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::train(const std::vector<T> &set, std::shared_ptr<Node> parent) {
    std::shared_ptr<Node> node = std::make_shared<Node>();

    if (parent) {
        parent->subNode.push_back(node);
    }

    node->type = Node::Middle;

    if (!root) {
        root = node;
    }

    if (set.size() == 0) {
        node->result = parent->result;
        node->type = Node::Leaf;
        return;
    }

    node->result = mostType<T, RT, F>(set);

    if (set.size() <= minLeafSize) {
        node->type = Node::Leaf;
        return;
    }

    auto cbegin = set.cbegin();

    if (set.size() == count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
            return F(*cbegin) == F(v);
        }) ||
        set.size() == count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
            return *cbegin == v;
        })
        ) {
        node->type = Node::Leaf;
        return;
    }

    std::map<RT, std::tuple<float, T>> barycenter;

    for (const auto &v : set) {
        if (barycenter.find(F(v)) == barycenter.end()) {
            barycenter[F(v)] = std::make_tuple(1, v);
            continue;
        }
        T center = std::get<1>(barycenter[F(v)]); // 旧的中心
        float center_weight = std::get<0>(barycenter[F(v)]); // 旧的中心体重
        T newCenter; // 新的中心
        float newCenter_weight = center_weight + 1; // 新的中心的体重
        for (int i = 0; i < ArgNum; ++i) {
            // 计算新中心坐标数据
            getFun[i](newCenter) = getFun[i](center) + (getFun[i](v) - getFun[i](center)) * (1 / (center_weight + 1));
        }
        barycenter[F(v)] = std::make_tuple(newCenter_weight, newCenter);
    }

    std::vector<std::tuple<T, RT>> centers;
    for (auto it = barycenter.begin(); it != barycenter.end(); ++it) {
        centers.push_back(std::make_tuple(std::get<1>(it->second), it->first));
    }
    barycenter.clear();

    std::vector<std::pair<float, std::pair<T, T>>> rank;

    for (int i = 0; i < centers.size(); ++i) {
        for (int j = i + 1; j < centers.size(); ++j) {
            if (i == j) {
                continue;
            }
            rank.push_back(std::make_pair(0, std::make_pair(std::get<0>(centers[i]), std::get<0>(centers[j]))));
        }
    }

    centers.clear();

    int bestRankIndex = 0;

    for (int i = 0; i < rank.size(); ++i) {
        T center0 = rank[i].second.first;
        T center1 = rank[i].second.second;
        std::vector<T> less, greater;
        for (const auto &v : set) {
            float dis0, dis1;
            float square_sum0 = 0;
            float square_sum1 = 0;
            for (int i = 0; i < ArgNum; ++i) {
                square_sum0 += std::pow(getFun[i](v) - getFun[i](center0), 2);
                square_sum1 += std::pow(getFun[i](v) - getFun[i](center1), 2);
            }
            dis0 = std::sqrt(square_sum0);
            dis1 = std::sqrt(square_sum1);
            if (dis0 < dis1) {
                less.push_back(v);
            } else {
                greater.push_back(v);
            }
        }
        // rank[i].first = Ent(set) - (((float(less.size()) / set.size()) * Ent(less)) + ((float(greater.size()) / set.size()) * Ent(greater)));
        rank[i].first = (((float(less.size()) / set.size()) * Gini(less)) + ((float(greater.size()) / set.size()) * Gini(greater)));
    }

    std::sort(rank.begin(), rank.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    node->points[0] = rank[0].second.first;
    node->points[1] = rank[0].second.second;

    rank.clear();

    std::vector<T> less, greater;
    for (const auto &v : set) {
        float dis0, dis1;
        float square_sum0 = 0;
        float square_sum1 = 0;
        for (int i = 0; i < ArgNum; ++i) {
            square_sum0 += std::pow(getFun[i](v) - getFun[i](node->points[0]), 2);
            square_sum1 += std::pow(getFun[i](v) - getFun[i](node->points[1]), 2);
        }
        dis0 = std::sqrt(square_sum0);
        dis1 = std::sqrt(square_sum1);
        if (dis0 < dis1) {
            less.push_back(v);
        } else {
            greater.push_back(v);
        }
    }

    const_cast<std::vector<T> &>(set).clear();

    if (less.size() == 0 || greater.size() == 0) {
        node->type = Node::Leaf;
        return;
    }

    train(less, node);
    train(greater, node);
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::test(const std::vector<T> &set) {
    if (set.size() == 0) {
        throw new std::runtime_error("test set member num is 0");
    }

    float t = 0;

    for (const auto &v : set) {
        if (F(v) == result(v)) {
            t++;
        }
    }

    return t / set.size();
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
RT Model<T, RT, F, ArgNum, GetArgFun>::result(const T &value) {
    if (!root) {
        throw new std::runtime_error("There is no model");
    }
    std::shared_ptr<Node> curNode = root;
    while (1) {
        if (curNode->type == Node::Leaf) {
            return curNode->result;
        }
        float dis0, dis1;
        float square_sum0 = 0;
        float square_sum1 = 0;
        for (int i = 0; i < ArgNum; ++i) {
            square_sum0 += std::pow(getFun[i](value) - getFun[i](curNode->points[0]), 2);
            square_sum1 += std::pow(getFun[i](value) - getFun[i](curNode->points[1]), 2);
        }
        dis0 = std::sqrt(square_sum0);
        dis1 = std::sqrt(square_sum1);
        if (dis0 < dis1) {
            curNode = curNode->subNode[0];
        } else {
            curNode = curNode->subNode[1];
        }
    }
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::save(const std::string &path) {
    std::ofstream o;
    o.open(path, std::ios_base::binary | std::ios_base::out);
    if (!o.is_open()) {
        throw new std::runtime_error("File path is invalid");
    }
    // TODO
    o.close();
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
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
template<typename T,typename RT, RT &(*F)(const T &)>
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

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::Ent(const std::vector<T> &set) {
    // 计算信息熵，越小越好
    float ent = 0;
    std::map<RT, int> types;
    for (const auto &v : set) {
        ++types[F(v)];
    }
    for (auto it = types.cbegin(); it != types.cend(); ++it) {
        float p = float(it->second) / set.size();
        ent += p * log2f(p);
    }

    return -ent;
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::Gini(const std::vector<T> &set) {
    // 获得 Gini(D)
    std::map<RT, int> types;
    for (const auto &v : set) {
        ++types[F(v)];
    }
    float sum = 0;
    for (auto it = types.cbegin(); it != types.cend(); ++it) {
        sum += std::pow(float(it->second) / set.size(), 2);
    }

    return 1 - sum;
}

#endif
