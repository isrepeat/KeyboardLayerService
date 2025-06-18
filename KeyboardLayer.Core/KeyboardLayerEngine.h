#pragma once
#include "KeyFilter.h"
#include <memory>
#include <vector>

class KeyboardLayerEngine {
public:
	explicit KeyboardLayerEngine(std::unique_ptr<KeyFilter> filter);
	void Run();

private:
	std::unique_ptr<KeyFilter> keyFilter;
	InterceptionContext context;

	std::wstring GetHardwareId(int device);
	DeviceInfo GetDeviceInfo(int device);
};