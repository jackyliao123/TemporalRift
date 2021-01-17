#include "PlayerEntity.h"

// #include "Game/GameComponents/PlayerSoundComponent.h"
#include "Game/GameComponents/PlayerCameraComponent.h"

#include "GameEngine/EntitySystem/Components/ZoneTriggerComponent.h"
#include "GameEngine/EntitySystem/Components/ParticleEmitterComponent.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
#include "GameEngine/Util/AnimationManager.h"
#include "Game/GameComponents/PlayerGunRenderComponent.h"
using namespace Game;

PlayerEntity::PlayerEntity() : PlayerEntity(0) {}

PlayerEntity::PlayerEntity (int input_id) { 
	id = input_id;
	this->SetSize(sf::Vector2f(72.f, 97.f));
	entityType = GameEngine::EntityType::Player;

	//Movement
	m_playerMovementComponent = AddComponent<PlayerMovementComponent>();

	//Render 
	m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();

	m_renderComponent->SetTexture(GameEngine::eTexture::Player1);
	
	if(input_id == 0) {
		m_renderComponent->SetZLevel(zIndex::Player3);
	} else if(input_id == 1) {
		m_renderComponent->SetZLevel(zIndex::Player1);
	} else if(input_id == 2) {
		m_renderComponent->SetZLevel(zIndex::Player2);
	}

	m_gunRenderComponent = AddComponent<PlayerGunRenderComponent>();	
	m_gunRenderComponent->SetTexture(GameEngine::eTexture::Raygun);
	m_gunRenderComponent->SetZLevel(zIndex::Monitor);

	hitEnemy = false;

	//Animation
	m_animComponent = AddComponent<GameEngine::AnimationComponent>();
		
	// //Collisions
	//AddComponent<GameEngine::ZoneTriggerComponent>();

	AddComponent<GameEngine::CollidablePhysicsComponent>();
	
	// //Particles
	// GameEngine::ParticleEmitterComponent* emitterComponent = AddComponent<GameEngine::ParticleEmitterComponent>();
	// GameEngine::SParticleDefinition particleDef = GameEngine::SParticleDefinition(GameEngine::eTexture::Particles, 1, sf::Vector2f(32.f, 32.f), GameEngine::EAnimationId::Smoke, 1.f);
	// emitterComponent->SetParticleDefinition(particleDef);

	// //Sound
	// GameEngine::SoundComponent* const soundComponent = AddComponent<GameEngine::SoundComponent>();
	// soundComponent->SetNumSimultaneousSounds(2); // Hard coded 5 simultaneous sounds for the player
												 
	// AddComponent<PlayerSoundComponent>();
	
	if (id == 0) {
		AddComponent<PlayerCameraComponent>();
	}
}

 
PlayerEntity::~PlayerEntity()
{

}


void PlayerEntity::OnAddToWorld()
{
	Entity::OnAddToWorld();
}


void PlayerEntity::OnRemoveFromWorld()
{
	Entity::OnRemoveFromWorld();
}
