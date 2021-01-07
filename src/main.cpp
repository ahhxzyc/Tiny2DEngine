#include "Tiny2DEngine.h"

// Derive a subclass from the engine
class MyApp : public Tiny2DEngine {
public:
    // Constructor
    MyApp(HINSTANCE hinst, int w, int h) : Tiny2DEngine(hinst, w, h) {}

    // Optional: Rewrite onMain function, which will get executed in the loop
    void onMain() override {drawPoint(100, 100, 2, Vector3f(1,1,1));}
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