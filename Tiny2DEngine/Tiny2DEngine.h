#pragma once

#include "windows.h"
#include "Trackball.h"
#include "Transform.h"
#include <ctime>
#include <queue>
#include <algorithm>
#include <Eigen/Dense>

using Eigen::Vector3f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
using Eigen::Vector2f;
using Eigen::Vector2i;

class Triangle2D;
class Triangle3D;

class Tiny2DEngine {
public:
    
    Tiny2DEngine(HINSTANCE hinst, int w, int h);
    ~Tiny2DEngine();
    
    void init();
    void mainloop();

    // Drawing API's

    inline void setPixel(int x, int y, const Vector3f &color);
    inline void drawPoint(int x, int y, int sz, const Vector3f &color);
    void drawTriangle(const Triangle3D &tri, const Vector3f &color);
    void drawTriangle(const Triangle3D &tri);
    inline void clear();

    void onMousePressed();
    void onMouseReleased();
    
    // User-defined behaviors

    virtual void onInit()               {}
    virtual void onMain()               {}
    virtual void onDestroy()            {}
    virtual void onUserMousePressed()   {}
    virtual void onUserMouseReleased()  {}


public:
    static LRESULT CALLBACK win_procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    inline void clamp_cursor_pos();
    void render();
    float findFps();
    void updateWindowInfo();
    void updateWindowTitle();
    void updateTrackball();
    inline Vector4f to_vec4f(Vector3f v, float f);
    inline Vector2f to_vec2f(Vector4f v);
    inline void clearZBuffer();
    Vector3f barycentric(const Vector2f &p, const Triangle2D &tri);

private:
    int             mWidth;
    int             mHeight;
    // windows stuff
    HINSTANCE       m_hInst;
    HWND            m_hWnd;
    HDC             m_hDC;
    // for frame buffer
    HBITMAP         m_hBufferBitmap;
    HDC             m_hBufferDC;
    BYTE            *mFrameBuffer;
    // for fps counting
    clock_t             mLastTime = 0;
    clock_t             mCurTime = 0;
    std::queue<float>   m_qTimes;
    float               mTimeSum = 0.f;
    // for cursor pos
    POINT               mMousePos;
    RECT                m_wndrect;
    bool                m_bCursorInWindow;
    // matrix
    Matrix4f            mModel;
    // zbuffer
    float*              m_pZBuffer;
    // trackball
    Trackball           mTrackball;
    Transform           mTransform;
    bool                m_bMousePressed = false;
};

//
// Set the color of a pixel
//
void Tiny2DEngine::setPixel(int x, int y, const Vector3f &color) {
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        return ;
    BYTE *base = mFrameBuffer + (x + y * mWidth) * 3;
    // Windows somehow have the order of RGB's reversed in the frame buffer
    base[0] = static_cast<BYTE>(color[2] * 255);
    base[1] = static_cast<BYTE>(color[1] * 255);
    base[2] = static_cast<BYTE>(color[0] * 255);
}

//
// Draw point of a certain size
//
void Tiny2DEngine::drawPoint(int x, int y, int sz, const Vector3f &color) {
    if (sz < 1)
        return;
    int offset = (sz - 1) / 2;
    x -= offset;
    y -= offset;
    for (int i = 0; i < sz; i ++ )
        for (int j = 0; j < sz; j ++ )
            setPixel(x + i, y + j, color);
}

//
// Clear screen to complete brightness
//
void Tiny2DEngine::clear() {
    memset(mFrameBuffer, 255, mWidth * mHeight * 3);
}

//
// Clamp cursor position to screen coordinates, i.e., x in [0,width) and y in [0,height)
// TODO: there's a bug to fix
//
void Tiny2DEngine::clamp_cursor_pos() {
    mMousePos.x = min(max(mMousePos.x, m_wndrect.left), m_wndrect.right) - m_wndrect.left;
    mMousePos.y = min(max(mMousePos.y, m_wndrect.top), m_wndrect.bottom) - m_wndrect.top;
    mMousePos.y = mHeight - mMousePos.y;
}

Vector4f Tiny2DEngine::to_vec4f(Vector3f v, float f)    {return Vector4f(v[0], v[1], v[2], f);}
Vector2f Tiny2DEngine::to_vec2f(Vector4f v)             {return Vector2f(v[0], v[1]);}
void Tiny2DEngine::clearZBuffer()                       {std::fill(m_pZBuffer, m_pZBuffer + mWidth * mHeight, -1e10);}