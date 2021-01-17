#pragma once
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"

namespace Game
{	

	class EnemyMovementComponent : public GameEngine::Component
	{
	public:
		EnemyMovementComponent();
		~EnemyMovementComponent();

		virtual void Update() override;
		virtual void OnAddToWorld() override;
		void SetStartAndEndPos(sf::Vector2f start, sf::Vector2f end) { 
			startPos = start; endPos = end;
		};
	private:
		sf::Vector2f velocity;
		sf::Vector2f startPos;
		sf::Vector2f endPos; 
		bool isGoingRight;
		// GameEngine::AnimationComponent* m_animComponent;
		// PlayerSoundComponent*			m_playerSoundComponent;
	};
}


