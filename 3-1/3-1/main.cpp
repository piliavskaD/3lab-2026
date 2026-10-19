#include <windows.h>
#include <string>


#define ID_EDIT_X        101
#define ID_EDIT_Y        102
#define ID_BUTTON        103
#define ID_STATIC_RESULT 104

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = "Lab3Task1";
    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowExA(0, "Lab3Task1",
        "f(x, y) = 2*x*y - 3", WS_OVERLAPPEDWINDOW,
        500, 300, 500, 350, nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEditX, hEditY, hButton, hResult;

    switch (message)
    {
    case WM_CREATE:
    {
        HINSTANCE hInst = reinterpret_cast<LPCREATESTRUCTA>(lParam)->hInstance;


        hEditX = CreateWindowExA(0, "EDIT", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            50, 70, 100, 25, hwnd,
            reinterpret_cast<HMENU>(ID_EDIT_X), hInst, nullptr);


        hEditY = CreateWindowExA(0, "EDIT", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            180, 70, 100, 25, hwnd,
            reinterpret_cast<HMENU>(ID_EDIT_Y), hInst, nullptr);


        hButton = CreateWindowExA(0, "BUTTON", "Calculate",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 120, 120, 30, hwnd,
            reinterpret_cast<HMENU>(ID_BUTTON), hInst, nullptr);

        hResult = CreateWindowExA(0, "STATIC", "0",
            WS_CHILD | WS_VISIBLE,
            180, 170, 200, 25, hwnd,
            reinterpret_cast<HMENU>(ID_STATIC_RESULT), hInst, nullptr);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON)
        {
            char bufX[50]{}, bufY[50]{};
            GetWindowTextA(hEditX, bufX, sizeof(bufX));
            GetWindowTextA(hEditY, bufY, sizeof(bufY));

            try
            {
                long long x = std::stoll(bufX);
                long long y = std::stoll(bufY);

       
                long long f = 2 * x * y - 3;

                SetWindowTextA(hResult, std::to_string(f).c_str());
            }
            catch (...) 
            {
                SetWindowTextA(hResult, "Input error");
            }
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        const char* t1 = "Enter x and y:";
        const char* t2 = "Result:";
        TextOutA(hdc, 50, 35, t1, lstrlenA(t1));
        TextOutA(hdc, 50, 170, t2, lstrlenA(t2));

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcA(hwnd, message, wParam, lParam);
    }
    return 0;
}