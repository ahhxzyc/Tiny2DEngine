# Tiny2DEngine

## Intro

作业用2D引擎，基于Windows。提供功能:

- 实时绘制

- 单个像素绘制
- 三角形绘制
- 通过鼠标拖拽调整视角

在Win10运行正常。

## Usage

1. 复制`Tiny2DEngine`目录到自己的项目下，在CMakeLists中添加对`libt2e`的依赖。
2. 包含头文件`Tiny2DEngine.h`，以及`Triangle.h`等（如果需要的话）。
3. 继承引擎定义的`Tiny2DEngine`虚类，重写`onInit()`、`onMain()`、`onDestroy()`等函数实现自己需要的功能。在其中可以使用`setPixel()`、`drawTriangle()`等引擎提供的绘制函数。

示例代码：

[CMakeLists.txt](CMakeLists.txt)

[main.cpp](src/main.cpp)



