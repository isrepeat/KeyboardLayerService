#include <Helpers/Win32/TrayWindow.h>
#include <Helpers/Logger.h>

#include "Interception/Actions/ExtendCapsLockFunctionalityAction.h"
#include "Interception/Actions/Win32KeySymbolReplaceAction.h"
#include "Interception/Actions/SimpleBlockAction.h"
#include "Interception/Actions/SimpleRemapAction.h"
#include "Interception/Rules/TrackDevicesAndKeysRule.h"

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
	static std::shared_ptr<Interception::KeyProcessor> GetFunctionKeysBlockingProcessor(
		std::wstring deviceIdMatch,
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeysRule>(
			deviceIdMatch,
			trackingKeys
		);

		auto action = std::make_shared<Interception::Actions::SimpleBlockAction>();

		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}


	static std::shared_ptr<Interception::KeyProcessor> GetExtendingCapsLockFunctionalityProcessor(
		std::wstring deviceIdMatch,
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeysRule>(
			deviceIdMatch,
			trackingKeys
		);

		auto action = std::make_shared<Interception::Actions::ExtendCapsLockFunctionalityAction>();
		
		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}


	static std::shared_ptr<Interception::KeyProcessor> GeReplacingKeyWithSymbolProcessor(
		std::wstring deviceIdMatch,
		Keyboard::Core::Enums::LogicalKey trackingKey,
		wchar_t replacedSymbol,
		Interception::KeyProcessor::ChainPolicy chainPolicy
	) {
		auto rule = std::make_shared<Interception::Rules::TrackDevicesAndKeyRule>(
			deviceIdMatch,
			trackingKey
		);

		auto action = std::make_shared<Interception::Actions::Win32KeySymbolReplaceAction>(replacedSymbol);

		return std::make_shared<Interception::KeyProcessor>(rule, action, chainPolicy);
	}
};


void KeybooardLayerRoutine(std::stop_token stopToken) {
	std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors;

	keyProcessors.push_back(KeyProcessorProvider::GetFunctionKeysBlockingProcessor( // TODO: must be applied for TECLAST.
		L"any",
		Keyboard::Core::LogicalKeyGroup::FunctionKeys(),
		Interception::KeyProcessor::ChainPolicy::AlwaysContinue
	));

	keyProcessors.push_back(KeyProcessorProvider::GetFunctionKeysBlockingProcessor(
		L"ACPI\\VEN_ATK", // Asus Vivobook pro 16 (N7600PC) keyboard
		Keyboard::Core::LogicalKeyGroup::Arrows() |
		Keyboard::Core::LogicalKeyGroup::Digits() |
		Keyboard::Core::LogicalKeyGroup::Numpad() |
		Keyboard::Core::LogicalKeyGroup::Letters() |
		Keyboard::Core::LogicalKeyGroup::Modifiers() |
		Keyboard::Core::LogicalKeyGroup::ControlKeys() |
		Keyboard::Core::LogicalKeyGroup::FunctionKeys(),
		Interception::KeyProcessor::ChainPolicy::AlwaysContinue
	));

	keyProcessors.push_back(KeyProcessorProvider::GetExtendingCapsLockFunctionalityProcessor( // TODO: must be applied for TECLAST.
		L"any",
		Keyboard::Core::LogicalKeyGroup::Letters(),
		Interception::KeyProcessor::ChainPolicy::StopOnHandled
	));

	keyProcessors.push_back(KeyProcessorProvider::GeReplacingKeyWithSymbolProcessor( // TODO: must be applied for TECLAST.
		L"any",
		Keyboard::Core::Enums::LogicalKey::Tilde,
		L'x',
		Interception::KeyProcessor::ChainPolicy::AlwaysContinue
	));

	auto engine = KeyboardLayerEngine(std::move(keyProcessors));
	engine.Run(stopToken);
}