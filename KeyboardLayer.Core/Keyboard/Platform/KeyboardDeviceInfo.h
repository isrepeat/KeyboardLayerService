#pragma once
#include <Helpers/Singleton.hpp>
#include <functional>
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace Keyboard::Platform {
	struct KeyboardDeviceInfo {
		std::wstring instanceId;   // полный путь устройства (из SetupAPI)
		std::wstring hardwareId;   // hardwareId (может быть тем же, что instanceId или приходить из событи€)
		std::wstring name;

		virtual ~KeyboardDeviceInfo() = default;

		// ¬иртуальные методы сравнени€
		virtual bool IsEqualTo(const KeyboardDeviceInfo& other) const {
			return this->hardwareId == other.hardwareId;
		}

		virtual bool IsLessThan(const KeyboardDeviceInfo& other) const {
			return this->hardwareId < other.hardwareId;
		}

		bool operator==(const KeyboardDeviceInfo& other) const {
			return this->IsEqualTo(other);
		}

		bool operator<(const KeyboardDeviceInfo& other) const {
			return this->IsLessThan(other);
		}
	};

	struct AcpiKeyboardInfo : KeyboardDeviceInfo {
		std::wstring venCode;
		std::wstring devCode;

		bool IsEqualTo(const KeyboardDeviceInfo& other) const override {
			if (const auto* acpi = dynamic_cast<const AcpiKeyboardInfo*>(&other)) {
				return this->venCode == acpi->venCode
					&& this->devCode == acpi->devCode;
			}
			return false;
		}

		bool IsLessThan(const KeyboardDeviceInfo& other) const override {
			if (const auto* acpi = dynamic_cast<const AcpiKeyboardInfo*>(&other)) {
				if (this->venCode != acpi->venCode) {
					return this->venCode < acpi->venCode;
				}
				return this->devCode < acpi->devCode;
			
			}
			// — другим типом сравниваем по hardwareId
			return this->hardwareId < other.hardwareId;
		}
	};

	struct HidKeyboardInfo : KeyboardDeviceInfo {
		std::wstring vid;
		std::wstring pid;
		std::optional<std::wstring> btGuid;
		std::optional<std::wstring> collectionIndex;

		bool IsEqualTo(const KeyboardDeviceInfo& other) const override {
			if (const auto* hid = dynamic_cast<const HidKeyboardInfo*>(&other)) {
				return this->vid == hid->vid
					&& this->pid == hid->pid
					&& this->btGuid == hid->btGuid
					&& this->collectionIndex == hid->collectionIndex;
			}
			return false;
		}

		bool IsLessThan(const KeyboardDeviceInfo& other) const override {
			if (const auto* hid = dynamic_cast<const HidKeyboardInfo*>(&other)) {
				if (this->vid != hid->vid) {
					return this->vid < hid->vid;
				}
				if (this->pid != hid->pid) {
					return this->pid < hid->pid;
				}
				if (this->btGuid != hid->btGuid) {
					return this->btGuid < hid->btGuid;
				}
				return this->collectionIndex < hid->collectionIndex;
			}

			// — другим типом сравниваем по hardwareId
			return this->hardwareId < other.hardwareId;
		}
	};

	namespace details {
		struct KeyboardDevicePtrLess {
			bool operator()(const std::shared_ptr<KeyboardDeviceInfo>& a, const std::shared_ptr<KeyboardDeviceInfo>& b) const {
				return *a < *b;
			}
		};
	}

	using KeyboardDeviceInfoList = std::set<std::shared_ptr<KeyboardDeviceInfo>, details::KeyboardDevicePtrLess>;
}