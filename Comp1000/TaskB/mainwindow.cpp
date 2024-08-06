#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define ID_OPEN 1
#define ID_SEARCH 2
#define ID_SAVE 3

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::string fileContent;
    static std::string searchTerm;
    static std::vector<std::string> searchResults;
    static int hitCount;

    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_OPEN: {
            OPENFILENAME ofn;
            char szFile[260];
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "Text files\0*.TXT\0All files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                std::ifstream file(ofn.lpstrFile);
                if (file.is_open()) {
                    std::ostringstream oss;
                    oss << file.rdbuf();
                    fileContent = oss.str();
                }
            }
            break;
        }
        case ID_SEARCH: {
            std::string line;
            std::istringstream iss(fileContent);
            searchResults.clear();
            hitCount = 0;
            while (std::getline(iss, line)) {
                if (line.find(searchTerm) != std::string::npos) {
                    searchResults.push_back(line);
                    ++hitCount;
                }
            }
            break;
        }
        case ID_SAVE: {
            OPENFILENAME ofn;
            char szFile[260];
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "CSV files\0*.CSV\0All files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileName(&ofn)) {
                std::ofstream file(ofn.lpstrFile);
                file << "FileName,SearchTerm,HitCount\n";
                file << "example.txt," << searchTerm << "," << hitCount << "\n";
            }
            break;
        }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Sample Windows Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
