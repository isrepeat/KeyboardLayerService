#include "KeyboardEnumerator.h"
#include <Helpers/Logger.h>
#include <Helpers/Memory.h>

#include "KeyboardParser.h"
#include <windows.h> 
#include <setupapi.h> // must be included after <windows.h>
#include <devguid.h>
#include <iostream>
#include <regstr.h>
#include <dbt.h>

#pragma comment(lib, "setupapi.lib")


namespace Keyboard::Platform {
	const KeyboardDeviceInfoList& KeyboardEnumerator::GetAllKeyboardDevices() {
		if (!this->initialized) {
			this->UpdateInternalList();
			this->initialized = true;
		}
		return this->cachedDevices;
	}

	std::shared_ptr<KeyboardDeviceInfo> KeyboardEnumerator::FindDeviceByHardwareIdSubstring(const std::wstring& substring) const {
		for (const auto& dev : this->cachedDevices) {
			if (dev->hardwareId.find(substring) != std::wstring::npos) {
				return dev;
			}
		}
		return nullptr;
	}


	void KeyboardEnumerator::SetDevicesChangedCallback(const DevicesChangedCallback& callback) {
		this->onDevicesChanged = callback;
	}


	// Запускает мониторинг системных событий об изменении устройств клавиатуры.
	// Требуется передать HWND окна, которое будет получать WM_DEVICECHANGE.
	void KeyboardEnumerator::StartMonitoring(HWND hwnd) {
		DEV_BROADCAST_DEVICEINTERFACE_W filter = {};
		filter.dbcc_size = sizeof(filter);
		filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		filter.dbcc_classguid = GUID_DEVCLASS_KEYBOARD;

		this->deviceNotificationHandle = RegisterDeviceNotificationW(
			hwnd,
			&filter,
			DEVICE_NOTIFY_WINDOW_HANDLE
		);

		if (!this->deviceNotificationHandle) {
			std::wcerr << L"RegisterDeviceNotification failed\n";
		}
	}


	// Обрабатывает системные сообщения Windows, связанные с изменением устройств.
	void KeyboardEnumerator::HandleWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam) {
		if (message == WM_DEVICECHANGE) {
			// Отслеживаем только добавление и удаление устройств
			if (wParam == DBT_DEVICEARRIVAL ||
				wParam == DBT_DEVICEREMOVECOMPLETE) {
				this->NotifyIfChanged();
			}
		}
	}


	// Сравнивает текущий список устройств с предыдущим.
	// Если есть изменения — вызывает пользовательский колбэк.
	void KeyboardEnumerator::NotifyIfChanged() {
		KeyboardDeviceInfoList previous = this->cachedDevices;

		this->UpdateInternalList();

		if (this->onDevicesChanged && previous != this->cachedDevices) {
			this->onDevicesChanged(this->cachedDevices);
		}
	}


	void KeyboardEnumerator::UpdateInternalList() {
		this->cachedDevices.clear();

		HDEVINFO deviceInfoSet = ::SetupDiGetClassDevsW(
			&GUID_DEVCLASS_KEYBOARD,
			nullptr,
			nullptr,
			DIGCF_PRESENT
		);

		if (deviceInfoSet == INVALID_HANDLE_VALUE) {
			std::wcerr << L"SetupDiGetClassDevs failed\n";
			return;
		}

		SP_DEVINFO_DATA deviceInfoData;
		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		for (DWORD index = 0;
			SetupDiEnumDeviceInfo(deviceInfoSet, index, &deviceInfoData);
			++index) {
			wchar_t nameBuffer[512] = {};
			wchar_t instanceBuffer[512] = {};

			// Получаем имя (не обязательно, но можно сохранить позже)
			::SetupDiGetDeviceRegistryPropertyW(
				deviceInfoSet,
				&deviceInfoData,
				SPDRP_DEVICEDESC,
				nullptr,
				reinterpret_cast<PBYTE>(nameBuffer),
				sizeof(nameBuffer),
				nullptr
			);

			if (!::SetupDiGetDeviceInstanceIdW(
				deviceInfoSet,
				&deviceInfoData,
				instanceBuffer,
				sizeof(instanceBuffer),
				nullptr)) {
				continue;
			}

			// Формируем базовую структуру
			KeyboardDeviceInfo baseKeyboardDeviceInfo;
			baseKeyboardDeviceInfo.instanceId = instanceBuffer;
			baseKeyboardDeviceInfo.hardwareId = L""; // будет сгенерировано в парсере

			// Пробуем распарсить ACPI
			if (auto acpiKeyboardDeviceInfo = KeyboardParser::TryParseAcpiKeyboard(baseKeyboardDeviceInfo)) {
				//this->cachedDevices.insert(std::make_unique<AcpiKeyboardInfo>(*acpiKeyboardDeviceInfo));
				this->cachedDevices.insert(
					std::static_pointer_cast<KeyboardDeviceInfo>(
						std::make_shared<AcpiKeyboardInfo>(*acpiKeyboardDeviceInfo)
					)
				);
				continue;
			}

			// Пробуем распарсить HID
			if (auto hidKeyboardDeviceInfo = KeyboardParser::TryParseHidKeyboard(baseKeyboardDeviceInfo)) {
				//this->cachedDevices.insert(std::make_unique<HidKeyboardInfo>(*hidKeyboardDeviceInfo));
				this->cachedDevices.insert(
					std::static_pointer_cast<KeyboardDeviceInfo>(
						std::make_shared<HidKeyboardInfo>(*hidKeyboardDeviceInfo)
					)
				);
				continue;
			}
		}

		::SetupDiDestroyDeviceInfoList(deviceInfoSet);
	}

}