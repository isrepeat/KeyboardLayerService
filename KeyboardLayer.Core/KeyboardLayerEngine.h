#pragma once
#include "3rdParty/Interception/interception.h"
#include "IKeyRemapper.h"
#include "IKeyFilter.h"
#include "DeviceInfo.h"
#include <memory>

class KeyboardLayerEngine {
public:
	KeyboardLayerEngine(
		std::unique_ptr<IKeyFilter> keyFilter,
		std::unique_ptr<IKeyRemapper> keyRemapper);

	void Run();

private:
	std::unique_ptr<IKeyFilter> keyFilter;
	std::unique_ptr<IKeyRemapper> keyRemapper;

	InterceptionContext context;

private:
	std::wstring GetHardwareId(int device);
	DeviceInfo GetDeviceInfo(int device);
};