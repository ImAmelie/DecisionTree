/**
 * ai2.hpp 所包含的决策树具有预剪枝功能
 */

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
std::pair<RT, int> mostType(const std::vector<T> &set);

// 类定义

/**
 * 决策树模型类
 * 模板参数：
 *   T: 数据集合中数据的类型
 *   RT: 数据的分类标签类型
 *   F: 指向 获取单个数据分类标签的函数 的指针
 *   ArgNum: 数据集合中数据的属性个数
 *   GetArgFun: 获取单个数据属性值的函数 的函数类型
 */
template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
class Model {
public:
    // 数据集中各属性的类型，目前用不到，保留
    // 如果编译出错，说明模板参数 GetArgFun 不是函数类型
    using ArgType = decltype(GetArgFun());

private:
    // 节点类，代表模型中的节点
    class Node {
        friend class Model<T, RT, F, ArgNum, GetArgFun>;
    private:
        enum Type { Leaf, Middle } type; // 节点类型，叶节点或中间节点
        // type is Middle
        std::pair<int, float> bestArg;
        std::vector<std::shared_ptr<Node>> subNode;
        // type is Leaf
        RT result; // 如果该节点是叶节点，此对象保存分类结果
    };
    std::vector<int> curArgs;
    std::shared_ptr<Node> root;

    // 用于预剪枝的验证集合
    std::vector<T> verifiedSet;

    // 工具成员函数
    inline std::pair<int, float> selectArg(const std::vector<T> &set, const std::vector<int> &curArgs);
    inline float Ent(const std::vector<T> &set);
    inline float Ent(const std::vector<std::pair<float, RT>> &set);

public:
    std::vector<GetArgFun> getFun;

    void train(const std::vector<T> &set, std::shared_ptr<Node> parent = nullptr);
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

        // 对 用于预剪枝的验证集合 进行初始化操作
        int n = set.size() * 0.7;
        verifiedSet.assign(set.begin() + n, set.end());
        const_cast<std::vector<T> &>(set).resize(n);
    }

    auto cbegin = set.cbegin();

    if (count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
        return *cbegin == v;
    }) == set.size()) {
        node->type = Node::Leaf;
        node->result = mostType<T, RT, F>(set).first;
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
        node->result = mostType<T, RT, F>(set).first;
        return;
    }

    // 剪枝处理开始
    if (0) {
        // 把当前节点当做叶节点，用验证集计算它的准确率
        node->type = Node::Leaf;
        node->result = mostType<T, RT, F>(set).first;

        const_cast<std::vector<T> &>(set).clear();

        float beforeAccuracy = test(verifiedSet);

        // 把当前节点当做中间节点，用验证集测试其准确率
        node->type = Node::Middle;
        node->bestArg = std::make_pair(index, point);

        std::shared_ptr<Node> leftNode = std::make_shared<Node>();
        leftNode->type = Node::Leaf;
        leftNode->result = mostType<T, RT, F>(left).first;

        std::shared_ptr<Node> RightNode = std::make_shared<Node>();
        RightNode->type = Node::Leaf;
        RightNode->result = mostType<T, RT, F>(right).first;

        node->subNode.push_back(leftNode);
        node->subNode.push_back(RightNode);

        float afterAccuracy = test(verifiedSet);

        node->subNode.pop_back();
        node->subNode.pop_back();

        if (beforeAccuracy >= afterAccuracy) { // 禁止划分
            node->type = Node::Leaf;
            return;
        }
    }
    // 剪枝处理结束

    train(left, node);
    train(right, node);
}

template<typename T, typename RT, RT (*F)(const T &), int ArgNum, typename GetArgFun>
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

// 返回set集合中样本数最多的类和个数
template<typename T,typename RT, RT (*F)(const T &)>
std::pair<RT, int> mostType(const std::vector<T> &set) {
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
    return std::make_pair(result, max);
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
            index = curIndex;
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
