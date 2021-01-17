#include "PlayerMovementComponent.h"

#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/SoundComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "Game/GameEntities/BulletEntity.h"
#include "GameEngine/Util/CollisionManager.h"
#include "Game/GameEntities/World/LadderEntity.h"

#include "GameEngine/Util/TextureManager.h"
#include "GameEngine/Util/AnimationManager.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/Util/CollisionManager.h"

#include "Game/GameEntities/PlayerEntity.h"

#include "math.h"

// #include "Game/GameComponents/PlayerSoundComponent.h"

#include <iostream>
#include <utility>

#include <queue>


#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

using namespace Game;

PlayerMovementComponent::PlayerMovementComponent()
	// : m_flyTimerDt(0.f)
	// , m_flyTimerMaxTime(2.f)
	: m_animComponent(nullptr)
	// , m_playerSoundComponent(nullptr)
{

}


PlayerMovementComponent::~PlayerMovementComponent()
{

}

void PlayerMovementComponent::OnAddToWorld()
{
	m_animComponent = GetEntity()->GetComponent<GameEngine::AnimationComponent>();
	// m_playerSoundComponent = GetEntity()->GetComponent<PlayerSoundComponent>();
}

void PlayerMovementComponent::setRotation(float value) {
	rotation = value;
}


void PlayerMovementComponent::Update()
{
	PlayerEntity* playerEntity = static_cast<PlayerEntity*>(GetEntity());
	Game::GameBoard* g = GameEngine::GameEngineMain::GetInstance()->m_gameBoard;
	if (g->m_isGameOver || g->gameOverScreenIsUp){
		return;
	}
	frameCnt += 1;
	Component::Update();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		GetEntity()->SetPos(GetEntity()->GetPos() + sf::Vector2f(5, 0));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		GetEntity()->SetPos(GetEntity()->GetPos() + sf::Vector2f(-5, 0));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
		GetEntity()->SetPos(GetEntity()->GetPos() + sf::Vector2f(0, -5));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		GetEntity()->SetPos(GetEntity()->GetPos() + sf::Vector2f(0, 5));
	}

	static float playerAccel = 0.6f;
	static float jumpAccel = 7.0f;
	static float strafeAccel = .05f;
	sf::Vector2f wantedAccel = sf::Vector2f(0.f, 0.f);

	GameEngine::CollidablePhysicsComponent *phys = GetEntity()->GetComponent<GameEngine::CollidablePhysicsComponent>();

	bool inAir = !phys->collidingTop;
	bool collidingBlock = phys->collidingLeft || phys->collidingRight;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		wantedAccel.y -= 0.5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		wantedAccel.y += 0.5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		wantedAccel.x -= 0.5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		wantedAccel.x += 0.5;
	}

	// if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	sf::Keyboard::Key pressedKey = sf::Keyboard::O;

	int id = playerEntity->id;

	bool A = false, D = false, W = false, shotBullet = false;
	sf::Vector2f bullet(0.0f, 0.0f);

	if (id > 0) {
		std::vector<std::pair<int, sf::Keyboard::Key>> &movementHistory = id == 1 ? g->firstMovementHistory : g->secondMovementHistory;
		std::vector<std::pair<int, sf::Vector2f>> &bulletHistory = id == 1 ? g->firstBulletHistory : g->secondBulletHistory;

		if (historyIndex < movementHistory.size()) {
			std::pair<int, sf::Keyboard::Key> key = movementHistory[historyIndex];
			
			while(historyIndex < movementHistory.size() && movementHistory[historyIndex].first < frameCnt) historyIndex++;
 
			if(historyIndex < movementHistory.size()){
				key  = movementHistory[historyIndex];
				while (frameCnt == key.first) {
					historyIndex++;
					if (!A) A = key.second == sf::Keyboard::A;
					if (!D) D = key.second == sf::Keyboard::D;
					if (!W) W = key.second == sf::Keyboard::W;
					if (historyIndex < movementHistory.size())
						key = movementHistory[historyIndex];
					else
						break;
				}
			}

		}

		if (bulletIndex  < bulletHistory.size()) {
			std::pair<int, sf::Vector2f> bulletDir = bulletHistory[bulletIndex];
			while (bulletIndex  < bulletHistory.size() && frameCnt > bulletHistory[bulletIndex].first) {
				bulletIndex++;
			}

			if (bulletIndex < bulletHistory.size() && frameCnt == bulletHistory[bulletIndex].first) {
				bullet = bulletHistory[bulletIndex].second;
				bulletIndex++;
				shotBullet = true;
			}
		}
		// std::cout << frameCnt << " " << id << " " << A << " " << D << " " << W << " " << shotBullet << std::endl;

	}

	std::queue<sf::Keyboard::Key> pressedKeys;

	if ((id == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::A))|| (id && A))
	{
		pressedKeys.push(sf::Keyboard::A);
		if (!inAir) {
			wantedAccel.x -= playerAccel;
		} else {
			wantedAccel.x -= strafeAccel;
		}
		GameEngine::SpriteRenderComponent* src =  GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();
		if(src) src->SetScaleRight(); 
	}
	if ((id == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (id && D))
	{
		pressedKeys.push(sf::Keyboard::D);
		if (!inAir){
			wantedAccel.x += playerAccel;
		} else {
			wantedAccel.x += strafeAccel;
		}
		GameEngine::SpriteRenderComponent* src =  GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();
		if(src) src->SetScaleLeft();
	}
	if (((id == 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::W))) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) || (id && W))
	{
		pressedKeys.push(sf::Keyboard::W);
		if (!inAir) {
			wantedAccel.y -= jumpAccel;
			wantedAccel.x *= 2;
		}

		bool foundLadder = false;

		for(GameEngine::Entity *entity : GameEngine::GameEngineMain::GetInstance()->m_entities) {
			LadderEntity *ladder = dynamic_cast<LadderEntity*>(entity);
			if(ladder) {
				sf::FloatRect aabb;
				sf::Vector2f size = ladder->GetSize();
				sf::Vector2f pos = ladder->GetPos();
				aabb.width = size.x;
				aabb.height = size.y;
				aabb.left = pos.x - size.x / 2.0f;
				aabb.top = pos.y - size.y / 2.0f;
				if(aabb.intersects(phys->GetWorldAABB())) {
					foundLadder = true;
					break;
				}
			}
		}

		if(foundLadder) {
			if(phys->vel.y > -5) {
				wantedAccel.y -= 0.7;
				phys->vel.x *= 0.92;
			}
		}
	}
	
	// if(collidingBlock) {
	// 	if(phys->vel.y > -2) {
	// 		wantedAccel.y -= 0.5;
	// 	}
	// }

	// Bullets

	GameEngine::AABBRect playerBox = phys -> GetWorldAABB();
	std::vector<GameEngine::CollidableComponent*>& collidables = GameEngine::CollisionManager::GetInstance()->GetCollidables();
	for(GameEngine::CollidableComponent* c: collidables){
		if(c->GetEntity() -> entityType != GameEngine::EntityType::Bullet) continue;
		if(playerBox.intersects(c->GetWorldAABB())){
			BulletEntity* be = dynamic_cast<BulletEntity*>(c->GetEntity());
			PlayerEntity* pe = dynamic_cast<PlayerEntity*>(GetEntity());
			if(be && (be-> id != pe -> id)){
				//sf::Vector2f bulletVelocity = be->direction;
				phys->vel += (be->direction/16.f);
				be->hitPlayer = true;

			}
		}
	}
	phys -> accel = wantedAccel;
	//GetEntity()->GetComponent<GameEngine::CollidablePhysicsComponent>()->accel = wantedAccel;


	// Shoot bullets
	if (id == 0) {
		while (!pressedKeys.empty()) {
			sf::Keyboard::Key pressedKey = pressedKeys.front();
			pressedKeys.pop();
			if (g->numLives == 3) {
				g->firstMovementHistory.push_back(std::make_pair(frameCnt, pressedKey));
			} else if (g->numLives == 2) {
				// std::cout << "pushing to movehistory 2" << std::endl;
				g->secondMovementHistory.push_back(std::make_pair(frameCnt, pressedKey));
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			wasFirePressed = true;
		} else {
			if (wasFirePressed) {

				sf::RenderWindow* rw = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();

				sf::View view = GameEngine::CameraManager::GetInstance()->GetCameraView();
				sf::Vector2i playerLoc = rw->mapCoordsToPixel(view.getCenter());
				sf::Vector2i mouseLoc = sf::Mouse::getPosition(*rw);
				float length = sqrt((mouseLoc.x-playerLoc.x)*(mouseLoc.x-playerLoc.x)+(mouseLoc.y-playerLoc.y)*(mouseLoc.y-playerLoc.y));
				sf::Vector2f v(
					(mouseLoc.x-playerLoc.x)/length * 32,
					(mouseLoc.y-playerLoc.y)/length * 32
				);

				if (id == 0) {
					if (g->numLives == 3) {
						g->firstBulletHistory.push_back(std::make_pair(frameCnt, v));
					} else if (g->numLives == 2) {
						g->secondBulletHistory.push_back(std::make_pair(frameCnt, v));
					}
				}

				SpawnProjectile(v);
			}
			wasFirePressed = false;
		}
	} else if (shotBullet) {
		SpawnProjectile(bullet);
	}



	if (m_animComponent)
	{
		double epsilon = 0.1;
		if (id == 0) {
			if (g->numLives == 3) {
					// Update player animation
						if(((phys -> vel.x < 0 ? -phys -> vel.x : phys -> vel.x) < epsilon) &&
						((phys -> vel.y < 0 ? -phys -> vel.y : phys -> vel.y) < epsilon)){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Idle){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Idle);
							}

						} else if(inAir){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Jumping){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Jumping);
							}
							
						} else{
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Walking){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Walking);
							}
						}
			} else if (g->numLives == 2) {
						if(((phys -> vel.x < 0 ? -phys -> vel.x : phys -> vel.x) < epsilon) &&
						((phys -> vel.y < 0 ? -phys -> vel.y : phys -> vel.y) < epsilon)){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Idle){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Idle);
							}

						} else if(inAir){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Jumping){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Jumping);
							}
							
						} else{
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Walking){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Walking);
							}
						}
			} else if (g->numLives == 1) {
						if(((phys -> vel.x < 0 ? -phys -> vel.x : phys -> vel.x) < epsilon) &&
						((phys -> vel.y < 0 ? -phys -> vel.y : phys -> vel.y) < epsilon)){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player3Idle){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Idle);
							}

						} else if(inAir){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player3Jumping){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player3Jumping);
							}
							
						} else{
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player3Walking){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player3Walking);
							}
						}
			}
		} else if (id == 1) {
						if(((phys -> vel.x < 0 ? -phys -> vel.x : phys -> vel.x) < epsilon) &&
						((phys -> vel.y < 0 ? -phys -> vel.y : phys -> vel.y) < epsilon)){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Idle){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Idle);
							}

						} else if(inAir){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Jumping){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Jumping);
							}
							
						} else{
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player1Walking){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player1Walking);
							}
						}
		} else if (id == 2) {
						if(((phys -> vel.x < 0 ? -phys -> vel.x : phys -> vel.x) < epsilon) &&
						((phys -> vel.y < 0 ? -phys -> vel.y : phys -> vel.y) < epsilon)){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Idle){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Idle);
							}

						} else if(inAir){
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Jumping){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Jumping);
							}
							
						} else{
							if(playerEntity -> m_animComponent -> GetCurrentAnimation() != GameEngine::EAnimationId::Player2Walking){
								playerEntity -> m_animComponent -> PlayAnim(GameEngine::EAnimationId::Player2Walking);
							}
						}
		}
	}
	// Position should be updated by the CollidablePhysicsComponent
}

void PlayerMovementComponent::SpawnProjectile(sf::Vector2f dir) {

	sf::Vector2f adjustment = rotation > 90 || rotation < -90 ? sf::Vector2f(-30, 15) :  sf::Vector2f(30, 15);

	BulletEntity* projectile = new BulletEntity(
		GetEntity()->GetPos() + adjustment,
		dir
	);
	projectile -> id = dynamic_cast<PlayerEntity*>(GetEntity()) -> id;
	GameEngine::GameEngineMain::GetInstance()->AddEntity(projectile);
}
