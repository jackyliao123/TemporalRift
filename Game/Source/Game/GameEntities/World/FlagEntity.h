#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/ZoneTriggerComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class FlagEntity : public GameEngine::Entity
    {
    public:
        FlagEntity()
        {
            entityType = GameEngine::EntityType::Objective;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::Flag);
            m_renderComponent->SetZLevel(Game::zIndex::Objectives);
			this->SetSize(sf::Vector2f(70.f, 70.f));

            AddComponent<GameEngine::ZoneTriggerComponent>();

			AddComponent<GameEngine::AnimationComponent>();
        };
        ~FlagEntity()
        {
        }

        virtual void OnAddToWorld() override
        {
            Entity::OnAddToWorld();
        };
        virtual void OnRemoveFromWorld() override
        {
            Entity::OnRemoveFromWorld();
        };

    protected:
        GameEngine::SpriteRenderComponent *m_renderComponent;
    };

} // namespace Game
