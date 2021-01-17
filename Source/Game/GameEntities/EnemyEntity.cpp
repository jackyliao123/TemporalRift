#include "EnemyEntity.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "GameEngine/EntitySystem/Components/ParticleEmitterComponent.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
#include "GameEngine/Util/AnimationManager.h"

using namespace Game;

EnemyEntity::EnemyEntity(sf::Vector2f startPos,sf::Vector2f endPos , int id)
{
	entityType = GameEngine::EntityType::Enemy;
	m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
	this->id = id;
	std::cout << id << std::endl;
	if(id == 0) {
		m_renderComponent->SetTexture(GameEngine::eTexture::EnemyFly);	
	}
	else {
		m_renderComponent->SetTexture(GameEngine::eTexture::EnemySlime);
	}

	GameEngine::CollidablePhysicsComponent* cpc = AddComponent<GameEngine::CollidablePhysicsComponent>();
	cpc->gravityEnabled = (id == 1);

	hitBullet = false;

	m_enemyMovementComponent = AddComponent<EnemyMovementComponent>();
	m_enemyMovementComponent->SetStartAndEndPos(startPos, endPos);

	this->startPos = startPos; this->endPos = endPos;
	m_renderComponent->SetZLevel(zIndex::Enemies);
	m_animComponent = AddComponent<GameEngine::AnimationComponent>();
}

 
EnemyEntity::~EnemyEntity()
{

}


void EnemyEntity::OnAddToWorld()
{
	Entity::OnAddToWorld();

	// if (m_animComponent)
	// {
	//  	// m_animComponent->PlayAnim(id == 0 ? GameEngine::EAnimationId::EnemyFlyMoving : GameEngine::EAnimationId::EnemySlimeMoving);
	// }
}


void EnemyEntity::OnRemoveFromWorld()
{
	Entity::OnRemoveFromWorld();
}
