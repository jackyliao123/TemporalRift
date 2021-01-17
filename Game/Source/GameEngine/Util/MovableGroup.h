#pragma once

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include <vector>
#include <iostream>

namespace GameEngine {
    class MovableGroup {
    public:
        void AddEntity(Entity *entity) {
            entities.push_back(entity);
            entityStartingLocations.push_back(entity->GetPos());
        }

        void SetOffset(sf::Vector2f offset) {
            for(unsigned i = 0; i < entities.size(); ++i) {
                sf::Vector2f pos = entityStartingLocations[i] + offset;
                Entity *entity = entities[i];
                CollidableComponent *comp = entity->GetComponent<CollidableComponent>();
                if(comp) {
                    comp->MoveToNewLocation(pos);
                } else {
                    std::cout << "set pos" << std::endl;
                    entity->SetPos(pos);
                }
            }
            this->offset = offset;
        }

        void AddOffset(sf::Vector2f add) {
            SetOffset(offset + add);
        }

        sf::Vector2f offset;
        std::vector<Entity*> entities;
        std::vector<sf::Vector2f> entityStartingLocations;
    };
};