#include "ZoneTriggerComponent.h"

#include "GameEngine/Util/CollisionManager.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/GameEngineMain.h"

#include <vector>
#include <iostream>
#include <functional>

using namespace GameEngine;

ZoneTriggerComponent::ZoneTriggerComponent() :
otherColliableComponent(nullptr)
{

}


ZoneTriggerComponent::~ZoneTriggerComponent()
{

}

void ZoneTriggerComponent::OnAddToWorld()
{
	if (m_useDefaultBox)
		SetupDefaultBoundingBox();
}


void ZoneTriggerComponent::OnRemoveFromWorld()
{

}


void ZoneTriggerComponent::Update()
{
	CollidableComponent::Update();	
	AABBRect intersection;
	AABBRect myBox = GetWorldAABB();

	bool triggeredThisTime = false;

	for(Entity *other : GameEngine::GameEngineMain::GetInstance()->m_entities) {
		if(other->entityType == EntityType::Player) {
			AABBRect colideBox = other->GetComponent<CollidableComponent>()->GetWorldAABB();
			if (myBox.intersects(colideBox, intersection))
			{
				triggeredThisTime = true;
				break;
			}
		}
	}
	if(triggeredThisTime) {
		if (!triggered)
		{
			callback();
			triggered = true;
		}
	} else {
		triggered = false;
	}
}