#pragma once
#include "Interception/InterceptionKeyCodeMapper.h"
#include "Interception/SimpleDeviceFilter.h"
#include "Interception/IDeviceFilter.h"

#include "Keyboard/Core/LogicalKeyGroup.h"
#include "Keyboard/Core/IKeyCodeMapper.h"
#include "IKeyRule.h"

#include <memory>
#include <string>
#include <set>

namespace Interception::Rules {
	class TrackDevicesAndKeysRule : public IKeyRule {
	public:
		TrackDevicesAndKeysRule(
			std::wstring deviceIdMatch,
			std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys)
			: deviceFilter{ std::make_shared<SimpleDeviceFilter>(std::move(deviceIdMatch)) }
			, trackingKeys{ std::move(trackingKeys) }
			, interceptionKeyCodeMapper{} {
		}

		bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const override {
			if (!this->deviceFilter->Accept(device)) {
				return false;
			}
			auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
			return logicalKey && this->trackingKeys.contains(*logicalKey);
		}

	private:
		std::shared_ptr<IDeviceFilter> deviceFilter;
		std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};



	class TrackDevicesAndKeyRule : public IKeyRule {
	public:
		TrackDevicesAndKeyRule(
			std::wstring deviceIdMatch,
			Keyboard::Core::Enums::LogicalKey trackingKey)
			: deviceFilter{ std::make_shared<SimpleDeviceFilter>(std::move(deviceIdMatch)) }
			, trackingKey{ trackingKey }
			, interceptionKeyCodeMapper{} {
		}

		bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const override {
			if (!this->deviceFilter->Accept(device)) {
				return false;
			}
			auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
			return logicalKey && this->trackingKey == logicalKey;
		}

	private:
		std::shared_ptr<IDeviceFilter> deviceFilter;
		Keyboard::Core::Enums::LogicalKey trackingKey;
		InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	};
}