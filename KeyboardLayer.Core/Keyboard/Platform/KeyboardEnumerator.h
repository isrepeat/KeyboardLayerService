#pragma once
#include <Helpers/Singleton.hpp>
#include "KeyboardDeviceInfo.h"
#include <functional>
#include <string>
#include <vector>
#include <set>

namespace Keyboard::Platform {
	class KeyboardEnumerator : public H::_Singleton<class KeyboardEnumerator> {
	private:
		using _MyBase = H::_Singleton<class KeyboardEnumerator>;
		friend _MyBase; // to have access to private Ctor

	public:
		using DevicesChangedCallback = std::function<void(const KeyboardDeviceInfoList&)>;

	public:
		const KeyboardDeviceInfoList& GetAllKeyboardDevices();
		std::shared_ptr<KeyboardDeviceInfo> FindDeviceByHardwareIdSubstring(const std::wstring& substring) const;

		void SetDevicesChangedCallback(const DevicesChangedCallback& callback);
		void StartMonitoring(HWND hwnd);
		void HandleWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		KeyboardEnumerator() = default;
		KeyboardEnumerator(const KeyboardEnumerator&) = delete;
		KeyboardEnumerator& operator=(const KeyboardEnumerator&) = delete;

		void UpdateInternalList();
		void NotifyIfChanged();

	private:
		KeyboardDeviceInfoList cachedDevices;
		bool initialized = false;
		DevicesChangedCallback onDevicesChanged;
		HDEVNOTIFY deviceNotificationHandle = nullptr;
	};
}