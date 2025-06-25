#include <Helpers/Logger.h>
#include <Helpers/Win32/TrayWindow.h>

#include "Keyboard/Core/LogicalKeyGroup.h"

#include "Interception/InterceptionKeyCodeMapper.h"
#include "Interception/SimpleKeyRemapper.h"
#include "Interception/SimpleKeyBlocker.h"

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
	auto keyCodeMapper = std::make_shared<Interception::InterceptionKeyCodeMapper>();
	
	auto remapKeys = std::map<Keyboard::Core::Enums::LogicalKey, Keyboard::Core::Enums::LogicalKey>{
	};
	auto remapper = std::make_unique<Interception::SimpleKeyRemapper>(
		L"ACPI\\VEN_ATK", // TODO: replace with TECLAST keyboard
		remapKeys
	);

	auto blockingKeys =
		Keyboard::Core::LogicalKeyGroup::Arrows() |
		Keyboard::Core::LogicalKeyGroup::Digits() |
		Keyboard::Core::LogicalKeyGroup::Numpad() |
		Keyboard::Core::LogicalKeyGroup::Letters() |
		Keyboard::Core::LogicalKeyGroup::Modifiers() |
		Keyboard::Core::LogicalKeyGroup::ControlKeys() |
		Keyboard::Core::LogicalKeyGroup::FunctionKeys();

	auto blocker = std::make_unique<Interception::SimpleKeyBlocker>(
		L"ACPI\\VEN_ATK", // Asus Vivobook pro 16 (N7600PC) keyboard
		blockingKeys.GetKeys()
	);


	auto engine = KeyboardLayerEngine(
		std::move(blocker),
		std::move(remapper)
	);
	engine.Run(stopToken);
}