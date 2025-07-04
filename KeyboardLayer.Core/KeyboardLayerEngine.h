#pragma once
#include <3rdParty/Interception/interception.h>

#include "Interception/KeyProcessor.h"
#include "Interception/DeviceInfo.h"

#include <memory>
#include <vector>
#include <thread>

class KeyboardLayerEngine {
public:
	KeyboardLayerEngine(std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors);

	void Run(std::stop_token stopToken);

private:
	Interception::DeviceInfo GetDeviceInfo(InterceptionDevice device);
	std::wstring GetHardwareId(InterceptionDevice device);

	bool ApplyKeyProcessors(Interception::DeviceInfo, InterceptionKeyStroke& keyStrokeRef);

private:
	std::vector<std::shared_ptr<Interception::KeyProcessor>> keyProcessors;
	InterceptionContext context;

	std::unordered_map<std::wstring, Interception::DeviceInfo> cachedDeviceInfos;
};