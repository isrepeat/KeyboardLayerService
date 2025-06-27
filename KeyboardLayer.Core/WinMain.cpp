#include <Helpers/Win32/TrayWindow.h>
#include <Helpers/Logger.h>

#include "Interception/Actions/ExtendCapsLockFunctionalityAction.h"
#include "Interception/Actions/Win32KeySymbolReplaceAction.h"
#include "Interception/Actions/SimpleBlockAction.h"
#include "Interception/Actions/SimpleRemapAction.h"
#include "Interception/Rules/TrackDevicesAndKeysRule.h"

#include "Keyboard/Platform/KeyboardDeviceInfo.h"
#include "Keyboard/Platform/KeyboardEnumerator.h"
#include "Keyboard/Core/LogicalKeyGroup.h"
#include "KeyboardLayerEngine.h"

#include <shellapi.h>
#include <windows.h>

#pragma comment (lib, "interception.lib")


void KeybooardLayerRoutine(std::stop_token stopToken);


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	std::jthread workRoutineThread(::KeybooardLayerRoutine);

	try {
		HICON trayIcon = ::LoadIconW(nullptr, IDI_APPLICATION); // системная иконка
		auto trayWindow = H::Win32::TrayWindow{ hInstance, trayIcon, L"My Tray App" };

		Keyboard::Platform::KeyboardEnumerator::GetInstance().SetDevicesChangedCallback([](const Keyboard::Platform::KeyboardDeviceInfoList& devices) {
			LOG_DEBUG_D(L"DeviceList changed:");
			for (const auto& device : devices) {
				LOG_DEBUG_D(L"instanceId = {}, name = {}"
					, device->instanceId
					, device->name
				);
			}
			});

		trayWindow.AddMessageHook([](UINT msg, WPARAM w, LPARAM l) {
			Keyboard::Platform::KeyboardEnumerator::GetInstance().HandleWindowsMessage(msg, w, l);
			return false; // не перехватываем, просто пробрасываем
			});

		Keyboard::Platform::KeyboardEnumerator::GetInstance().StartMonitoring(trayWindow.GetHwnd());

		trayWindow.RunMessageLoop();
		workRoutineThread.request_stop();
	}
	catch (const std::exception& ex) {
		LOG_ERROR_D("ex = {}", ex.what());
		workRoutineThread.request_stop();
	}

	if (workRoutineThread.joinable())
		workRoutineThread.join();

	return 0;
}




class KeyProcessorProvider {
public:
	static std::shared_ptr<Interception::KeyProcessor> GetKeysBlockingProcessor(
		Keyboard::Platform::KeyboardDeviceInfoList trackingDevices,
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeysRule>(
			trackingDevices,
			trackingKeys
		);

		auto action = std::make_shared<Interception::Actions::SimpleBlockAction>();

		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}


	static std::shared_ptr<Interception::KeyProcessor> GetExtendingCapsLockFunctionalityProcessor(
		Keyboard::Platform::KeyboardDeviceInfoList trackingDevices,
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeysRule>(
			trackingDevices,
			trackingKeys
		);

		auto action = std::make_shared<Interception::Actions::ExtendCapsLockFunctionalityAction>();

		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}


	static std::shared_ptr<Interception::KeyProcessor> GeReplacingKeyWithSymbolProcessor(
		Keyboard::Platform::KeyboardDeviceInfoList trackingDevices,
		Keyboard::Core::Enums::LogicalKey trackingKey,
		wchar_t replacedSymbol,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeyRule>(
			trackingDevices,
			trackingKey
		);

		auto action = std::make_shared<Interception::Actions::Win32KeySymbolReplaceAction>(replacedSymbol);

		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}
};


void KeybooardLayerRoutine(std::stop_token stopToken) {
	const auto& allKeyboardDevices = Keyboard::Platform::KeyboardEnumerator::GetInstance().GetAllKeyboardDevices();

	auto acpiKeyboardDeviceASUS = Keyboard::Platform::KeyboardEnumerator::GetInstance().FindDeviceByHardwareIdSubstring(L"ACPI\\ATK3001");
	auto hidKeyboardDeviceTECLAST = Keyboard::Platform::KeyboardEnumerator::GetInstance().FindDeviceByHardwareIdSubstring(L"HID\\{00001124-0000-1000-8000-00805F9B34FB}_VID&000204E8_PID&7021&COL01");

	std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors;

	if (acpiKeyboardDeviceASUS) {
		keyProcessors.push_back(KeyProcessorProvider::GetKeysBlockingProcessor(
			{ acpiKeyboardDeviceASUS }, // Asus Vivobook pro 16 (N7600PC) keyboard
			Keyboard::Core::LogicalKeyGroup::Arrows() |
			Keyboard::Core::LogicalKeyGroup::Digits() |
			Keyboard::Core::LogicalKeyGroup::Numpad() |
			Keyboard::Core::LogicalKeyGroup::Letters() |
			Keyboard::Core::LogicalKeyGroup::Modifiers() |
			Keyboard::Core::LogicalKeyGroup::ControlKeys() |
			Keyboard::Core::LogicalKeyGroup::FunctionKeys(),
			Interception::KeyProcessor::ChainPolicy::AlwaysContinue
		));
	}

	if (hidKeyboardDeviceTECLAST) {
		keyProcessors.push_back(KeyProcessorProvider::GetKeysBlockingProcessor(
			{ hidKeyboardDeviceTECLAST },
			Keyboard::Core::LogicalKeyGroup::FunctionKeys(),
			Interception::KeyProcessor::ChainPolicy::AlwaysContinue
		));

		keyProcessors.push_back(KeyProcessorProvider::GetExtendingCapsLockFunctionalityProcessor(
			{ hidKeyboardDeviceTECLAST },
			Keyboard::Core::LogicalKeyGroup::Letters() -
			Keyboard::Core::Enums::LogicalKey::Tilde,
			Interception::KeyProcessor::ChainPolicy::AlwaysContinue
		));

		keyProcessors.push_back(KeyProcessorProvider::GeReplacingKeyWithSymbolProcessor(
			{ hidKeyboardDeviceTECLAST },
			Keyboard::Core::Enums::LogicalKey::Tilde,
			L'*',
			Interception::KeyProcessor::ChainPolicy::AlwaysContinue
		));
	}

	auto engine = KeyboardLayerEngine(std::move(keyProcessors));
	engine.Run(stopToken);
}