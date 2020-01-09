# pacific: a toy runtime for ark compiler

方舟编译器的Runtime参考实现。

目前由PLCT实验室进行开发和维护。

PLCT实验室全称是程序语言与编译技术实验室，

隶属于中国科学院软件研究所智能软件研究中心。

本项目采用 Apache 协议。

## 编译 pacific

首先，确保已经安装了 aarch64 gnu linux 工具链。比如在 Ubuntu 18.04 上，可以执行 apt-get install gcc-8-aarch64-linux-gnu。

其次，确保你安装了 aarch64 qemu。可以通过 make aarch64-qemu 来安装。

二者安装完毕后执行 make 或者 make pacific 即会编译出 pacific。

## 运行 pacific

make sample 将使用 pacific 加载一个从方舟编译器生成的 Hello World 汇编文件，你也可以通过 make sample Q V=1 或者直接查看 Makefile 来理解 pacific 是如何运行。doc 目录下也有简单的设计文档。
