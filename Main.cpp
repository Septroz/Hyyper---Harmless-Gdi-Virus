#include <windows.h>
#include <vector>
#include <string>
#include <ctime>
#include <cmath> // Needed for fabs

const int duration = 100 * 1000; // 100 seconds
const int delay = 10;            // milliseconds per frame

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

// === Fractal Injector ===
void drawBurningShipFractal(int originX, int originY, HDC hdc) {
    const int width = 200, height = 200;
    const int maxIter = 256;

    for (int py = 0; py < height; ++py) {
        for (int px = 0; px < width; ++px) {
            double x0 = (px - width / 2) * 3.0 / width;
            double y0 = (py - height / 2) * 3.0 / height;
            double x = 0, y = 0;
            int iter = 0;

            while (x * x + y * y < 4 && iter < maxIter) {
                double xtemp = x * x - y * y + x0;
                y = fabs(2 * x * y) + y0;
                x = fabs(xtemp);
                iter++;
            }

            COLORREF color = RGB(iter % 32 * 8, iter % 16 * 16, iter % 8 * 32);
            SetPixel(hdc, originX + px, originY + py, color);
        }
    }
}

// === Simulated Bouncing Cursor ===
void bounceCursor(DWORD startTime) {
    int x = screenWidth / 2, y = screenHeight / 2;
    int dx = 12, dy = 8;

    while (GetTickCount() - startTime < duration) {
        DWORD elapsed = GetTickCount() - startTime;

        // Bounce logic
        if (x <= 0 || x >= screenWidth) dx = -dx;
        if (y <= 0 || y >= screenHeight) dy = -dy;
        x += dx; y += dy;

        SetCursorPos(x, y);
        Sleep(delay);

        // === PHASES ===
        if (elapsed > 10000 && elapsed < 12000) {
            HWND hDesktop = GetDesktopWindow();
            HDC hdc = GetDC(hDesktop);
            BitBlt(hdc, 0, 0, screenWidth, screenHeight, hdc, 0, 0, NOTSRCCOPY);
            ReleaseDC(hDesktop, hdc);
        }

        if (elapsed > 30000 && elapsed < 50000) {
            HWND hwnd = CreateWindowEx(WS_EX_TOPMOST, "STATIC", "🔄", WS_VISIBLE | WS_POPUP,
                                       rand() % screenWidth, rand() % screenHeight, 100, 100,
                                       NULL, NULL, NULL, NULL);
            Sleep(150);
            DestroyWindow(hwnd);
        }

        if (elapsed > 50000 && (elapsed / 1000) % 1 == 0 && (rand() % 3 == 0)) {
            ShellExecuteA(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOWNORMAL);
            Sleep(1000);
        }

        // === Fractal Graffiti Mode ===
        if ((elapsed / 10000) % 2 == 1 && rand() % 50 == 0) {
            HWND hDesktop = GetDesktopWindow();
            HDC hdc = GetDC(hDesktop);
            drawBurningShipFractal(x, y, hdc);
            ReleaseDC(hDesktop, hdc);
        }

        if (elapsed >= duration) break;
    }
}

// === Final Message ===
void showFinale() {
    MessageBox(NULL, "This chaos is done.", "System Mischief Complete", MB_OK | MB_ICONINFORMATION);
}

int main() {
    srand((unsigned)time(0));
    DWORD startTime = GetTickCount();

    bounceCursor(startTime);
    showFinale();

    return 0;
}