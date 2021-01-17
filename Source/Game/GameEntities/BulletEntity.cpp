#include "BulletEntity.h"
#include "GameEngine/GameEngineMain.h"
#include <cmath>

#define M_PI 3.1415926

using namespace Game;


BulletEntity::BulletEntity(sf::Vector2f pos, sf::Vector2f dir) {
    entityType = GameEngine::EntityType::Bullet;
    SetPos(pos);
    SetSize(sf::Vector2f(35.f, 35.f));

    SetRotation(90-(atan2(dir.x, dir.y)/M_PI * 180));

    m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
	m_renderComponent->SetTexture(GameEngine::eTexture::RaygunLaser);
    m_renderComponent->SetFillColor(sf::Color::Transparent);
	m_renderComponent->SetZLevel(zIndex::Monitor);

    m_collidablePhysicsComponent = AddComponent<GameEngine::CollidablePhysicsComponent>();
    m_collidablePhysicsComponent->vel = dir;
    this->direction = dir;
    m_collidablePhysicsComponent->gravityEnabled = false;

    m_animComponent = AddComponent<GameEngine::AnimationComponent>();
}
 
BulletEntity::~BulletEntity() {

}

void BulletEntity::Update() { 
    Entity::Update();
    frameLifeTime -= 1;
    if (frameLifeTime <= 0) {
        GameEngine::GameEngineMain::GetInstance()->RemoveEntity(this);
    }
    if (m_collidablePhysicsComponent->collidingTop != nullptr
    || m_collidablePhysicsComponent->collidingBottom != nullptr
    || m_collidablePhysicsComponent->collidingLeft != nullptr
    || m_collidablePhysicsComponent->collidingRight != nullptr
    || this->hitPlayer) {
        GameEngine::GameEngineMain::GetInstance()->RemoveEntity(this);
    }
}



void BulletEntity::OnAddToWorld() {
	Entity::OnAddToWorld();

	// if (m_animComponent) {
	//  	m_animComponent->PlayAnim(GameEngine::EAnimationId::Bullet);
	// }
}


void BulletEntity::OnRemoveFromWorld() {
	Entity::OnRemoveFromWorld();
    //TODO: Might need to free the component memories
}
