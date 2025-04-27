#include "pch.h"
#include "UnityInterface.h"


BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
){
    switch (ul_reason_for_call){
    case DLL_PROCESS_ATTACH:
        StartMouseBlocking();
        break;
    case DLL_PROCESS_DETACH:
        StopMouseBlocking();
        break;
    }
    return TRUE;
}

