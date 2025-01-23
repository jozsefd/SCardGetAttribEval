//
// SCardGetAttribEval.cpp
// Simple Windows console application for checking the Windows API function SCardGetAttrib.
// 
// Background:
// The SCardGetAttrib is essential in a Windows Minidriver, because the minidriver receives
// only the smart card connection handle, and the name of the card reader device is required
// in subsequent minidriver operations.
// SCardGetAttrib provides multiple Attibute values with the card reader name, this application
// tests the ascii and wide variations of SCARD_ATTR_DEVICE_SYSTEM_NAME and 
// SCARD_ATTR_DEVICE_FRIENDLY_NAME 
//

#include <windows.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Winscard")

int main() {
	std::cout << "Windows SmartCard API Evaluation" << std::endl;
	std::cout << "This console application checks the API SCardGetAttrib and it requires a plugged smart card" << std::endl;

	SCARDCONTEXT context = 0;
	LPSTR mszReaders = nullptr;
	try {
		LONG rc = ::SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context);
		if (rc != SCARD_S_SUCCESS) {
			throw std::string{ "Failed to establish SCard context: " } + std::to_string(rc);
		}

		DWORD dwLen = SCARD_AUTOALLOCATE;
		rc = ::SCardListReadersA(context, NULL, (LPSTR)&mszReaders, &dwLen);
		if (rc == SCARD_E_NO_READERS_AVAILABLE) {
			throw std::string{ "No smart card reader found. Please connect a device and restart the application" };
		}
		if (rc != SCARD_S_SUCCESS) {
			throw std::string{ "Failed to list available readers: " }  + std::to_string(rc);
		}
		
		LPSTR readerName = mszReaders;
		while (readerName != nullptr && *readerName != 0) {
			SCARD_READERSTATEA state{readerName, nullptr, 0, 0};
			rc = ::SCardGetStatusChangeA(context, 0, &state, 1);
			if (rc != SCARD_S_SUCCESS) {
				throw std::string{ "Failed to get reader state: " }  + std::to_string(rc);
			}
			if (state.dwEventState & SCARD_STATE_PRESENT) {
				// we have a card
				std::cout << "Testing API SCardGetAttrib with card in reader " << readerName << std::endl;

				SCARDHANDLE card;
				DWORD activeProtocol;
				rc = ::SCardConnectA(context, readerName, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &card, &activeProtocol);
				if (rc != SCARD_S_SUCCESS) {
					throw std::string{ "Failed to connect smart card: " }  + std::to_string(rc);
				}

				//==================================================
				// this code fails on macOS VDI {
				//==================================================
				struct Attribute {
					const char* name;
					DWORD value;
				};
				Attribute attributes[] = {
					{ "System name (Ascii)", SCARD_ATTR_DEVICE_SYSTEM_NAME_A },
					{ "Friendly name (Ascii)", SCARD_ATTR_DEVICE_FRIENDLY_NAME_A },
					{ "System name (Wide)", SCARD_ATTR_DEVICE_SYSTEM_NAME_W },
					{ "Friendly name (Wide)", SCARD_ATTR_DEVICE_FRIENDLY_NAME_W },
				};
				for (const auto& attribute : attributes) {
					// testing 
					DWORD size = 0;
					rc = ::SCardGetAttrib(card, attribute.value, nullptr, &size);
					if (rc == SCARD_S_SUCCESS) {
						BYTE* data = new BYTE[size];
						rc = ::SCardGetAttrib(card, attribute.value, data, &size);
						delete[] data;
					}
					std::cout << " - " << attribute.name << ": ";
					if (rc == SCARD_S_SUCCESS) {
						std::cout << "Success";
					} else {
						std::cout << "Failed with error " << rc;
					}
					std::cout << std::endl;
				}
				//==================================================
				// } this code fails on macOS VDI
				//==================================================

				::SCardDisconnect(card, SCARD_LEAVE_CARD);
			} else {
				std::cout << "Please plug a card into " << readerName << " and restart the application" << std::endl;
			}
			readerName += strlen(readerName) + 1;
		}
	} catch (const std::string& errorMessage) {
		std::cout << "Error: " << errorMessage << std::endl;
	}
	if ( mszReaders != nullptr ) ::SCardFreeMemory(context, mszReaders);
	if ( context != 0 ) ::SCardReleaseContext(context);

	system("pause");
	return EXIT_SUCCESS;
}

