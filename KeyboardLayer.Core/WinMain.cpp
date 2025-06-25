#include <Helpers/Win32/TrayWindow.h>
#include <Helpers/Logger.h>

#include "Interception/Actions/SimpleBlockAction.h"
#include "Interception/Actions/SimpleRemapAction.h"
#include "Interception/Actions/Win32SendAction.h"
#include "Interception/Rules/SimpleKeyRule.h"

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




void KeybooardLayerRoutine(std::stop_token stopToken) {
	std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors;

	{
		auto trackingKeysGroup =
			Keyboard::Core::LogicalKeyGroup::Arrows() |
			Keyboard::Core::LogicalKeyGroup::Digits() |
			Keyboard::Core::LogicalKeyGroup::Numpad() |
			Keyboard::Core::LogicalKeyGroup::Letters() |
			Keyboard::Core::LogicalKeyGroup::Modifiers() |
			Keyboard::Core::LogicalKeyGroup::ControlKeys() |
			Keyboard::Core::LogicalKeyGroup::FunctionKeys();

		auto rule = std::make_shared<Interception::Rules::SimpleKeyRule>(
			L"ACPI\\VEN_ATK", // Asus Vivobook pro 16 (N7600PC) keyboard
			trackingKeysGroup.GetKeys()
		);

		auto action = std::make_shared<Interception::Actions::SimpleBlockAction>();

		keyProcessors.push_back(std::make_shared<Interception::KeyProcessor>(rule, action));
	}

	{
		auto trackingKeysGroup = Keyboard::Core::LogicalKeyGroup::Letters();

		auto rule = std::make_shared<Interception::Rules::SimpleKeyRule>(
			L"any",
			trackingKeysGroup.GetKeys()
		);

		auto action = std::make_shared<Interception::Actions::ExtendFunctionalityAction>();

		keyProcessors.push_back(std::make_shared<Interception::KeyProcessor>(rule, action));
	}


	auto engine = KeyboardLayerEngine(std::move(keyProcessors));
	engine.Run(stopToken);
}