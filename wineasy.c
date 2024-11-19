#include "WinEasy.h"

#include <stdio.h>
#include <time.h>

float timer = 0.0f;
WINBOOL rev = 0;

typedef struct {
    int x;
    int y;
} Vec2;

Vec2 p1 = {50, 50};
Vec2 p2 = {200, 300};

float lerp(int a, int b, float t)
{
    return a + t * (b - a);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch(msg)
    {   
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_KEYDOWN:
            switch (wParam)
            {
                //R
                case 0x52:
                    //printf("R");
                    break;
                default:
                    break;
            }
        break;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC frontHDC, backHDC;
            HBITMAP backBuffer;
            RECT windowRect = {0,0,0,0};
            clock_t beg_time = clock();
            WinEasyStartBackBuffer(&ps, &hwnd, &frontHDC, &backHDC, &backBuffer, &windowRect);

            WinEasyDrawBackground(backHDC, colors[DARK_GRAY], windowRect);
            WinEasyDrawCircle(backHDC, colors[RED], p1.x, p1.y, 10);
            WinEasyDrawCircle(backHDC, colors[RED], p2.x, p2.y, 10);

            Vec2 p3 = {(int)lerp(p1.x, p2.x,timer), (int)lerp(p1.y, p2.y,timer)};
            WinEasyDrawCircle(backHDC, colors[GREEN], p3.x, p3.y, 5);

            if (timer >= 1.0f){
                rev = 1;
            }
            else if (timer <= 0.0f){
                rev = 0;
            }

            if(rev){
                timer -= 0.002;
            }
            else{
                timer += 0.002;
            }

            WinEasyCopyBackBuffer(frontHDC, backHDC, windowRect);

            WinEasyEndBackBuffer(ps, hwnd, backHDC, backBuffer);
            InvalidateRect(hwnd, &(RECT){windowRect.top, windowRect.left, windowRect.right, windowRect.bottom}, FALSE);
            clock_t end_time = clock();
            WinEasySleepForFrames(60, beg_time, end_time);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    char ClassName[] = "myWindowClass";

    WinEasyCreateWindowClass(&wc, hInstance, &WndProc, ClassName, (HBRUSH)(COLOR_WINDOW-1));

    // Step 2: Creating the Window
    WinEasyCreateWindow(&hwnd, "WinEasy Test", ClassName, hInstance, 800, 600, 500, 200);

    ShowWindow(hwnd, nCmdShow);

    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}