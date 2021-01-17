#pragma once
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/Util/SoundManager.h"

namespace Game
{	

	class PlayerMovementComponent : public GameEngine::Component
	{
	public:
		PlayerMovementComponent();
		~PlayerMovementComponent();

		virtual void Update() override;
		virtual void OnAddToWorld() override;
		void SpawnProjectile(sf::Vector2f);
		int frameCnt = 0;
		sf::Vector2f velocity;
		
		void setRotation(float);

	private:
		// float m_flyTimerDt;
		// float m_flyTimerMaxTime;

		float rotation = 0.0f;

		int historyIndex = 0;
		int bulletIndex = 0;

		bool wasFirePressed = false;

		

		GameEngine::AnimationComponent* m_animComponent;
		// PlayerSoundComponent* m_playerSoundComponent;
	};
}


