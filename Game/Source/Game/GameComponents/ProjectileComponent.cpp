#include "ProjectileComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Component.h"
#include "GameEngine/Util/CollisionManager.h"

using namespace Game;
// DEPRECATED
ProjectileComponent::ProjectileComponent()
{
}

ProjectileComponent::~ProjectileComponent()
{

}

void ProjectileComponent::Update() {
    Component::Update();
    frameLifeTime -= 1;

    sf::Vector2f wantedVel(0, 0);
    if (direction) {
        wantedVel.x = 10;
    } else {
        wantedVel.x = -10;
    }
    GetEntity()->SetPos(GetEntity()->GetPos() + wantedVel);

    if (frameLifeTime <= 0) {
        GameEngine::GameEngineMain::GetInstance()->RemoveEntity(GetEntity());
    }
}