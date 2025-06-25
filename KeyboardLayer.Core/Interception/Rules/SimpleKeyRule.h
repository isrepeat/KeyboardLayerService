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
	class SimpleKeyRule : public IKeyRule {
	public:
		SimpleKeyRule(
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



	//class CapsLockKeyRule : public IKeyRule {
	//public:
	//	CapsLockKeyRule(std::wstring deviceIdMatch)
	//		: deviceFilter{ std::make_shared<SimpleDeviceFilter>(std::move(deviceIdMatch)) }
	//		, interceptionKeyCodeMapper{} 
	//	{
	//		//auto _trackingKeys = Keyboard::Core::LogicalKeyGroup::Letters().GetKeys();

	//		this->trackingKeys = Keyboard::Core::LogicalKeyGroup::Letters().GetKeys();
	//	}

	//	bool IsMatch(const DeviceInfo& device, const InterceptionKeyStroke& keyStroke) const override {
	//		if (!this->deviceFilter->Accept(device)) {
	//			return false;
	//		}
	//		auto logicalKey = this->interceptionKeyCodeMapper.FromNative(keyStroke.code);
	//		return logicalKey && this->trackingKeys.contains(*logicalKey);
	//	}

	//private:
	//	std::shared_ptr<IDeviceFilter> deviceFilter;
	//	InterceptionKeyCodeMapper interceptionKeyCodeMapper;
	//	std::set<Keyboard::Core::Enums::LogicalKey> trackingKeys;
	//};
}