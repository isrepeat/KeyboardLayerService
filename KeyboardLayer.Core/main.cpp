#include "KeyboardLayerEngine.h"
#include "InterceptionKeyCodeMapper.h"
#include "SimpleKeyRemapper.h"
#include "SimpleKeyBlocker.h"
#include "LogicalKeyGroup.h"

#pragma comment (lib, "interception.lib")

int main() {
	auto keyCodeMapper = std::make_shared<InterceptionKeyCodeMapper>();

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
	engine.Run();

	return 0;
}