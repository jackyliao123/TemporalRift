#pragma once
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "Game/GameEntities/PlayerEntity.h"
#include <SFML/System/Vector2.hpp>

#define M_PI 3.1415926

class PlayerGunRenderComponent : public GameEngine::SpriteRenderComponent {
	void Update() override
	{
	}
	void Render(sf::RenderTarget* target) override
	{
		if (!target)
		{
			return;
		}

		sf::RenderWindow* rw = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();

		sf::View view = GameEngine::CameraManager::GetInstance()->GetCameraView();
		sf::Vector2i playerLoc = rw->mapCoordsToPixel(view.getCenter());
		sf::Vector2i mouseLoc = sf::Mouse::getPosition(*rw);

		sf::Vector2i pix = mouseLoc - playerLoc;

		float rotation = atan2(pix.y, pix.x) / M_PI * 180.0f;

		Game::PlayerEntity* playerEntity = static_cast<Game::PlayerEntity*>(GetEntity());
		playerEntity->m_playerMovementComponent->setRotation(rotation);

		if(rotation > 90 || rotation < -90) {
			m_sprite.setScale(sf::Vector2f(1, -1));
			m_sprite.setPosition(GetEntity()->GetPos() + sf::Vector2f(-30, 15));
			m_sprite.setRotation(rotation);
		} else {
			m_sprite.setScale(sf::Vector2f(1, 1));
			m_sprite.setPosition(GetEntity()->GetPos() + sf::Vector2f(30, 15));
			m_sprite.setRotation(rotation);
		}

		target->draw(m_sprite);
	}
};