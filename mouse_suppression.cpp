// mouse_suppression.cpp: This file illustrates how to open up the G HUB BusEnumerator driver
// and request suppression of all mouse movement macros, for the duration of the process that
// invokes the IOCTL.

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <initguid.h>
#include <Windows.h>
#include <SetupAPI.h>

// Link against setupapi.lib
#pragma comment(lib, "setupapi.lib")

DEFINE_GUID(GUID_LOGI_VIRTUAL_BUS_ENUMERATOR_GP, 0xdfbedcdd, 0x2148, 0x416d, 0x9e, 0x4d, 0xce, 0xcc, 0x24, 0x24, 0x12,
            0x8c);

#define IOCTL_BUSENUM_SUPPRESS_MOUSE CTL_CODE(FILE_DEVICE_BUS_EXTENDER, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main()
{
    printf("This sample application will enable mouse suppression. As long as this executable is\n");
    printf("running, no mouse movement will be emulated by the driver.\n");

    HDEVINFO hardwareDeviceInfo =
        SetupDiGetClassDevs(&GUID_LOGI_VIRTUAL_BUS_ENUMERATOR_GP, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    SP_INTERFACE_DEVICE_DATA deviceInfoData;
    memset(&deviceInfoData, 0, sizeof(deviceInfoData));
    deviceInfoData.cbSize = sizeof(deviceInfoData);

    if (!SetupDiEnumDeviceInterfaces(hardwareDeviceInfo, 0, &GUID_LOGI_VIRTUAL_BUS_ENUMERATOR_GP, 0, &deviceInfoData))
    {
        printf("Failed get enumerate device interfaces.\n");
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
        return 1;
    }

    ULONG requiredLength = 0;
    SetupDiGetInterfaceDeviceDetail(hardwareDeviceInfo, &deviceInfoData, NULL, 0, &requiredLength, NULL);
    if (0 == requiredLength)
    {
        printf("Failed to get interface detail data length.\n");
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
        return 2;
    }

    std::vector<uint8_t> deviceDetailBuffer;
    deviceDetailBuffer.resize(requiredLength, 0x00);
    PSP_INTERFACE_DEVICE_DETAIL_DATA pDeviceDetailData =
        reinterpret_cast<PSP_INTERFACE_DEVICE_DETAIL_DATA>(deviceDetailBuffer.data());
    pDeviceDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
    if (!SetupDiGetInterfaceDeviceDetail(hardwareDeviceInfo, &deviceInfoData, pDeviceDetailData, requiredLength,
                                         &requiredLength, NULL))
    {
        printf("Failed to get interface detail data.\n");
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
        return 3;
    }
    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);

    HANDLE busEnumerator =
        CreateFile(pDeviceDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == busEnumerator)
    {
        printf("Failed to open up interface.\n");
        return 4;
    }

    BOOL ret = DeviceIoControl(busEnumerator, IOCTL_BUSENUM_SUPPRESS_MOUSE, NULL, 0, NULL, 0, NULL, NULL);
    DWORD error = GetLastError();
    printf("Suppressing mouse movement, GLE: 0x%08x\n", error);
    CloseHandle(busEnumerator);

    printf("Press any key to exit.\n");
    getchar();
    return 0;
}
