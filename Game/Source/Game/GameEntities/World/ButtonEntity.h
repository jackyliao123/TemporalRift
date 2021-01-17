#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/ZoneTriggerComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class ButtonEntity : public GameEngine::Entity
    {
    public:
        ButtonEntity()
        {
            entityType = GameEngine::EntityType::Objective;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::Button);
            m_renderComponent->SetZLevel(Game::zIndex::Blocks);
            this->SetSize(sf::Vector2f(70, 70));

            AddComponent<GameEngine::ZoneTriggerComponent>();
        };
        ~ButtonEntity()
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
