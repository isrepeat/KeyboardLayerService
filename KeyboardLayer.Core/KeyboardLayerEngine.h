#pragma once
#include <3rdParty/Interception/interception.h>
#include "Interception/DeviceInfo.h"
#include "Interception/IKeyBlocker.h"
#include "Interception/IKeyRemapper.h"
#include <memory>
#include <thread>


class KeyboardLayerEngine {
public:
	KeyboardLayerEngine(
		std::unique_ptr<Interception::IKeyBlocker> keyBlocker,
		std::unique_ptr<Interception::IKeyRemapper> keyRemapper);

	void Run(std::stop_token stopToken);

private:
	std::wstring GetHardwareId(int device);
	Interception::DeviceInfo GetDeviceInfo(int device);

private:
	std::unique_ptr<Interception::IKeyBlocker> keyBlocker;
	std::unique_ptr<Interception::IKeyRemapper> keyRemapper;
	//std::unique_ptr<Interception::IKeyInjector> keyInjector;

	InterceptionContext context;
};