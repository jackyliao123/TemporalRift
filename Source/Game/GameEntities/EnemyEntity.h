#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "Game/GameComponents/EnemyMovementComponent.h"

namespace Game
{			

	class EnemyEntity : public GameEngine::Entity
	{
	public:
		EnemyEntity(sf::Vector2f, sf::Vector2f, int id);
		~EnemyEntity();

		bool hitBullet;

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

		sf::Vector2f startPos;
		sf::Vector2f endPos;

		int id; // 0 = fly, 1 = slime
		GameEngine::AnimationComponent* m_animComponent;
		GameEngine::SpriteRenderComponent* m_renderComponent;		

	protected:
        EnemyMovementComponent*	 m_enemyMovementComponent;

	};
}

