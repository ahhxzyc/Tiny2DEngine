

#include "Tiny2DEngine.h"
#include "Triangle.h"
#include "AABB.h"
#include <cstdio>



LRESULT CALLBACK Tiny2DEngine::win_procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Get the Window object bound to hwnd
	Tiny2DEngine* pwnd = reinterpret_cast<Tiny2DEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (message) {
		case WM_PAINT: {
			if (pwnd) {
				pwnd->render();
				pwnd->updateWindowInfo();
				pwnd->updateWindowTitle();
			}
			return 0;
		}
		case WM_DESTROY: {
			if (pwnd) {
				pwnd->onDestroy();
			}
			PostQuitMessage(0);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			if (pwnd) {
				pwnd->onMousePressed();
			}
			return 0;
		}
		case WM_LBUTTONUP: {
			if (pwnd) {
				pwnd->onMouseReleased();
			}
		}
	}
    return DefWindowProc(hwnd, message, wParam, lParam);
}

Tiny2DEngine::Tiny2DEngine(HINSTANCE hinst, int w, int h) :
                mWidth(w), mHeight(h), m_hInst(hinst) {
	// Define window class
    WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc  = win_procedure;
	wndclass.cbClsExtra   = 0;
	wndclass.cbWndExtra   = 0;
	wndclass.hInstance    = hinst;
	wndclass.hIcon        = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor      = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground= (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName = NULL ;
	wndclass.lpszClassName= TEXT("WindowClass");
	RegisterClass(&wndclass);
	// Get window overrall size
	RECT rect;
	SetRect(&rect, 0, 0, mWidth, mHeight);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// Create window
	m_hWnd = CreateWindow(
		TEXT("WindowClass"),
		TEXT("Welcome"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hinst,
		NULL
	);
	m_hDC = GetDC(m_hWnd);
	ShowWindow(m_hWnd, 1);
	// Bind this instance to handle
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}


//
// Destructor
// 
Tiny2DEngine::~Tiny2DEngine() {
	delete[] m_pZBuffer;
}

//
// Initialize frame buffer and zbuffer of the engine
//
void Tiny2DEngine::init() {
	// Create bitmap and a DC that uses this bitmap
	// They are used as frame buffer
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mWidth;
	bmi.bmiHeader.biHeight = mHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	m_hBufferBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&mFrameBuffer, NULL, NULL);
	m_hBufferDC = CreateCompatibleDC(m_hDC);
	SelectObject(m_hBufferDC, m_hBufferBitmap);
	// Init zbuffer
	m_pZBuffer = new float[mWidth * mHeight]();
	clearZBuffer();
	// User-defined init
	onInit();
}


void Tiny2DEngine::render() {
	PAINTSTRUCT ps;
	BeginPaint (m_hWnd, &ps);
	// start rendering
	clearZBuffer();
	mModel = Matrix4f::Identity();
	onMain();
	// Update framebuffer
	BitBlt(m_hDC, 0, 0, mWidth, mHeight, m_hBufferDC, 0, 0, SRCCOPY);
	EndPaint (m_hWnd, &ps);
}


//
// Calculate average fps of last 100 frames
//
float Tiny2DEngine::findFps() {
	mCurTime = clock();
	float fps = -1.f;
	if (mLastTime) {
		float elapsed_t = (float)(mCurTime - mLastTime) / CLOCKS_PER_SEC;
		m_qTimes.push(elapsed_t);
		mTimeSum += elapsed_t;
		if (m_qTimes.size() > 100) {
			mTimeSum -= m_qTimes.front();
			m_qTimes.pop();
			fps = 100.f / mTimeSum;
		}
	}
	mLastTime = mCurTime;
	return fps;
}

//
// Get cursor position & window rect
//
void Tiny2DEngine::updateWindowInfo() {
	GetCursorPos(&mMousePos);
	GetWindowRect(m_hWnd, &m_wndrect);
	clamp_cursor_pos();
}

void Tiny2DEngine::updateWindowTitle() {
	char title[50];
	sprintf(title, "average fps: %.2f, cursor: %d,%d", findFps(), mMousePos.x, mMousePos.y);
	SetWindowText(m_hWnd, TEXT(title));
}

//
// Enter the loop where:
// 1. windows messages are handled
// 2. rendering happens
//
void Tiny2DEngine::mainloop() {
    MSG msg;
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		// send WM_PAINT
		InvalidateRect(m_hWnd, NULL, FALSE);
	}
}


//
// Draw triangle of a given color
//
void Tiny2DEngine::drawTriangle(const Triangle3D &tri, const Vector3f &color) {
	// To normalized device coordinates
    Vector4f a = mModel * to_vec4f(tri.a(), 1.f);
    Vector4f b = mModel * to_vec4f(tri.b(), 1.f);
    Vector4f c = mModel * to_vec4f(tri.c(), 1.f);
    Vector3f vz(a[2], b[2], c[2]);
    Vector2f a2 = to_vec2f(a);
    Vector2f b2 = to_vec2f(b);
    Vector2f c2 = to_vec2f(c);
    Triangle2D tri2d{a2, b2, c2};
    // Bounding box
    AABB2Df boxf(a2, b2);
    boxf.add(c2);
    AABB2D box( Vector2i((boxf.maxp[0] + 1.f) * 0.5f * mWidth, (boxf.maxp[1] + 1.f) * 0.5 * mHeight),
                Vector2i((boxf.minp[0] + 1.f) * 0.5f * mWidth, (boxf.minp[1] + 1.f) * 0.5 * mHeight));
    AABB2D screen(Vector2i(0, 0), Vector2i(mWidth - 1, mHeight - 1));
    box = box.intersect(screen);
    int boxWidth = box.maxp[0] - box.minp[0] + 1;
    int boxHeight = box.maxp[1] - box.minp[1] + 1;
    // For each pixel in the bounding box
    for (int i = 0; i < boxWidth * boxHeight; i ++ ) {
        // Normalized coordinates
        int x = box.minp[0] + i % boxWidth;
        int y = box.minp[1] + i / boxWidth;
        float fx = (float) x / mWidth * 2.f - 1.f;
        float fy = (float) y / mHeight * 2.f - 1.f;
        // Barycentric coords
        Vector3f baryCoords = barycentric(Vector2f(fx, fy), tri2d);
        if (baryCoords[0] < 0 || baryCoords[1] < 0 || baryCoords[2] < 0) {
            continue;
        }
        // ZBuffer comparison
        float z = baryCoords.dot(vz);
        if (z > m_pZBuffer[x + mWidth * y]) {
            setPixel(x, y, color);
            m_pZBuffer[x + mWidth * y] = z;
        }
    }
}

//
// This is an overload function
// The triangle's color is by default determined by the direction it's facing
// TODO: this function is unfinished, the triangle's now painted with its normal.
//
void Tiny2DEngine::drawTriangle(const Triangle3D &tri) {
	Vector3f color = tri.positive_normal();
	drawTriangle(tri, color);
}

//
// Return barycentric coordinates of point P for the triangle
//
Vector3f Tiny2DEngine::barycentric(const Vector2f &p, const Triangle2D &tri) {
	Vector2f ab = tri.b() - tri.a();
	Vector2f ac = tri.c() - tri.a();
	Vector2f pa = tri.a() - p;
	Vector3f x(ab[0], ac[0], pa[0]);
	Vector3f y(ab[1], ac[1], pa[1]);
	Vector3f u = x.cross(y);
	// See if the triangle is degenerate(three vertices on a line)
	if (u[2] < 1e-10) {
		return Vector3f(-1, 1, 1);
	}
	// Return barycentric coords
	float div = 1.f / u[2];
	float uu = u[0] * div;
	float vv = u[1] * div;
	return Vector3f(1.f - uu - vv, uu, vv);
}