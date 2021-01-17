#include "EnemyMovementComponent.h"

#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/Util/CollisionManager.h"

#include "GameEngine/Util/TextureManager.h"
#include "GameEngine/Util/AnimationManager.h"

#include "Game/GameEntities/PlayerEntity.h"
#include "Game/GameEntities/EnemyEntity.h"

// #include "Game/GameComponents/PlayerSoundComponent.h"

#include <iostream>
#include "GameEngine/Util/CollisionManager.h"

#include <SFML/Window/Keyboard.hpp>
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

using namespace Game;

EnemyMovementComponent::EnemyMovementComponent()
: isGoingRight(true)
{
	
}


EnemyMovementComponent::~EnemyMovementComponent()
{

}

void EnemyMovementComponent::OnAddToWorld()
{
}


void EnemyMovementComponent::Update()
{
    
	Component::Update();

	sf::Vector2f pos = GetEntity()->GetPos();

	static float playerAccel = 0.2f; //Pixels/s

    Game::GameBoard* g = GameEngine::GameEngineMain::GetInstance()->m_gameBoard;

    PlayerEntity* p = g->m_player;
	if(p == nullptr) return; // We delete the player when it dies
    sf::Vector2f playerPos = p->GetPos();
	std::vector<GameEngine::Entity*> obstacles = g->m_obstacles;
    //std::cout << playerPos.x << " " << playerPos.y << " " << pos.x << " " << pos.y << std::endl;

	sf::Vector2f wantedAccel = sf::Vector2f(0.f, 0.f);

	double MAX_X = 600.f;
	
	// Player's AABB  (MIGHT NOT EXIST??? )
	GameEngine::CollidableComponent* pcc = p->GetComponent<GameEngine::CollidableComponent>();
	GameEngine::CollidableComponent* ecc = GetEntity() -> GetComponent<GameEngine::CollidableComponent>();
	GameEngine::AABBRect playerBox, curBox;
	if(pcc) playerBox = pcc -> GetWorldAABB();
	if(ecc) curBox =  ecc -> GetWorldAABB();
	
	// Check if it's currently on an obstacle
	bool hitObstacle = false;
	//bool inAir = !GetEntity()->GetComponent<GameEngine::CollidablePhysicsComponent>()->isCollidingTop;
	std::vector<GameEngine::CollidableComponent*>& collidables = GameEngine::CollisionManager::GetInstance()->GetCollidables();
			GameEngine::AABBRect intersection;
	if(ecc){
		for(GameEngine::CollidableComponent* c: collidables){

			if(curBox.intersects(c->GetWorldAABB(), intersection)){
							// dont intersect itself
				if(c->GetEntity() -> entityType == GameEngine::EntityType::Bullet){
					// the enemy should die.....
					EnemyEntity* ee = dynamic_cast<EnemyEntity*>( GetEntity());
					if(ee) {
						ee -> hitBullet = true;
					}

				} else if(c->GetEntity() -> entityType == GameEngine::EntityType::Player){
					PlayerEntity* pe = dynamic_cast<PlayerEntity*> (c -> GetEntity());
					if(pe) pe -> hitEnemy = true;
					// c->GetEntity()->hitEnemy = true;
					// player hit enemy
				} else if(c->GetEntity()->entityType 
						== GameEngine::EntityType::Obstacle && intersection.width > 0){
					isGoingRight = curBox.left > c->GetWorldAABB().left;
					wantedAccel.x = (isGoingRight) ? playerAccel : -playerAccel;
					hitObstacle = true;
					break;
				}
			}
		}
	}
	
	
	bool foundPlayer = false;
	// Keep looking ahead to see if there is player
	while(pcc && ecc && !hitObstacle && curBox.left < MAX_X && curBox.left > 0){
		GameEngine::AABBRect intersection;
		// If it hits an obstacle stop line of sight
		bool hitObstacle = false;
		for(GameEngine::CollidableComponent* c: collidables){
			if(c == ecc || c->GetEntity()->entityType 
				!= GameEngine::EntityType::Obstacle) continue;
			if(curBox.intersects(c->GetWorldAABB(), intersection)){
				hitObstacle = true; // The obstacle breaks line of sight
				break;
			}
		}
		if(hitObstacle) break;

		if(curBox.intersects(playerBox, intersection)){
			// Move toward player
			foundPlayer = true;
			wantedAccel.x = (playerPos.x < pos.x) ? -2*playerAccel : 2*playerAccel;
			//wantedAccel.y = (playerPos.y < pos.y) ? -2*playerAccel : 2*playerAccel;
			isGoingRight = wantedAccel.x > 0;

			break;
		}
		else {
			// Increment the box by 1 width
			curBox.left += ( isGoingRight ? 1 : -1 ) * curBox.width;
		}
		
	}

	if(!foundPlayer && !hitObstacle){

		if(pos.x <= startPos.x ){ // Switch directions
			isGoingRight = true;
		} else if(pos.x >= endPos.x) isGoingRight = false;
		wantedAccel.x = isGoingRight ? playerAccel : -playerAccel;

	}

	GameEngine::SpriteRenderComponent* src =  GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();
	if(src){
		if(isGoingRight) src->SetScaleRight();
		else src->SetScaleLeft();
		// You would only need to switch directions if you're not following player??? Probably not true 
	}
	

	// Process wantedAccel
	//std::cout << "4: " << wantedAccel.x << std::endl;
	velocity += wantedAccel;

	velocity = velocity * 0.9f;

	pos += velocity;

	// dont walk off cliff
	if(pos.x <= startPos.x) {
		pos.x = startPos.x;
		velocity.x = 0;
	}

	if(pos.x >= endPos.x) {
		pos.x = endPos.x;
		velocity.x = 0;
	}

	GetEntity()->SetPos(pos);
	GameEngine::AnimationComponent* m_animComponent = GetEntity() -> GetComponent<GameEngine::AnimationComponent>();
	EnemyEntity* ee = dynamic_cast<EnemyEntity*>(GetEntity());
	GameEngine::SpriteRenderComponent* esrc = ee -> GetComponent<GameEngine::SpriteRenderComponent>(); 
	if(m_animComponent){
		//std::cout << "anim component! " << ee->id << std::endl;
		if(ee->id == 0){
			if(m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::EnemyFlyMoving){
				//std::cout << "EnemyFlyMoving" << std::endl;
				m_animComponent -> PlayAnim(GameEngine::EAnimationId::EnemyFlyMoving);
				esrc -> SetTexture(GameEngine::eTexture::EnemyFly);

			}
		} else if(ee->id == 1){
			esrc -> SetTexture(GameEngine::eTexture::EnemySlime);
			//std::cout << ee -> GetSize().x << " " << ee->GetSize().y << std::endl;
			if(m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::EnemySlimeMoving){
			//std::cout << "EnemySlimeMoving" << std::endl;
				m_animComponent -> PlayAnim(GameEngine::EAnimationId::EnemySlimeMoving);
				esrc -> SetTexture(GameEngine::eTexture::EnemySlime);
				
			} 
		}
	}

}
