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

示例：

```cmake
################################# CMakeLists.txt ##################################
cmake_minimum_required(VERSION 3.10)
project(foo)
set(TARGET_NAME run)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/include)
# Setup t2e
add_subdirectory(Tiny2DEngine)
include_directories(${CMAKE_SOURCE_DIR}/Tiny2DEngine)
# Setup executable target
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*.h)
add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} libt2e)
```



```cpp
/////////////////////////////////////// main.cpp ///////////////////////////////////
#include "Tiny2DEngine.h"

// Derive a subclass from the engine
class MyApp : public Tiny2DEngine {
public:
    // Constructor
    MyApp(HINSTANCE hinst, int w, int h) : Tiny2DEngine(hinst, w, h) {}

    // Optional: Rewrite onMain function, which will get executed in the loop
    // In this case, a white dot is drawn at (100,100)
    void onMain() override {
        setPixel(100, 100, Vector3f(1,1,1));
    }
};

// Main function for Win32 Application
int WinMain(HINSTANCE hinstance,
			HINSTANCE hprevinstance,
			LPSTR lpcmdline,
			int ncmdshow) {
    MyApp app(hinstance, 800, 600);     // Create a window
    app.init();                         // Necessary initializations
    app.mainloop();                     // Enter the main loop where stuff gets shown and mouse/keyboard gets answered
}
```

