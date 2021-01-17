#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class LadderEntity : public GameEngine::Entity
    {
    public:
        LadderEntity()
        {
            entityType = GameEngine::EntityType::Ladder;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::Ladder);
            m_renderComponent->SetZLevel(Game::zIndex::Blocks);
            this->SetSize(sf::Vector2f(70, 70));

            // AddComponent<GameEngine::CollidableComponent>();
        };
        ~LadderEntity()
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
