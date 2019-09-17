由于本次实验内容较多，所以我一共提交了三个版本的代码
page_adress_translation_1.cpp对应:内存充足，无页面置换，无TLB
page_adress_translation_2.cpp对应:内存不足，使用随机页面置换，使用TLB
page_adress_translation_3.cpp对应:内存不足，使用随机页面置换，但不使用TLB，用于和2号版本对比

提供了makefile
如果make失败,请注意:

1.
在我的代码中我有一处用到memcpy_s()
这个函数原本是MS的单独实现，后来在c11被加入标准
C11 standard (ISO/IEC 9899:2011):
7.24.2.1 The memcpy function (p: 362)
K.3.7.1.1 The memcpy_s function (p: 614)
我不确认您的编译环境是否和我相同，所以如果make失败，请尝试将其替换为普通版本的memcpy(),或使用vs编译
或参考https://stackoverflow.com/questions/31278172/undefined-reference-to-memcpy-s

2.
如果尝试手动编译，请注意至少要使用c++11标准。(使用了auto, foreach)