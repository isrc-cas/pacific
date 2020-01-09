# pacific: a toy runtime for ark compiler

方舟编译器的Runtime参考实现。

目前由PLCT实验室进行开发和维护。

PLCT实验室全称是程序语言与编译技术实验室，

隶属于中国科学院软件研究所智能软件研究中心。

本项目采用 Apache 协议。

TODO：确认方舟编译器使用的木兰协议跟Apache协议的相容性。

## 编译 pacific (Ubuntu 18.04)
1、安装 aarch64 gnu linux 工具链：

  可以使用： apt-get install gcc-8-aarch64-linux-gnu，安装aarch64 gnu linux 工具链。

2、安装aarch64 qemu：

  在pacific目录之下，通过运行 pacific/script/build-qemu-aarch64.sh来安装aarch64 qemu，或者通过 make aarch64-qemu 来安装。

3、编译pacific：

  执行 make 或者 make pacific 即会编译出 pacific。

## 编译 pacific (Ubuntu 16.04)
1、安装 aarch64 gnu linux 工具链:

  可以使用： apt-get install gcc-aarch64-linux-gnu，安装aarch64 gnu linux 工具链。

2、安装aarch64 qemu:

  在pacific目录之下，通过运行 pacific/script/build-qemu-aarch64.sh来安装aarch64 qemu，或者通过 make aarch64-qemu 来安装。

3、代码修改:

  将Makefile之中的“CROSS_AARCH64_GCC = aarch64-linux-gnu-gcc-8”修改为“CROSS_AARCH64_GCC = aarch64-linux-gnu-gcc”。

4、编译pacific:

  执行 make 或者 make pacific 即会编译出 pacific。

## 运行 pacific

make sample 将使用 pacific 加载一个从方舟编译器生成的 Hello World 汇编文件。

也可以通过 make sample Q V=1 或者直接查看 Makefile 来理解 pacific 是如何运行。

另外，doc 目录下也有简单的设计文档。
