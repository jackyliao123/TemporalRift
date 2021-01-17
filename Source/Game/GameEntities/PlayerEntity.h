#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "Game/GameComponents/PlayerMovementComponent.h"

class PlayerGunRenderComponent;
namespace Game
{			
	
	class PlayerEntity : public GameEngine::Entity
	{
	public:
		PlayerEntity();
		PlayerEntity(int);
		~PlayerEntity();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

		bool hitEnemy;
		int id = 0; // 0 is actual player, 1 is phantom 1, 2 is phantom 2
		PlayerMovementComponent*	 m_playerMovementComponent;
		GameEngine::SpriteRenderComponent* m_renderComponent;
		GameEngine::AnimationComponent* m_animComponent;
	protected:
		PlayerGunRenderComponent* m_gunRenderComponent;	
	};
}

