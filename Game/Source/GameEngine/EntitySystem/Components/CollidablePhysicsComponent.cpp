#include "CollidablePhysicsComponent.h"

#include "GameEngine/Util/CollisionManager.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/Util/AABBUtil.h"
#include <iostream>

#include <vector>

using namespace GameEngine;

CollidablePhysicsComponent::CollidablePhysicsComponent()
{

}


CollidablePhysicsComponent::~CollidablePhysicsComponent()
{

}


void CollidablePhysicsComponent::OnAddToWorld()
{
	CollidableComponent::OnAddToWorld();
}


void CollidablePhysicsComponent::OnRemoveFromWorld()
{
	CollidableComponent::OnRemoveFromWorld();
}

#define EPSILON 0
#define SPACING 1e-3

void CollidablePhysicsComponent::PerformCollision() {
	// Get all colliable components
	std::vector<CollidableComponent*>& collidables = CollisionManager::GetInstance()->GetCollidables();

	AABBRect thisBox = GetWorldAABB();

	std::vector<AABBCollisionFace> collisionFaces;

	for (int a = 0; a < collidables.size(); ++a)
	{
		CollidableComponent* colComponent = collidables[a];
		if (colComponent == this)
			continue;

		// Other box is also movable, skip for now TODO
		CollidablePhysicsComponent *physComponent = dynamic_cast<CollidablePhysicsComponent*>(colComponent);
		if(physComponent) {
			continue;
		}

		AABBRect otherBox = colComponent->GetWorldAABB();

		sf::Vector2f otherVelocity(0, 0);

		if(colComponent->hasLocationUpdate) {
			otherVelocity = colComponent->updatedLoc - colComponent->GetEntity()->GetPos();
		}

		AABBRect thisBoxExpanded = AABBUtil::expandByAmount(AABBUtil::expandByVector(AABBUtil::expandByVector(thisBox, vel), -otherVelocity), 1e-3);

		// Box sweep no collision, skip logic
		if(!thisBoxExpanded.intersects(otherBox)) {
			continue;
		}

		bool hasFace = false;
		AABBCollisionFace face;

		sf::Vector2f relVel = vel - otherVelocity;

		if(relVel.x < -EPSILON) {
			hasFace = true;
			face = {NEGX, thisBox.top, thisBox.top + thisBox.height, thisBox.left, otherBox.top, otherBox.top + otherBox.height, otherBox.left + otherBox.width, colComponent, otherVelocity};
		} else if(relVel.x > EPSILON) {
			hasFace = true;
			face = {POSX, thisBox.top, thisBox.top + thisBox.height, thisBox.left + thisBox.width, otherBox.top, otherBox.top + otherBox.height, otherBox.left, colComponent, otherVelocity};
		}

		if(hasFace) {
			collisionFaces.push_back(face);
		}

		hasFace = false;

		if(relVel.y < -EPSILON) {
			hasFace = true;
			face = {NEGY, thisBox.left, thisBox.left + thisBox.width, thisBox.top, otherBox.left, otherBox.left + otherBox.width, otherBox.top + otherBox.height, colComponent, otherVelocity};
		} else if(relVel.y > EPSILON) {
			hasFace = true;
			face = {POSY, thisBox.left, thisBox.left + thisBox.width, thisBox.top + thisBox.height, otherBox.left, otherBox.left + otherBox.width, otherBox.top, colComponent, otherVelocity};
		}

		if(hasFace) {
			collisionFaces.push_back(face);
		}
	}

	std::sort(collisionFaces.begin(), collisionFaces.end(), [](AABBCollisionFace &face1, AABBCollisionFace &face2) {
		return face1.getIntersectionDistance() < face2.getIntersectionDistance();
	});

	sf::Vector2f pos = GetEntity()->GetPos();

	collidingTop = nullptr;
	collidingBottom = nullptr;
	collidingLeft = nullptr;
	collidingRight = nullptr;

	for(AABBCollisionFace &face : collisionFaces) {
		AABBCollisionResult result = AABBUtil::ComputeSideSweepResult(face, vel - face.otherVelocity);
		if(result.valid) {
			if(face.faceType == POSX) {
				pos.x = face.y2 - thisBox.width / 2.0f - SPACING;
				vel.x = face.otherVelocity.x;
				collidingLeft = face.component;
			} else if(face.faceType == NEGX) {
				pos.x = face.y2 + thisBox.width / 2.0f + SPACING;
				vel.x = face.otherVelocity.x;
				collidingRight = face.component;
			} else if(face.faceType == POSY) {
				pos.y = face.y2 - thisBox.height / 2.0f - SPACING;
				vel.y = face.otherVelocity.y;
				collidingTop = face.component;
			} else if(face.faceType == NEGY) {
				pos.y = face.y2 + thisBox.height / 2.0f + SPACING;
				vel.y = face.otherVelocity.y;
				collidingBottom = face.component;
			}
		}
	}

	GetEntity()->SetPos(pos);
}

void CollidablePhysicsComponent::Update()
{
	// std::cout << "Physics update" << std::endl;

	// Apply forces
	vel += accel;
	accel.x = accel.y = 0;

	if(gravityEnabled) {
		// Gravity
		vel.y += 0.2;
	}

	// Ground friction
	if(collidingTop) {
		float targetVelocityX = 0;
		if(collidingTop->hasLocationUpdate) {
			targetVelocityX = (collidingTop->updatedLoc - collidingTop->GetEntity()->GetPos()).x;
		}
		float relVel = vel.x - targetVelocityX;
		relVel *= 0.9;
		vel.x = targetVelocityX + relVel;
	}

	// std::cout << "BEFORE: " << vel.x << ", " << vel.y << std::endl;

	// Collision
	PerformCollision();

	// std::cout << "AFTER: " << vel.x << ", " << vel.y << std::endl;

	// Move entity
	GetEntity()->SetPos(GetEntity()->GetPos() + vel);
}