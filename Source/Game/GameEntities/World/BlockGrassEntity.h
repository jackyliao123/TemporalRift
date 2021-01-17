#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class BlockGrassEntity : public GameEngine::Entity
    {
    public:
        BlockGrassEntity()
        {
            entityType = GameEngine::EntityType::Obstacle;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::BlockGrass); // Need to set Tile Index
            m_renderComponent->SetZLevel(Game::zIndex::Blocks);
            this->SetSize(sf::Vector2f(70, 70));

            AddComponent<GameEngine::CollidableComponent>();
        };
        ~BlockGrassEntity()
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
