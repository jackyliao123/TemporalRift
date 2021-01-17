#include "PlayerCameraComponent.h"

#include "GameEngine/GameEngineMain.h"
#include "GameEngine/Util/CameraManager.h"

using namespace Game;

PlayerCameraComponent::PlayerCameraComponent()
{

}


PlayerCameraComponent::~PlayerCameraComponent()
{

}

void PlayerCameraComponent::OnAddToWorld()
{

}


void PlayerCameraComponent::Update()
{
	Component::Update();

	sf::View view = GameEngine::CameraManager::GetInstance()->GetCameraView();
	sf::Vector2f viewCenter = view.getCenter();
	sf::Vector2f entityCenter = GetEntity()->GetPos();
	viewCenter += (entityCenter - viewCenter) / 10.0f;
	view.setCenter(entityCenter);

	GameEngine::CameraManager::GetInstance()->GetCameraView().setCenter(viewCenter);

}
