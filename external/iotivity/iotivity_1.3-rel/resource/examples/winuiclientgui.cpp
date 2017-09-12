/* ****************************************************************
 *
 * Copyright 2016 Intel Corporation All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <string>
#include "winuiclient.h"

#include <Windows.h>
#include <Commctrl.h>

#define ID_TRACKBAR             0x50505
#define ID_BUTTONPLAYPAUSE      0xFFF0
#define ID_BUTTONFIND           0xFFF1
#define ID_BUTTONGET            0xFFF2
#define ID_BUTTONPUT            0xFFF3
#define ID_BUTTONPOST           0xFFF4
#define ID_BUTTONBEGINOBSERVE   0xFFF5
#define ID_BUTTONCANCELOBSERVE  0xFFF6

int g_CurSliderVal = 0;

HWND g_hwnd, g_icon_button, g_hwndVolumeSlider, g_hwndVolumeExpectedLabel;
HWND hwndButtonPlayPause, hwndButtonFind, hwndButtonGet, hwndButtonPut, hwndButtonPost, hwndButtonBeginObserve, hwndButtonCancelObserve;
HANDLE Timer, Thread;
HICON hIcon1;
HBRUSH g_BkgndBrush;

WinUIClient::WinUIClientApp* app = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LabelPrintf (HWND hwndEdit, TCHAR * szFormat, ...);
HWND WINAPI CreateTrackbar(HWND parent, int x, int y, int w, int h,UINT iMin, UINT iMax, UINT iSelMin, UINT iSelMax);
HWND WINAPI CreateLabel(HWND parent, LPCTSTR lpText, int x, int y, int w, int h);
HWND WINAPI CreateButton(HWND parent, UINT_PTR id, LPCTSTR caption, int x, int y, int w, int h);

FILE* client_open(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen("./oic_svr_db_client.dat", mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE /* hPrevInstance */,
            LPSTR /* lpCmdLine */, int nCmdShow )
{
    MSG msg;
    WNDCLASS wc = {sizeof(WNDCLASS)};
    wc.lpszClassName = TEXT( "Static Control" );
    wc.hInstance     = hInstance ;
    wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
    wc.lpfnWndProc   = WndProc;
    wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);

    RegisterClass(&wc);
    g_hwnd = CreateWindow( wc.lpszClassName, TEXT("IoTivity Media Client - Windows UI"),
                   WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
                   100, 100, 275, 170, 0, 0, hInstance, 0);

    InitCommonControls(); // loads common control's DLL

    g_hwndVolumeSlider =
             CreateTrackbar(g_hwnd,
                            10,10,
                            170,40,
                            0,100,
                            0,0);
    g_hwndVolumeExpectedLabel =
                CreateLabel(g_hwnd,
                            "Expected Volume",
                            10,50,
                            240,30);

    LabelPrintf(g_hwndVolumeExpectedLabel,
                "Expected Volume: %i",
                0);

    hwndButtonPlayPause =
                CreateButton(g_hwnd,
                             ID_BUTTONPLAYPAUSE,
                             "PlayPause",
                             180,10,
                             80,35);

    hwndButtonFind =
                CreateButton(g_hwnd,
                             ID_BUTTONFIND,
                             "Find",
                             10,75,
                             64,25);

    hwndButtonGet =
                CreateButton(g_hwnd,
                             ID_BUTTONGET,
                             "Get",
                             10,105,
                             64,25);

    hwndButtonPut =
                CreateButton(g_hwnd,
                             ID_BUTTONPUT,
                             "Put",
                             80,75,
                             64,25);

    hwndButtonPost =
                CreateButton(g_hwnd,
                             ID_BUTTONPOST,
                             "Post",
                             80,105,
                             64,25);
    hwndButtonBeginObserve =
                CreateButton(g_hwnd,
                             ID_BUTTONBEGINOBSERVE,
                             "Bgn Obs",
                             150,75,
                             64,25);
    hwndButtonCancelObserve =
                CreateButton(g_hwnd,
                             ID_BUTTONCANCELOBSERVE,
                             "Cncl Obs",
                             150,105,
                             64,25);


    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT
CALLBACK
WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    HDC hDC;
    RECT rect;
    WinUIClient::Media myMedia;

    switch(msg){
      case WM_HSCROLL:
        switch (LOWORD(wParam)) {
            case TB_ENDTRACK:
                g_CurSliderVal = (int)SendMessage(g_hwndVolumeSlider, TBM_GETPOS, 0, 0);
                LabelPrintf(g_hwndVolumeExpectedLabel,"Volume: %i", g_CurSliderVal);

                myMedia = app->GetMedia();
                myMedia.m_volume = g_CurSliderVal;
                app->SetMedia(myMedia.m_state, myMedia.m_volume);
                app->PostMediaRepresentation();
                break;
        }
        break;
      case WM_CREATE:
          {
              hIcon1 = LoadIcon (NULL, IDI_WARNING);
              SendMessage(g_icon_button,BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon1);
              GetClientRect(hwnd, &rect);
              g_BkgndBrush = GetSysColorBrush(COLOR_MENU);

              OCPersistentStorage ps = {client_open, fread, fwrite, fclose, unlink };
              app = new WinUIClient::WinUIClientApp(ps);
              if (app->Initialize())
              {
                  app->Run();
              }
          }
          break;
      case WM_DESTROY:

          delete app;

          PostQuitMessage(0);
          break;
      case WM_ERASEBKGND:
          GetClientRect(hwnd, &rect);
          hDC = GetDC(hwnd);
          SelectObject((HDC)wParam, g_BkgndBrush);
          Rectangle((HDC)hDC, rect.left, rect.top, rect.right, rect.bottom);
          ReleaseDC(hwnd, hDC);
          break;
      case WM_SIZE:
          break;
      case WM_RBUTTONDOWN:
      case WM_LBUTTONDOWN:
          break;
      case WM_MOUSELEAVE:
      case WM_RBUTTONUP:
      case WM_LBUTTONUP:
          break;
      case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {

            switch (LOWORD(wParam))
            {
                case ID_BUTTONPLAYPAUSE:
                    myMedia = app->GetMedia();

                    // HACK: We are using m_state in a stateless way.
                    // True means "toggle".  The server does not have state
                    // knowledge about whether or not media is playing on the
                    // system.  It's dumb and hits the "PlayPause" key on the
                    // keyboard.
                    //
                    myMedia.m_state = true;
                    app->SetMedia(myMedia.m_state, myMedia.m_volume);
                    myMedia.m_state = false;

                    app->PutMediaRepresentation();
                    break;
                case ID_BUTTONFIND:
                    app->FindResources();
                    break;
                case ID_BUTTONGET:
                    app->GetMediaRepresentation();
                    break;
                case ID_BUTTONPUT:
                    myMedia = app->GetMedia();
                    myMedia.m_volume = g_CurSliderVal;
                    app->SetMedia(myMedia.m_state, myMedia.m_volume);
                    app->PutMediaRepresentation();
                    break;
                case ID_BUTTONPOST:
                    myMedia = app->GetMedia();
                    myMedia.m_volume = g_CurSliderVal;
                    app->SetMedia(myMedia.m_state, myMedia.m_volume);
                    app->PostMediaRepresentation();
                    break;
                case ID_BUTTONBEGINOBSERVE:
                    app->BeginObserving();
                    break;
                case ID_BUTTONCANCELOBSERVE:
                    app->CancelObserving();
                    break;
            }
        }
        break;
      default:
          return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE -6
#endif
HWND WINAPI CreateButton(HWND parent,
                            UINT_PTR id,
                            LPCTSTR caption    ,
                            int x,
                            int y,
                            int w,
                            int h
                            )
{
    HWND hwndButton = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed
        caption,      // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        x,         // x position
        y,         // y position
        w,        // Button width
        h,        // Button height
        parent,     // Parent window
        (HMENU)id,       // No menu.
        (HINSTANCE)GetWindowLongPtr(parent, GWL_HINSTANCE),
        NULL);      // Pointer not needed.

    return hwndButton;
}

HWND WINAPI CreateLabel(
    HWND parent,  // handle of parent window
    LPCTSTR lpText,
    int x,
    int y,
    int w,
    int h
)
{
   HWND hwndPowerLabel  = CreateWindow("static", "ST_U",
                                      WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                                      x, y, w, h,
                                      parent, (HMENU)(501),
                                      (HINSTANCE)GetWindowLongPtr(parent, GWL_HINSTANCE),
                                      NULL);
   SetWindowText(hwndPowerLabel, lpText);
   return hwndPowerLabel;
}

// CreateTrackbar - creates and initializes a trackbar.
//
// Global variable
//     g_hinstTrackBar - instance handle
//
HWND WINAPI CreateTrackbar(
    HWND parent,  // handle of dialog box (parent window)
    int x,
    int y,
    int w,
    int h,
    UINT iMin,     // minimum value in trackbar range
    UINT iMax,     // maximum value in trackbar range
    UINT iSelMin,  // minimum value in trackbar selection
    UINT iSelMax)  // maximum value in trackbar selection
{
    HWND hwndVolumeSlider = CreateWindowEx(
        0,                              // no extended styles
        TRACKBAR_CLASS,                 // class name
        "Trackbar Control",             // title (caption)
        WS_CHILD |
        WS_VISIBLE |
        TBS_AUTOTICKS |
        TBS_ENABLESELRANGE,             // style
        x,y,w,h,                        // position ,size
        parent,                        // parent window
        (HMENU)ID_TRACKBAR,             // control identifier
        (HINSTANCE)GetWindowLongPtr(parent, GWL_HINSTANCE),// instance
        NULL                            // no WM_CREATE parameter
        );

    SendMessage(hwndVolumeSlider, TBM_SETRANGE,
        (WPARAM) TRUE,                  // redraw flag
        (LPARAM) MAKELONG(iMin, iMax)); // min. & max. positions

    SendMessage(hwndVolumeSlider, TBM_SETPAGESIZE,
        0, (LPARAM) 4);                 // new page size

    SendMessage(hwndVolumeSlider, TBM_SETSEL,
        (WPARAM) FALSE,                 // redraw flag
        (LPARAM) MAKELONG(iSelMin, iSelMax));

    SendMessage(hwndVolumeSlider, TBM_SETPOS,
        (WPARAM) TRUE,                  // redraw flag
        (LPARAM) iSelMin);

    SetFocus(hwndVolumeSlider);

    return hwndVolumeSlider;
}

void LabelPrintf (HWND hwndEdit, TCHAR * szFormat, ...)
{
     TCHAR   szBuffer [1024] ;
     va_list pArgList ;

     va_start (pArgList, szFormat) ;
     wvsprintf (szBuffer, szFormat, pArgList) ;
     va_end (pArgList) ;

     SetWindowText(hwndEdit, szBuffer);
}

