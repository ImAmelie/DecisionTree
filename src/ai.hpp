/**
 * ai1.hpp 所包含的决策树没有剪枝功能
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
        std::vector<int> curArgs;
        std::pair<int, ArgType> bestArg;
        ArgType compass;
        std::vector<std::shared_ptr<Node>> subNode;
        // type is Leaf
        RT result; // 如果该节点是叶节点，此对象保存分类结果
    };
    std::shared_ptr<Node> root;

    // 工具成员函数
    inline std::pair<int, ArgType> selectArg(const std::vector<T> &set, const std::vector<int> &curArgs);
    inline float Ent(const std::vector<T> &set);
    inline float Ent(const std::vector<std::pair<ArgType, RT>> &set);

public:
    using BestArgType = typename Node::BestArgType;

    std::vector<GetArgFun> getFun;
    std::vector<BestArgType> argTypes;
    std::array<std::tuple<ArgType, ArgType>, ArgNum> z_score_model;

    int minLeafSize = 3; // 最小叶生成尺寸

    void train(const std::vector<T> &set, std::shared_ptr<Node> parent = nullptr, ArgType compass = ArgType());
    float test(const std::vector<T> &set);
    RT result(const T &value);

    std::vector<T> &z_score_train(std::vector<T> &set);
    std::vector<T> &z_score_test(std::vector<T> &set);

    // 把模型保存到本地文件
    void save(const std::string &path);
    // 从本地文件加载模型
    void load(const std::string &path);
};

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
void Model<T, RT, F, ArgNum, GetArgFun>::train(const std::vector<T> &set, std::shared_ptr<Node> parent, ArgType compass) {
    std::shared_ptr<Node> node = std::make_shared<Node>();

    node->compass = compass;

    if (parent) {
        parent->subNode.push_back(node);
    }

    node->type = Node::Middle;

    if (!root) {
        for (int i = 0; i < ArgNum; ++i) {
            node->curArgs.push_back(i);
        }
        root = node;
    } else {
        node->curArgs = parent->curArgs;
    }

    node->result = mostType<T, RT, F>(set);

    if (set.size() <= minLeafSize) {
        node->type = Node::Leaf;
        return;
    }

    auto cbegin = set.cbegin();

    if (node->curArgs.size() == 0 ||
        set.size() == count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
            return F(*cbegin) == F(v);
        }) ||
        set.size() == count_if(set.cbegin(), set.cend(), [cbegin](const T &v) {
            return *cbegin == v;
        })
        ) {
        node->type = Node::Leaf;
        return;
    }

    node->bestArg = selectArg(set, node->curArgs);
    int index = node->bestArg.first;
    ArgType point = node->bestArg.second;

    std::vector<std::vector<T>> subSet;
    std::vector<ArgType> subSet_point;

    if (argTypes.at(index) == BestArgType::Discrete) {
        std::vector<T> &tmp = const_cast<std::vector<T> &>(set);
        std::sort(tmp.begin(), tmp.end(), [this, index](const T &a, const T &b) {
            return getFun[index](a) < getFun[index](b);
        });
        subSet.push_back(std::vector<T>());
        int subSetIndex = 0;
        auto ptr = set.cbegin();
        subSet_point.push_back(getFun[index](*ptr));
        for (int i = 0; i < set.size(); ++i) {
            if (getFun[index](set[i]) == (getFun[index](*ptr))) {
                subSet[subSetIndex].push_back(set[i]);
            } else {
                subSet.push_back(std::vector<T>());
                subSet_point.push_back(getFun[index](set[i]));
                ++subSetIndex;
                subSet[subSetIndex].push_back(set[i]);
                ptr = set.cbegin() + i;
            }
        }
        std::remove(node->curArgs.begin(), node->curArgs.end(), index);
    } else { // argTypes.at(curIndex) == BestArgType::Continuous
        subSet.push_back(std::vector<T>());
        subSet.push_back(std::vector<T>());
        for (const auto &v : set) {
            if (getFun[index](v) < point) {
                subSet[0].push_back(v);
            } else {
                subSet[1].push_back(v);
            }
        }

        if (subSet[0].size() == 0 || subSet[1].size() == 0) {
            node->type = Node::Leaf;
            return;
        }

        subSet_point.push_back(ArgType());
        subSet_point.push_back(ArgType());
    }

    const_cast<std::vector<T> &>(set).clear();

    for (int i = 0; i < subSet.size(); ++i) {
        train(subSet[i], node, subSet_point[i]);
    }
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
    if (root.use_count() == 0) {
        throw new std::runtime_error("There is no model");
    }
    std::shared_ptr<Node> curNode = root;
    while (1) {
        if (curNode->type == Node::Leaf) {
            return curNode->result;
        }
        if (argTypes[curNode->bestArg.first] == BestArgType::Discrete) {
            for (const auto &v : curNode->subNode) {
                if (v->compass == getFun[curNode->bestArg.first](value)) {
                    curNode = v;
                    break;
                }
            }
        } else { // argTypes(curNode->bestArg.first) == BestArgType::Continuous
            if (getFun[curNode->bestArg.first](value) < curNode->bestArg.second) {
                curNode = curNode->subNode[0];
            } else {
                curNode = curNode->subNode[1];
            }
        }
    }
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
std::vector<T> &Model<T, RT, F, ArgNum, GetArgFun>::z_score_train(std::vector<T> &set) {
    if (set.size() == 0) {
        throw new std::runtime_error("set is empty");
    }

    for (int i = 0; i < ArgNum; ++i) {
        if (argTypes[i] == BestArgType::Discrete) {
            continue;
        }
        ArgType tmp;
        ArgType sum = 0;
        for (const T &v : set) {
            sum += getFun[i](v);
        }
        ArgType mean = sum / set.size();
        ArgType diff_square_sum = 0;
        for (const T &v : set) {
            tmp = getFun[i](v) - mean;
            diff_square_sum += tmp * tmp;
        }
        ArgType standard_deviation = std::sqrt(diff_square_sum / (set.size() - 1));
        for (T &v : set) {
            getFun[i](v) = (getFun[i](v) - mean) / standard_deviation;
        }

        std::get<0>(z_score_model.at(i)) = mean;
        std::get<1>(z_score_model.at(i)) = standard_deviation;
    }

    return set;
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
std::vector<T> &Model<T, RT, F, ArgNum, GetArgFun>::z_score_test(std::vector<T> &set) {
    if (set.size() == 0) {
        throw new std::runtime_error("set is empty");
    }

    for (int i = 0; i < ArgNum; ++i) {
        if (argTypes[i] == BestArgType::Discrete) {
            continue;
        }

        ArgType mean = std::get<0>(z_score_model.at(i));
        ArgType standard_deviation = std::get<1>(z_score_model.at(i));

        for (T &v : set) {
            getFun[i](v) = (getFun[i](v) - mean) / standard_deviation;
        }
    }

    return set;
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
std::pair<int, typename Model<T, RT, F, ArgNum, GetArgFun>::ArgType>
Model<T, RT, F, ArgNum, GetArgFun>::selectArg(const std::vector<T> &set, const std::vector<int> &curArgs) {
    int index = curArgs[0];
    float gani = -1e60;
    float ent_set = Ent(set);

    ArgType point; // 划分点

    for (int i = 0; i < curArgs.size(); ++i) {
        int curIndex = curArgs.at(i);
        std::vector<std::pair<ArgType, RT>> a; // a 是属性值
        for (const auto &v : set) {
            a.push_back(std::make_pair(getFun[curIndex](v), F(v)));
        }
        std::sort(a.begin(), a.end(), [](const auto &a, const auto &b) {
            return a.first < b.first;
        });

        float t_max = -1e60;

        if (argTypes.at(curIndex) == BestArgType::Discrete) {
            std::vector<ArgType> attrs;
            for (const auto &v : a) {
                attrs.push_back(v.first);
            }
            auto last = std::unique(attrs.begin(), attrs.end());
            attrs.erase(last, attrs.end());

            float sum = 0;

            for (const auto &v : attrs) {
                std::vector<T> attr_v_set;
                for (const T &e : set) {
                    if (getFun[curIndex](e) == v) {
                        attr_v_set.push_back(e);
                    }
                }
                sum += float(attr_v_set.size()) / set.size() * Ent(attr_v_set);
            }
            t_max = Ent(set) - sum;
            if (t_max > gani) {
                gani = t_max;
                index = curIndex;
            }
        } else { // argTypes.at(curIndex) == BestArgType::Continuous
            std::vector<ArgType> tSet;
            for (auto it = a.cbegin(); it != a.cend() - 1; ++it) {
                tSet.push_back((it->first + (it + 1)->first) / 2);
            }
            ArgType tmpPoint = 0; // 临时划分点
            for (ArgType t : tSet) {
                std::vector<std::pair<ArgType, RT>> minus, plus;
                for (const auto &v : a) {
                    if (v.first < t) {
                        minus.push_back(v);
                    } else {
                        plus.push_back(v);
                    }
                }
                float gani_t = ent_set - ((((ArgType)minus.size() / a.size()) * Ent(minus)) + (((ArgType)plus.size() / a.size()) * Ent(plus)));
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
    }

    return std::make_pair(index, point);
}

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
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

template<typename T, typename RT, RT &(*F)(const T &), int ArgNum, typename GetArgFun>
float Model<T, RT, F, ArgNum, GetArgFun>::Ent(const std::vector<std::pair<ArgType, RT>> &set) {
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
