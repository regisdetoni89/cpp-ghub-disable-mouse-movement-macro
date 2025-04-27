#pragma once

#ifdef INJECTEDMOUSE_EXPORTS
#define INJECTEDMOUSE_API __declspec(dllexport)
#else
#define INJECTEDMOUSE_API __declspec(dllimport)
#endif

extern "C" {
    // Mouse blocking functions
    INJECTEDMOUSE_API bool StartMouseBlocking();
    INJECTEDMOUSE_API void StopMouseBlocking();
    INJECTEDMOUSE_API bool IsMouseBlockingActive();
    INJECTEDMOUSE_API DWORD GetLastHookError();
    
} 