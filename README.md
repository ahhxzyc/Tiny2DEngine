# Tiny2DEngine

## Intro

作业用2D引擎，基于Windows。提供单个像素绘制功能。

在Win10运行正常。

## Getting started

引擎依赖于Eigen库，即`include/Eigen`。

此外还包含头文件`Tiny2DEngine.h`和cpp文件`Tiny2DEngine.cpp`。它们定义了一个虚类`Tiny2DEngine`，需要继承使用。

示例：

```cpp
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

