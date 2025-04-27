#include "pch.h"
#include "UnityInterface.h"
#include <windows.h>
#include <iostream>

// Global variables for mouse hook
HHOOK g_mouseHook = NULL;
bool g_isBlockingActive = false;
DWORD g_lastHookError = 0;

// Low-level mouse hook procedure
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
        // Check if the input is injected using the flags
        if (mouseStruct->flags & LLMHF_INJECTED || mouseStruct->flags & LLMHF_LOWER_IL_INJECTED) {
            return 1;
        }

        INPUT_MESSAGE_SOURCE source;
        if (GetCurrentInputMessageSource(&source)) {
            if (source.originId == IMO_INJECTED) {
                return 1;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Start mouse blocking
bool StartMouseBlocking() {
    if (g_mouseHook != NULL) {
        g_lastHookError = ERROR_ALREADY_EXISTS;
        return false; // Already active
    }

    g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);
    if (g_mouseHook == NULL) {
        g_lastHookError = GetLastError();
        return false;
    }

    g_isBlockingActive = true;
    g_lastHookError = 0; // Reset error on success
    return true;
}

// Stop mouse blocking
void StopMouseBlocking() {
    if (g_mouseHook != NULL) {
        if (!UnhookWindowsHookEx(g_mouseHook)) {
            g_lastHookError = GetLastError();
        }
        g_mouseHook = NULL;
    }
    g_isBlockingActive = false;
}

// Check if mouse blocking is active
bool IsMouseBlockingActive() {
    return g_isBlockingActive;
}

// Get the last error code from hook operations
DWORD GetLastHookError() {
    return g_lastHookError;
}
