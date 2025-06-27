#pragma once
#include <Helpers/Ranges.h>

#include "Interception/InterceptionKeyCodeMapper.h"

#include "Keyboard/Platform/KeyboardEnumerator.h"
#include "Keyboard/Platform/KeyboardDeviceInfo.h"
#include "Keyboard/Core/LogicalKeyGroup.h"
#include "Keyboard/Core/IKeyCodeMapper.h"
#include "IKeyRule.h"

#include <algorithm>
#include <memory>
#include <string>
#include <set>

namespace Interception::Rules {
	class TrackDevicesAndKeysRule : public IKeyRule {
	public:
		TrackDevicesAndKeysRule(
			Keyboard::Platform::KeyboardDeviceInfoList trackingDevices,
			std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys)
			: trackingDevices{ std::move(trackingDevices) }
			, trackingKeys{ std::move(trackingKeys) }
			, interceptionKeyCodeMapper{} {
		}

		bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const override {
			if (!this->trackingDevices.contains(device.keyboardDeviceInfo)) {
				return false;
			}

			auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
			return logicalKey && this->trackingKeys.contains(*logicalKey);
		}

	private:
		Keyboard::Platform::KeyboardDeviceInfoList trackingDevices;
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};



	class TrackDevicesAndKeyRule : public IKeyRule {
	public:
		TrackDevicesAndKeyRule(
			Keyboard::Platform::KeyboardDeviceInfoList trackingDevices,
			Keyboard::Core::Enums::LogicalKey trackingKey)
			: trackingDevices{ std::move(trackingDevices) }
			, trackingKey{ trackingKey }
			, interceptionKeyCodeMapper{} {
		}

		bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const override {
			if (!this->trackingDevices.contains(device.keyboardDeviceInfo)) {
				return false;
			}

			auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
			return logicalKey && this->trackingKey == logicalKey;
		}

	private:
		Keyboard::Platform::KeyboardDeviceInfoList trackingDevices;
		Keyboard::Core::Enums::LogicalKey trackingKey;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};
}