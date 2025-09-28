// Windows3DTouchDemo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <dwmapi.h>
#include "Windows3DTouchDemo.h"
#include "TouchpadReader.h"

#pragma comment(lib, "dwmapi.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

TouchpadReader *touchpad;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWS3DTOUCHDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWS3DTOUCHDEMO));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWS3DTOUCHDEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.hbrBackground  = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWS3DTOUCHDEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

static void EnableMica(HWND hwnd)
{
    // Enable dark mode
    BOOL darkMode = TRUE;
    DwmSetWindowAttribute(hwnd, 20, &darkMode, sizeof(darkMode)); // DWMWA_USE_IMMERSIVE_DARK_MODE

    // Set window corner preference
    DWORD cornerPreference = 2; // DWMWCP_ROUND - rounded corners
    DwmSetWindowAttribute(hwnd, 33, &cornerPreference, sizeof(cornerPreference));

    // Enable Mica
    const DWORD micaEnabled = 0x01;
    DwmSetWindowAttribute(hwnd, 1029, &micaEnabled, sizeof(micaEnabled)); // DWMWA_MICA_EFFECT

    // Make window transparent for Mica to show through
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);


}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hButton;
    RECT buttonRect = { 40, 40, 100, 30 };
    RECT prevButtonRect = { 0 };

    switch (message)
    {
    case WM_CREATE:
        {
            //EnableMica(hWnd);

           

            

            touchpad = new TouchpadReader(hWnd);

            hButton = CreateWindow(
                L"BUTTON",
                L"OK",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, buttonRect.left, buttonRect.top, buttonRect.right, buttonRect.bottom, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
            
            HFONT hFont = CreateFontW(
                14, 0, 0, 0, FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, L"Segoe UI"
            ); 
            
            if (hFont) {
                SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
            }
        }
        break;
    case WM_INPUT:
        {
            HRAWINPUT hRawInput = (HRAWINPUT)lParam;
            if (touchpad)
            {
                TOUCHPAD_EVENT data = touchpad->ProcessInput(hRawInput);

                auto dimensions = data.touches[0].touch.dimensions;
                double size = data.touches[0].touch.size;
                
                if (hButton && data.touchCount > 0) {

                    POINT cursorPos = { 0 };
                    GetCursorPos(&cursorPos);

                    ScreenToClient(hWnd, &cursorPos);

                    if (true || PtInRect(&buttonRect, cursorPos))
                    {
                        OutputDebugString(L"WOW");

                        double bias = -30;
                        double biasedSize = size + bias;
                        if (biasedSize < 0) biasedSize = 0;
                        double multiplier = (biasedSize / 100) + 1;

                        RECT adjustedRect = { 0 };
                        adjustedRect.bottom = buttonRect.bottom * multiplier;
                        adjustedRect.right = buttonRect.right * multiplier;
                        adjustedRect.top = buttonRect.top - (adjustedRect.bottom - buttonRect.bottom) / 2.0f;
                        adjustedRect.left = buttonRect.left - (adjustedRect.right - buttonRect.right) / 2.0f;
                        SetWindowPos(hButton, NULL, adjustedRect.left, adjustedRect.top, adjustedRect.right, adjustedRect.bottom, SWP_NOZORDER | SWP_NOREDRAW);

                        InvalidateRect(hWnd, &prevButtonRect, TRUE);
                        prevButtonRect = adjustedRect;
                        InvalidateRect(hButton, NULL, TRUE);
                        UpdateWindow(hButton);
                    }
                }
            }
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    /*case WM_ERASEBKGND:
        return 1;*/
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
           /* POINT dstSize = { 0 };
            dstSize.x = 69;
            SIZE size;
            size.cx = 40;
            UpdateLayeredWindow(hWndButton, hdc, &dstSize, &size, )*/

            //RECT rc;
            //GetClientRect(hWnd, &rc);

            //// Create a semi-transparent brush if you want some opacity
            //HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
            //FillRect(hdc, &rc, hBrush);
            //DeleteObject(hBrush);
            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

