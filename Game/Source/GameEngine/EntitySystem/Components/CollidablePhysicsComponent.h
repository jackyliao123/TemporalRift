#pragma once
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

namespace GameEngine
{
	//This one allows for movement and obstacle detection, 
	class CollidablePhysicsComponent : public CollidableComponent
	{
	public:
		CollidablePhysicsComponent();
		virtual ~CollidablePhysicsComponent();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;
		virtual void PerformCollision();

		virtual void Update() override;

		sf::Vector2f vel = sf::Vector2f(0, 0);
		sf::Vector2f accel = sf::Vector2f(0, 0);

		bool gravityEnabled = true;

		CollidableComponent *collidingTop = nullptr;
		CollidableComponent *collidingBottom = nullptr;
		CollidableComponent *collidingLeft = nullptr;
		CollidableComponent *collidingRight = nullptr;
	};
}

