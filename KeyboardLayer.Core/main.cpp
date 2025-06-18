#include "KeyboardLayerEngine.h"
#include "InterceptionKeyCode.h"
#include "SimpleKeyBlocker.h"
#include "HidKeyCodes.h"

#pragma comment (lib, "interception.lib")

int main() {
	std::set<int> blockedKeys = {
		static_cast<int>(InterceptionKeyCode::W),
		static_cast<int>(InterceptionKeyCode::Space),
	};
	std::wstring targetKeyboardIdPart = L"ACPI\\VEN_ATK&DEV_3001";

	auto filter = std::make_unique<SimpleKeyBlocker>(targetKeyboardIdPart, blockedKeys);
	KeyboardLayerEngine engine(std::move(filter));
	engine.Run();

	return 0;
}