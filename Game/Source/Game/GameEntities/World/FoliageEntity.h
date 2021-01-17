#pragma once

#include "Game/GameBoard.h"

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

#include <SFML/Graphics.hpp>

namespace Game
{
    class FoliageEntity : public GameEngine::Entity
    {
    public:
        FoliageEntity()
        {
            entityType = GameEngine::EntityType::Foliage;

            m_renderComponent = AddComponent<GameEngine::SpriteRenderComponent>();
            m_renderComponent->SetTexture(GameEngine::eTexture::FoliageGrass); // SHOULD BE OVERRIDEN
            m_renderComponent->SetZLevel(Game::zIndex::Monitor); // MUST BE OVERRIDEN

            //AddComponent<GameEngine::CollidableComponent>();
        };
        ~FoliageEntity()
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
