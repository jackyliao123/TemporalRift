#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class CloudEntity : public GameEngine::Entity
    {
    public:
        CloudEntity()
        {
            entityType = GameEngine::EntityType::Foliage;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::Cloud1); // SHOULD BE OVERRIDEN
            m_renderComponent->SetZLevel(Game::zIndex::Clouds);
            this->SetSize(sf::Vector2f(128, 71));

            //AddComponent<GameEngine::CollidableComponent>();
        };
        ~CloudEntity()
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
