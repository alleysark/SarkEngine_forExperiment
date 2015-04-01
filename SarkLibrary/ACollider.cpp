#include "ACollider.h"

namespace sark {

	ACollider::ACollider(ASceneComponent* reference)
		: mReference(reference)
	{}

	ACollider::~ACollider() {}

}