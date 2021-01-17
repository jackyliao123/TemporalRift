#include "ObstacleEntity.h"

#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <SFML/Graphics.hpp>

using namespace Game;

ObstacleEntity::ObstacleEntity()	
{
	entityType = GameEngine::EntityType::Obstacle;

	m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
	m_renderComponent->SetFillColor(sf::Color::Black);
	m_renderComponent->SetZLevel(0);

	AddComponent<GameEngine::CollidableComponent>();
}


ObstacleEntity::~ObstacleEntity()
{

}


void ObstacleEntity::OnAddToWorld()
{
	Entity::OnAddToWorld();
}


void ObstacleEntity::OnRemoveFromWorld()
{
	Entity::OnRemoveFromWorld();
}
#include <cmath>

void ObstacleEntity::Update()
{
	ctr += 1;
	Entity::Update();
	GameEngine::CollidableComponent *collide = GetComponent<GameEngine::CollidableComponent>();
	float dx = sin(ctr / 100.0);
	float dy = cos(ctr / 100.0);
	collide->MoveToNewLocation(GetPos() + sf::Vector2f(dx, dy));
}