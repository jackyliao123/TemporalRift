#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"

namespace Game {
	class BulletEntity : public GameEngine::Entity {
	public:
		BulletEntity(sf::Vector2f, sf::Vector2f);
		~BulletEntity();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

        virtual void Update() override;
		sf::Vector2f direction;
		int id = 0;
		bool hitPlayer = false;

	protected:
		GameEngine::SpriteRenderComponent* m_renderComponent;		
		GameEngine::AnimationComponent* m_animComponent;
        GameEngine::CollidablePhysicsComponent* m_collidablePhysicsComponent;
        int frameLifeTime = 60 * 5;
	};
}

