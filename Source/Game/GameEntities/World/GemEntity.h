#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/ZoneTriggerComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class GemEntity : public GameEngine::Entity
    {
    public:
        GemEntity()
        {
            entityType = GameEngine::EntityType::Objective;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::Gem);
            m_renderComponent->SetZLevel(Game::zIndex::Objectives);
            this->SetSize(sf::Vector2f(34, 24));

            AddComponent<GameEngine::ZoneTriggerComponent>();
        };
        ~GemEntity()
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
