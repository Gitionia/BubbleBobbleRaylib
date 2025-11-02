#pragma once

#include "../ecs/SystemBase.h"

class AudioSystem : public SystemBase {
SYSTEM_DEF(AudioSystem)

public:
	void Update() override;

};

