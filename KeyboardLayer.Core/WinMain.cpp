#include <Helpers/Win32/MainWindow.h>
#include <Helpers/Win32/TrayWindow.h>
#include <Helpers/Logger.h>

#include "KeyboardLayerEngine.h"
#include "InterceptionKeyCodeMapper.h"
#include "SimpleKeyRemapper.h"
#include "SimpleKeyBlocker.h"
#include "LogicalKeyGroup.h"

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
	auto keyCodeMapper = std::make_shared<InterceptionKeyCodeMapper>();
	// TODO: сделай так чтоб тильда всегда писалась без SHIFT
	auto remapKeys = std::map<LogicalKey, LogicalKey>{

		//{ LogicalKey::T, LogicalKey::Digit6 },
		//{ LogicalKey::U, LogicalKey::Digit7 },
	};
	auto remapper = std::make_unique<SimpleKeyRemapper>(
		remapKeys,
		keyCodeMapper
	);


	//auto blockingKeys = std::set<LogicalKey>{
	//	LogicalKey::W,
	//	LogicalKey::T,
	//	LogicalKey::Space,
	//};
	auto blockingKeys =
		LogicalKeyGroup::Arrows() |
		LogicalKeyGroup::Digits() |
		LogicalKeyGroup::Numpad() |
		LogicalKeyGroup::Letters() |
		LogicalKeyGroup::Modifiers() |
		LogicalKeyGroup::ControlKeys() |
		LogicalKeyGroup::FunctionKeys();

	auto blocker = std::make_unique<SimpleKeyBlocker>(
		L"ACPI\\VEN_ATK", // Asus Vivobook pro 16 (N7600PC) keyboard
		blockingKeys.GetKeys(),
		keyCodeMapper
	);


	auto engine = KeyboardLayerEngine(
		std::move(blocker),
		std::move(remapper)
	);
	engine.Run(stopToken);
}