#include <windows.h>

// Ідентифікатори пунктів меню
#define ID_IMAGE_SQUARE      1001
#define ID_IMAGE_CIRCLE      1002
#define ID_TRANSFORM_COLOR   1003
#define ID_TRANSFORM_ENLARGE 1004
#define ID_INFO_ABOUT        1005
#define ID_INFO_EXIT         1006

// Стан програми
static int      figureType = 0;               // 0 — нічого, 1 — квадрат, 2 — коло
static COLORREF figureColor = RGB(0, 255, 0);  // колір фігури
static double   scale = 1.0;             // масштаб (варіант 17 — збільшення)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Створення меню: Image / Transformations / Info
HMENU CreateAppMenu()
{
    HMENU hMenu = CreateMenu();

    HMENU hImage = CreatePopupMenu();
    AppendMenuA(hImage, MF_STRING, ID_IMAGE_SQUARE, "Square");
    AppendMenuA(hImage, MF_STRING, ID_IMAGE_CIRCLE, "Circle");

    HMENU hTransform = CreatePopupMenu();
    AppendMenuA(hTransform, MF_STRING, ID_TRANSFORM_COLOR, "Change color");
    AppendMenuA(hTransform, MF_STRING, ID_TRANSFORM_ENLARGE, "Enlarge");

    HMENU hInfo = CreatePopupMenu();
    AppendMenuA(hInfo, MF_STRING, ID_INFO_ABOUT, "About");
    AppendMenuA(hInfo, MF_STRING, ID_INFO_EXIT, "Exit");

    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hImage, "Image");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hTransform, "Transformations");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hInfo, "Info");
    return hMenu;
}

// Малювання фігури
void DrawImage(HDC hdc)
{
    HPEN   hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(figureColor);
    HPEN   oldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // Центр фіксований, змінюється лише половина сторони
    const int cx = 250, cy = 250;
    int half = static_cast<int>(100 * scale);

    if (figureType == 1)
        Rectangle(hdc, cx - half, cy - half, cx + half, cy + half); // квадрат
    else if (figureType == 2)
        Ellipse(hdc, cx - half, cy - half, cx + half, cy + half);   // коло

    // Повертаємо старі об'єкти й видаляємо свої
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // Реєстрація класу вікна
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = "Lab3Task2";
    RegisterClassExA(&wc);

    // Створення вікна з меню
    HWND hwnd = CreateWindowExA(0, "Lab3Task2", "Lab 3 - Task 2",
        WS_OVERLAPPEDWINDOW, 300, 100, 700, 600,
        nullptr, CreateAppMenu(), hInstance, nullptr);
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл повідомлень
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
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_IMAGE_SQUARE:
            figureType = 1;
            scale = 1.0;                         // нова фігура — скидаємо розмір
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_IMAGE_CIRCLE:
            figureType = 2;
            scale = 1.0;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_TRANSFORM_COLOR:
            figureColor = RGB(255, 0, 0);        // змінюємо колір на червоний
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_TRANSFORM_ENLARGE:               // варіант 17: збільшення
            if (figureType != 0 && scale < 2.0)  // обмеження, щоб не вийти за вікно
            {
                scale += 0.2;
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            break;
        case ID_INFO_ABOUT:
            MessageBoxA(hwnd, "Lab 3, variant 17\nTask 2: menu and enlarge",
                "About", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_INFO_EXIT:
            DestroyWindow(hwnd);
            break;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawImage(hdc);
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