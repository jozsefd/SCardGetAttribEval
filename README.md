# SCardGetAttribEval

Windows console application for checking the Windows API function SCardGetAttrib.

## Background

Smart Card Minidriver fails unexpectedly in VDI sessions initiated from macOS.
The problem was traced back to calls of SCardGetAttrib when the minidriver tries
to determine the name of the card reader device. 

The SCardGetAttrib is essential in a Windows Minidriver, because the minidriver receives
only the smart card connection handle and the name of the card reader device is required
in subsequent minidriver operations.

SCardGetAttrib provides multiple Attibute values for querying the card reader name,
this application tests the ascii and wide variations of SCARD_ATTR_DEVICE_SYSTEM_NAME and 
SCARD_ATTR_DEVICE_FRIENDLY_NAME:

- SCARD_ATTR_DEVICE_SYSTEM_NAME_A
- SCARD_ATTR_DEVICE_FRIENDLY_NAME_A
- SCARD_ATTR_DEVICE_SYSTEM_NAME_W
- SCARD_ATTR_DEVICE_FRIENDLY_NAME_W

## Test Results

### Windows 10 (Direct)

Testing API SCardGetAttrib with card in reader Generic Smart Card Reader Interface 0
 - System name (Ascii): Success
 - Friendly name (Ascii): Success
 - System name (Wide): Success
 - Friendly name (Wide): Success

### Windows 10 (VDI from Windows 10)

Testing API SCardGetAttrib with card in reader Generic Smart Card Reader Interface 0
- System name (Ascii): Success
- Friendly name (Ascii): Success
- System name (Wide): Success
- Friendly name (Wide): Success

### Windows 10 (VDI from Ubuntu 24.04)

Testing API SCardGetAttrib with card in reader Generic USB2.0-CRW [Smart Card Reader Interface] (20070818000000000) 00 00
- System name (Ascii): Success
- Friendly name (Ascii): Success
- System name (Wide): Failed with error -2146435041
- Friendly name (Wide): Failed with error -2146435041

### Windows 10 (VDI from macOS 14.7, Citrix Workspace 2405)

Testing API SCardGetAttrib with card in reader Generic USB2.0-CRW
- System name (Ascii): Failed with error -2146435038
- Friendly name (Ascii): Success
- System name (Wide): Failed with error -2146435038
- Friendly name (Wide): Success

### Windows 10 (VDI from macOS 14.7, Citrix Workspace 2411)

Testing API SCardGetAttrib with card in reader Generic USB2.0-CRW
- System name (Ascii): Failed with error -2146435041
- Friendly name (Ascii): Failed with error -2146435041
- System name (Wide): Failed with error -2146435041
- Friendly name (Wide): Failed with error -2146435041

## Error Info

-2146435041 (0x8010001F)

```
// MessageId: SCARD_E_UNEXPECTED
//
// MessageText:
//
// An unexpected card error has occurred.
//
#define SCARD_E_UNEXPECTED               _HRESULT_TYPEDEF_(0x8010001FL)
```

## Building

mkdir build
cd build
cmake .. -A x64 -G "Visual Studio 17"
cmake --build .
