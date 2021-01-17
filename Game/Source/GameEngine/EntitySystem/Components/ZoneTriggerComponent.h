#pragma once
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <functional>

namespace GameEngine
{
	class ZoneTriggerComponent : public CollidableComponent
	{
	public:
		ZoneTriggerComponent();
		virtual ~ZoneTriggerComponent();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

		virtual void Update() override;

		void SetTriggerFunction(std::function<void()> callback) {
			this->callback = callback;
		}

	private:
		CollidableComponent* otherColliableComponent;
		bool triggered = false;
		std::function<void()> callback;
	};
}

