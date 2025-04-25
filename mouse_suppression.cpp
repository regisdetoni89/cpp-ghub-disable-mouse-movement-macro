#include <windows.h>
#include <iostream>

// Function to block mouse input if it's injected
bool BlockInjectedMouseInput() {
    INPUT_MESSAGE_SOURCE source;
    if (GetCurrentInputMessageSource(&source)) {
        if (source.originId == IMO_INJECTED) {
            std::cout << "Blocked injected input (originId: " << source.originId << ")" << std::endl;
            return true;
        }
    }
    else {
        std::cout << "Failed to get input message source" << std::endl;
    }
    return false;
}

// Low-level mouse hook procedure
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;

        // Check if the input is injected using the flags
        if (mouseStruct->flags & LLMHF_INJECTED || mouseStruct->flags & LLMHF_LOWER_IL_INJECTED) {
            std::cout << "Blocked input based on LLMHF flags" << std::endl;
            return 1;
        }

        if (BlockInjectedMouseInput()) {
            return 1;
        }

    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Set up the mouse hook
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);

    if (mouseHook == NULL) {
        DWORD error = GetLastError();
        std::cerr << "Failed to set mouse hook. Error code: " << error << std::endl;
        return 1;
    }

    std::cout << "Mouse input blocking is active. Press Ctrl+C to exit." << std::endl;
    std::cout << "Debug logging is enabled - you will see details about input events." << std::endl;

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    UnhookWindowsHookEx(mouseHook);
    return 0;
}