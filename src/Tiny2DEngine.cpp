

#include "Tiny2DEngine.h"
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
// Initialize frame buffer of the engine
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
	onInit();
}


void Tiny2DEngine::render() {
	PAINTSTRUCT ps;
	BeginPaint (m_hWnd, &ps);
	onMain();
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