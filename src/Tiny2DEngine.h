#pragma once

#include "windows.h"
#include <ctime>
#include <queue>
#include <Eigen/Dense>

using Eigen::Vector3f;

class Tiny2DEngine {
public:
    // ctor
    Tiny2DEngine(HINSTANCE hinst, int w, int h);
    // API's
    void init();
    void paint();
    void mainloop();
    inline void setPixel(int x, int y, const Vector3f &color);
    inline void clear();
    
    int width() const       {return mWidth;}
    int height() const      {return mHeight;}

public:
    // virtual functions
    virtual void onInit()               {}
    virtual void onMain()               {}
    virtual void onDestroy()            {}
    virtual void onMousePressed()       {}
    virtual void onMouseReleased()      {}

/////////////////////////////////////////////////////
// Dirty parts
/////////////////////////////////////////////////////
public:
    static LRESULT CALLBACK win_procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    inline void clamp_cursor_pos();

protected:
    int             mWidth;
    int             mHeight;
    // basic info about the window
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
};


void Tiny2DEngine::setPixel(int x, int y, const Vector3f &color) {
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
        return ;
    }
    BYTE *base = mFrameBuffer + (x + y * mWidth) * 3;
    // Windows somehow have the order of RGB's reversed in the frame buffer
    base[0] = static_cast<BYTE>(color[2] * 255);
    base[1] = static_cast<BYTE>(color[1] * 255);
    base[2] = static_cast<BYTE>(color[0] * 255);
}

void Tiny2DEngine::clear() {
    memset(mFrameBuffer, 255, mWidth * mHeight * 3);
}

void Tiny2DEngine::clamp_cursor_pos() {
    mMousePos.x = min(max(mMousePos.x, m_wndrect.left), m_wndrect.right) - m_wndrect.left;
    mMousePos.y = min(max(mMousePos.y, m_wndrect.top), m_wndrect.bottom) - m_wndrect.top;
    mMousePos.y = mHeight - mMousePos.y;
}