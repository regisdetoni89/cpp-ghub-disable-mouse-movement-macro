using System;
using System.Runtime.InteropServices;
using UnityEngine;

public class MouseBlockManager : MonoBehaviour
{
    // Import DLL functions
    [DllImport("injectedMouse")]
    private static extern bool StartMouseBlocking();

    [DllImport("injectedMouse")]
    private static extern void StopMouseBlocking();

    [DllImport("injectedMouse")]
    private static extern bool IsMouseBlockingActive();

    [DllImport("injectedMouse")]
    private static extern uint GetLastHookError();

    private void Start()
    {
        if(!IsMouseBlockingActive()){
            if (!StartMouseBlocking()){
                uint error = GetLastHookError();
                Debug.LogError($"Failed to start mouse blocking. Error code: {error}");
            }else{
                Debug.Log("Mouse blocking started successfully");
            }
        }
        
    }

    private void OnDestroy(){
        if(IsMouseBlockingActive()){
            StopMouseBlocking();
            Debug.Log("Mouse blocking stopped");
        }
    }

} 