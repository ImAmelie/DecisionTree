<div align="center">
  <h1>手撕决策树（人工智能/机器学习）</h1>
  <p>
    <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="知识共享许可协议" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><a href="https://www.gnu.org/licenses/gpl-3.0.html"><img alt="GPLv3开源许可协议" style="border-width:0;width:88px" src="https://www.gnu.org/graphics/gplv3-with-text-136x68.png" /></a>
  </p>
</div>


## 前言

最近小伙伴 Revival 教了我点人工智能/机器学习的知识，作为检测我的学习效果，他提出了让我“手撸决策树”的考验。

经过不懈努力，我终于完成一个决策树，并且实现了预剪枝特性。

正确跑通了算法后，Revival 让我趁热打铁写一篇相关文章，但由于篇幅过长，而且大部分内容都与《机器学习》（周志华）第四章重复，所以就懒得写了 :-)

## 决策树

什么是决策树？简单讲就是分类，比如这里有很多种玩具，我们要分出适合小男孩玩的玩具和适合小女孩玩的，玩具有好多属性，比如颜色、质地。我们可以根据颜色是冷色暖色、质地是柔软坚硬来分出男孩女孩的玩具。

好了，你已经会决策树了，现在开始手撸吧！

详细数学算法在《机器学习》（周志华）第四章有介绍，我写的这个决策树就是看完第4章写的。（只看第4章就可以）

## 结果准确率

```
Raisin:
训练用时: 1.02 s
测试用时：0.00 s
测试结果准确率: 82.59%
WDBC:
训练用时: 1.33 s
测试用时：0.00 s
测试结果准确率: 90.70%
Iris:
训练用时: 0.01 s
测试用时：0.00 s
测试结果准确率: 97.83%
Wine:
训练用时: 0.04 s
测试用时：0.00 s
测试结果准确率: 91.07%
```

自己设计的决策树算法的准确率：

```
Raisin:
训练用时: 0.01 s
测试用时：0.00 s
测试结果准确率: 77.41%
WDBC:
训练用时: 0.01 s
测试用时：0.00 s
测试结果准确率: 86.63%
Iris:
训练用时: 0.00 s
测试用时：0.00 s
测试结果准确率: 100.00%
Wine:
训练用时: 0.00 s
测试用时：0.00 s
测试结果准确率: 58.93%
```

[数据集下载网站](https://archive.ics.uci.edu/ml/index.php)

## 构建

可以使用 CMake 工具直接构建，也可以使用 g++/clang++ 等工具构建。

构建之前记得修改 `main.cpp` 里的 `load()` 函数的参数（训练/测试用文件的路径）。

```shell
cd src
g++ main.cpp raisin.cpp wdbc.cpp -o demo.exe
./demo.exe
```

## 其他说明

`ai.hpp` 是不带剪枝功能的 C4.5 决策树。

`ai2.hpp` 是带剪枝功能的 C4.5 决策树。

`ai3.hpp` 是不带剪枝功能的 CART 决策树

`ai_Amelie.hpp` 是自创的算法，不是很优秀。

切换方法：修改 `main.cpp` 头文件包含就可以了，如把

```cpp
#include "ai.hpp"
```

改为

```cpp
#include "ai3.hpp"
```
