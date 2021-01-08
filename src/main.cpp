#include "Tiny2DEngine.h"
#include "Triangle.h"

// Derive a subclass from the engine
class MyApp : public Tiny2DEngine {
public:
    // Constructor
    MyApp(HINSTANCE hinst, int w, int h) : Tiny2DEngine(hinst, w, h) {}

    // Optional: Rewrite onMain function, which will get executed in the loop
    void onMain() override {
        Triangle3D tri{Vector3f(-0.5,-0.5,0),
                    Vector3f(0.5,-0.5,0),
                    Vector3f(0.5,0.5,0)};
        drawTriangle(tri);
        Vector3f red(1,0,0);
        drawPoint(tri.a(), 5, red);
        drawPoint(tri.b(), 5, red);
        drawPoint(tri.c(), 5, red);
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