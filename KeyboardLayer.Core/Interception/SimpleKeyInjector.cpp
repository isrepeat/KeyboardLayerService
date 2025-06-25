//#include "SimpleKeyInjector.h"
//
//SimpleKeyInjector::SimpleKeyInjector(InterceptionContext context)
//	: context(context) {
//}
//
//void SimpleKeyInjector::InjectSequence(
//	const DeviceInfo& device,
//	const std::vector<InterceptionKeyStroke>& strokes
//) {
//	for (const auto& stroke : strokes) {
//		InterceptionStroke buffer{};
//		std::memcpy(buffer, &stroke, sizeof(InterceptionKeyStroke));
//
//		interception_send(this->context, device.device, &buffer, 1);
//	}
//}