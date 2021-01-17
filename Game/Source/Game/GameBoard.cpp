#include "GameBoard.h"

#include "Game/GameEntities/World/BlockGrassEntity.h"
#include "Game/GameEntities/World/ButtonEntity.h"
#include "Game/GameEntities/World/CloudEntity.h"
#include "Game/GameEntities/World/FlagEntity.h"
#include "Game/GameEntities/World/FoliageEntity.h"
#include "Game/GameEntities/World/GemEntity.h"
#include "Game/GameEntities/World/LadderEntity.h"
#include "Game/GameComponents/UIRenderComponent.h"

#include "Game/GameEntities/ObstacleEntity.h"
#include "Game/GameEntities/PlayerEntity.h"
#include "Game/GameEntities/EnemyEntity.h"
#include "Game/GameEntities/World/GemEntity.h"
#include "GameEngine/EntitySystem/Components/AnimationComponent.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/Util/CameraManager.h"
#include "GameEngine/Util/MovableGroup.h"
#include "GameEngine/Util/TextureManager.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace Game;

GameBoard::GameBoard()
	: m_player(nullptr), m_isGameOver(false), numLives(3), gameOverScreenIsUp(false), gameOverEntity(nullptr), countDownEntity(nullptr), uiComponent(nullptr), numUpdates(20 * 60.f), phantom_1(nullptr), phantom_2(nullptr), cameraAdjustmentDummy(nullptr)
{
	CreateBackGround();
	//CreateTestMap();
	CreateMap();

	SpawnAllEnemies();
	SpawnPlayerEntity();
}

void GameBoard::SpawnAllEnemies(){
	for (Game::EnemyEntity* e : m_enemies) { 
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(e); 
	}
	m_enemies.clear();

	SpawnEnemy(sf::Vector2f(380.f, 930.f), sf::Vector2f(750.f, 930.f), 1, sf::Vector2f(380.f, 930.f) );
	SpawnEnemy(sf::Vector2f(400.f, 930.f), sf::Vector2f(780.f, 930.f), 1, sf::Vector2f(700.f, 930.f));
	SpawnEnemy(sf::Vector2f(880.f, 620.f), sf::Vector2f(1600.f, 620.f), 0, sf::Vector2f(880.f, 550.f));
	SpawnEnemy(sf::Vector2f(900.f, 620.f), sf::Vector2f(1630.f, 620.f), 0, sf::Vector2f(1500.f, 550.f));
}

void GameBoard::SpawnPlayerEntity()
{
	m_player = new PlayerEntity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);
	m_player->SetPos(sf::Vector2f(410.f, 500.f));
	m_player->SetSize(sf::Vector2f(72.f, 97.f));

	button1Pressed = false;
	button2Pressed = false;
	platformGroup1.SetOffset(sf::Vector2f(0, 0));
	platformGroup2.SetOffset(sf::Vector2f(0, 0));
	button1->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
	button2->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);

	flag1Set = false;
	flag2Set = false;
	flag1->GetComponent<GameEngine::AnimationComponent>()->StopAnim();
	flag1->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(2, 0);
	flag2->GetComponent<GameEngine::AnimationComponent>()->StopAnim();
	flag2->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(2, 0);
}

void GameBoard::SpawnEnemy(sf::Vector2f start, sf::Vector2f end, int id, sf::Vector2f initialPos)
{
	EnemyEntity* newEnemy = new EnemyEntity(start, end, id);
	// newEnemy->id = id;
	newEnemy->SetPos(initialPos);
	if(id == 0) newEnemy->SetSize(sf::Vector2f(75.f , 36.f ));
	else newEnemy->SetSize(sf::Vector2f(59.f , 30.f ));
	//newEnemy->SetZLevel(zIndex::Enemy)
	m_enemies.push_back(newEnemy);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(newEnemy);

}

void GameBoard::CreateBackGround()
{
	// Create background
	for (int x = -10; x < 20; x++)
	{
		for (int y = -5; y < 15; y++)
		{
			GameEngine::Entity *bgEntity = new GameEngine::Entity();
			GameEngine::SpriteRenderComponent *render = bgEntity->AddComponent<GameEngine::SpriteRenderComponent>();
			render->SetTexture(GameEngine::eTexture::Background);
			render->SetZLevel(zIndex::Background);
			bgEntity->SetPos(sf::Vector2f(256.f * x, 256.f * y));
			bgEntity->SetSize(sf::Vector2f(256.f, 256.f));
			GameEngine::GameEngineMain::GetInstance()->AddEntity(bgEntity);
		}
	}

	// Add background foliage clouds
	std::vector<sf::Vector2f> cloudLocations;
	cloudLocations.push_back(sf::Vector2f(220, 300));
	cloudLocations.push_back(sf::Vector2f(880, 260));
	cloudLocations.push_back(sf::Vector2f(904, 474));
	cloudLocations.push_back(sf::Vector2f(530, 750));
	cloudLocations.push_back(sf::Vector2f(197, 890));
	cloudLocations.push_back(sf::Vector2f(777, 1020));
	cloudLocations.push_back(sf::Vector2f(360, 1333));
	cloudLocations.push_back(sf::Vector2f(1300, 860));
	cloudLocations.push_back(sf::Vector2f(1440, 67));
	cloudLocations.push_back(sf::Vector2f(1690, 330));
	cloudLocations.push_back(sf::Vector2f(2300, 280));
	cloudLocations.push_back(sf::Vector2f(1840, 981));
	cloudLocations.push_back(sf::Vector2f(2180, 1405));
	cloudLocations.push_back(sf::Vector2f(2530, 804));
	for (int i = 0; i < cloudLocations.size(); i++)
	{
		CloudEntity *cloud = new CloudEntity();

		GameEngine::eTexture::type texture = GameEngine::eTexture::Cloud1;
		if (i % 3 == 1)
			texture = GameEngine::eTexture::Cloud2;
		if (i % 3 == 2)
			texture = GameEngine::eTexture::Cloud3;
		cloud->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(texture);
		cloud->SetPos(cloudLocations[i]);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(cloud);
	}
}

float RandomFloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}
void GameBoard::CreateTestMap()
{
	static float minObstacleXPos = 20.f;
	static float maxObstacleXPos = 450.f;
	static float minObstacleYPos = 50.f;
	static float maxObstacleYPos = 450.f;
	static float minObstacleHeight = 20.f;
	static float maxObstacleHeight = 40.f;
	static float minObstacleWidth = 50.f;
	static float maxObstacleWidth = 170.f;
	static float maxNumObstacles = 10.f;

	for (int i = 0; i < floor(RandomFloatRange(1.f, maxNumObstacles)); i++)
	{
		sf::Vector2f pos = sf::Vector2f(RandomFloatRange(minObstacleXPos, maxObstacleXPos), RandomFloatRange(minObstacleYPos, maxObstacleYPos));
		sf::Vector2f size = sf::Vector2f(RandomFloatRange(minObstacleWidth, maxObstacleWidth), RandomFloatRange(minObstacleHeight, maxObstacleHeight));

		// TODO DEPRECATED
		// SpawnNewObstacle(pos, size);
	}
}

FoliageEntity* GameBoard::SpawnFoliage(
	GameEngine::eTexture::type texture,
	zIndex z,
	const sf::Vector2f &pos
) {
	FoliageEntity* foliage = new FoliageEntity();
	foliage->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(texture);
	foliage->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(z);
	foliage->SetPos(pos);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(foliage);

	return foliage;
}

void GameBoard::CreateMap()
{
	int tempX = 0;
	int tempY = 0;
	std::vector<sf::Vector2f> grassFullLedgeLeftLocations;
	std::vector<sf::Vector2f> grassFullLedgeRightLocations;
	std::vector<sf::Vector2f> grassFullBlockLocations;
	std::vector<sf::Vector2f> grassPlatformLedgeLeftLocations;
	std::vector<sf::Vector2f> grassPlatformLedgeRightLocations;
	std::vector<sf::Vector2f> grassPlatformBlockLocations;
	std::vector<sf::Vector2f> grassSlantedLedgeLeftLocations;
	std::vector<sf::Vector2f> grassSlantedLedgeRightLocations;
	std::vector<sf::Vector2f> grassCurvedBlockLeftLocations;
	std::vector<sf::Vector2f> grassCurvedBlockRightLocations;
	std::vector<sf::Vector2f> grassCurvedBlockLocations;

	std::vector<sf::Vector2f> ladderTopLocations;
	std::vector<sf::Vector2f> ladderBottomLocations;
	std::vector<sf::Vector2f> ladderLocations;

	// Grass Full Platforms
	{
		tempX = 0;
		tempY = 1090;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 380;
		tempY = 1000;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 380;
		tempY = 640;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 380;
		tempY = 640;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 1280;
		tempY = 670;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		//tempX = 1560;
		//tempY = 490;
		//grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		//grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 1710;
		tempY = 1130;
		grassFullLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));
	}

	std::vector<sf::Vector2f> platform1;
	std::vector<sf::Vector2f> platform2;
	// Moving Platforms
	{
		tempX = 1110;
		tempY = 360;
		grassPlatformLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassPlatformLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 1110;
		tempY = 360;
		platform1.push_back(sf::Vector2f(tempX, tempY));
		platform1.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 1280;
		tempY = 620;
		grassPlatformLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassPlatformBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassPlatformBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassPlatformBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassPlatformLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 1280;
		tempY = 620;
		platform2.push_back(sf::Vector2f(tempX, tempY));
		platform2.push_back(sf::Vector2f(tempX += 70, tempY));
		platform2.push_back(sf::Vector2f(tempX += 70, tempY));
		platform2.push_back(sf::Vector2f(tempX += 70, tempY));
		platform2.push_back(sf::Vector2f(tempX += 70, tempY));
	}

	// Ladders
	{
		tempX = 790;
		tempY = 360;
		ladderTopLocations.push_back(sf::Vector2f(tempX, tempY));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderLocations.push_back(sf::Vector2f(tempX, tempY += 70));
		ladderBottomLocations.push_back(sf::Vector2f(tempX, tempY += 70));
	}

	// Curved Block Blockers
	{
		tempX = 2340;
		tempY = 477;
		grassCurvedBlockLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassCurvedBlockRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));

		tempX = 2490;
		tempY = 1000;
		grassCurvedBlockLocations.push_back(sf::Vector2f(tempX, tempY));
	}

	// End Game Platform
	{
		tempX = 2040 + 70;
		tempY = 600;
		grassSlantedLedgeLeftLocations.push_back(sf::Vector2f(tempX, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassFullBlockLocations.push_back(sf::Vector2f(tempX += 70, tempY));
		grassSlantedLedgeRightLocations.push_back(sf::Vector2f(tempX += 70, tempY));
	}

	for (int i = 0; i < grassFullLedgeLeftLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassFullLedgeLeftLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(3, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassFullLedgeRightLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassFullLedgeRightLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(5, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassFullBlockLocations.size(); i++)
	{
	 	BlockGrassEntity *block = new BlockGrassEntity();
	 	block->SetPos(grassFullBlockLocations[i]);
	 	block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(16, 0);
	 	GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassPlatformLedgeLeftLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassPlatformLedgeLeftLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(8, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
		if(std::find(platform1.begin(), platform1.end(), block->GetPos()) != platform1.end()) {
			platformGroup1.AddEntity(block);
		}

		if(std::find(platform2.begin(), platform2.end(), block->GetPos()) != platform2.end()) {
			platformGroup2.AddEntity(block);
		}
	}
	for (int i = 0; i < grassPlatformLedgeRightLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassPlatformLedgeRightLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(10, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
		if(std::find(platform1.begin(), platform1.end(), block->GetPos()) != platform1.end()) {
			platformGroup1.AddEntity(block);
		}

		if(std::find(platform2.begin(), platform2.end(), block->GetPos()) != platform2.end()) {
			platformGroup2.AddEntity(block);
		}
	}
	for (int i = 0; i < grassPlatformBlockLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassPlatformBlockLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(9, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
		if(std::find(platform1.begin(), platform1.end(), block->GetPos()) != platform1.end()) {
			platformGroup1.AddEntity(block);
		}

		if(std::find(platform2.begin(), platform2.end(), block->GetPos()) != platform2.end()) {
			platformGroup2.AddEntity(block);
		}
	}
	for (int i = 0; i < grassSlantedLedgeLeftLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassSlantedLedgeLeftLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(4, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassSlantedLedgeRightLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassSlantedLedgeRightLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(6, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassCurvedBlockLeftLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassCurvedBlockLeftLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(15, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassCurvedBlockRightLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassCurvedBlockRightLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(17, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < grassCurvedBlockLocations.size(); i++)
	{
		BlockGrassEntity *block = new BlockGrassEntity();
		block->SetPos(grassCurvedBlockLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}

	for (int i = 0; i < ladderTopLocations.size(); i++)
	{
		LadderEntity *block = new LadderEntity();
		block->SetPos(ladderTopLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < ladderLocations.size(); i++)
	{
		LadderEntity *block = new LadderEntity();
		block->SetPos(ladderLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 1);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}
	for (int i = 0; i < ladderBottomLocations.size(); i++)
	{
		LadderEntity *block = new LadderEntity();
		block->SetPos(ladderBottomLocations[i]);
		block->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 2);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(block);
	}

	// Button #1
	button1 = new ButtonEntity();
	button1->SetPos(sf::Vector2f(25, 1020));
	button1->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
	button1->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		button1->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(1, 0);
		button1Pressed = true;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(button1);

	// Button #2
	button2 = new ButtonEntity();
	button2->SetPos(sf::Vector2f(2410, 1060));
	button2->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
	button2->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		button2->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(1, 0);
		button2Pressed = true;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(button2);

	// Flag #1
	flag1 = new FlagEntity();
	flag1->SetPos(sf::Vector2f(1180, 290));
	flag1->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(2, 0);
	flag1->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&] {
		score++;
		flag1->GetComponent<GameEngine::AnimationComponent>()->PlayAnim(GameEngine::EAnimationId::Flag);
		flag1Set = true;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(flag1);
	platformGroup1.AddEntity(flag1);

	// Flag #2
	flag2 = new FlagEntity();
	flag2->SetPos(sf::Vector2f(2250, 530));
	flag2->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(2, 0);
	flag2->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&] {
		score++;
		flag2->GetComponent<GameEngine::AnimationComponent>()->PlayAnim(GameEngine::EAnimationId::Flag);
		flag2Set = true;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(flag2);

	CreateGems();

	// Foliage
	{
		// Flag 1 Platform
		{
			int gOffset = 21;
			platformGroup1.AddEntity(SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(1110, 332 - gOffset)));
			platformGroup1.AddEntity(SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(1193, 332 - gOffset)));
			platformGroup1.AddEntity(SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(1139, 339 - gOffset)));
			platformGroup1.AddEntity(SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(1175, 336 - gOffset)));
		}
		
		// Bottom Left Island
		{
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(105, 1042));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(135, 1040));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(168, 1048));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(179, 1040));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(202, 1044));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(233, 1048));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(242, 1040));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(276, 1040));
		}
	
		// Spawn Island
		{
			SpawnFoliage(
				GameEngine::eTexture::FoliagePebble,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(439, 605));
			SpawnFoliage(
				GameEngine::eTexture::FoliageRockMossAlt,
				Game::zIndex::Foliage,
				sf::Vector2f(480, 573));
			SpawnFoliage(
				GameEngine::eTexture::FoliageRockMoss,
				Game::zIndex::Foliage,
				sf::Vector2f(550, 573));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(600, 589));
			SpawnFoliage(
				GameEngine::eTexture::FoliagePebble,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(634, 605));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(660, 589));
		}

		// Under Spawn Island
		{
			SpawnFoliage(
				GameEngine::eTexture::FoliageBush,
				Game::zIndex::Foliage,
				sf::Vector2f(500, 955));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(563, 949));
			SpawnFoliage(
				GameEngine::eTexture::FoliagePebble,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(580, 964));
			SpawnFoliage(
				GameEngine::eTexture::FoliageRock,
				Game::zIndex::Foliage,
				sf::Vector2f(612, 947));
			SpawnFoliage(
				GameEngine::eTexture::FoliagePebble,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(673, 964));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::FrontFoliage,
				sf::Vector2f(715, 955));
			SpawnFoliage(
				GameEngine::eTexture::FoliageGrass,
				Game::zIndex::Foliage,
				sf::Vector2f(747, 949));
		}

		// Bottom Right Island
		int gOffset = 13;
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(1754, 1092 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageBush,
			Game::zIndex::BackFoliage,
			sf::Vector2f(1770, 1084));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(1809, 1096 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(1849, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageBush,
			Game::zIndex::BackFoliage,
			sf::Vector2f(1893, 1084));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(1939, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(1970, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2004, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(2061, 1093 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliagePebble,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(2084, 1095));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2117, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2174, 1095 - gOffset));
		SpawnFoliage(
			GameEngine::eTexture::FoliagePebble,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(2205, 1095));
		SpawnFoliage(
			GameEngine::eTexture::FoliageRock,
			Game::zIndex::Foliage,
			sf::Vector2f(2228, 1077));
		SpawnFoliage(
			GameEngine::eTexture::FoliagePebble,
			Game::zIndex::FrontFoliage,
			sf::Vector2f(2283, 1095));
		SpawnFoliage(
			GameEngine::eTexture::FoliageRockMossAlt,
			Game::zIndex::Foliage,
			sf::Vector2f(2310, 1063));


		// Bottom Right Blocker
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2475, 950));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2510, 950));

		// Final Flag Island
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2110, 548));
		SpawnFoliage(
			GameEngine::eTexture::FoliageBush,
			Game::zIndex::Foliage,
			sf::Vector2f(2157, 554));
		SpawnFoliage(
			GameEngine::eTexture::FoliageRockMoss,
			Game::zIndex::Foliage,
			sf::Vector2f(2275, 532));

		// Final Flag Blocker
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2340, 428));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2380, 428));
		SpawnFoliage(
			GameEngine::eTexture::FoliageGrass,
			Game::zIndex::Foliage,
			sf::Vector2f(2420, 428));
	}

}

void GameBoard::CreateGems(){
	if(gem1 != nullptr) {
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem1);
	}
	gem1 = new GemEntity();
	gem1->SetPos(sf::Vector2f(410, 942));
	gem1->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		score++;
		std::cout << "gem 1 hit! score: " << score << std::endl;
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem1);
		gem1 = nullptr;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(gem1);

	
	if(gem2 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem2);

	gem2 = new GemEntity();
	gem2->SetPos(sf::Vector2f(675, 580));
	gem2->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		score++;
		std::cout << "gem 2 hit! score: " << score << std::endl;
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem2);
		gem2 = nullptr;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(gem2);


	if(gem3 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem3);
	gem3 = new GemEntity();
	gem3->SetPos(sf::Vector2f(1030, 560));
	gem3->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		score++;
		std::cout << "gem 3 hit! score: " << score << std::endl;
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem3);
		gem3 = nullptr;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(gem3);


	if(gem4 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem4);
	gem4 = new GemEntity();
	gem4->SetPos(sf::Vector2f(2250, 1070));
	gem4->GetComponent<GameEngine::ZoneTriggerComponent>()->SetTriggerFunction([&]{
		score++;
		std::cout << "gem 4 hit! score: " << score << std::endl;
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gem4);
		gem4 = nullptr;
	});
	GameEngine::GameEngineMain::GetInstance()->AddEntity(gem4);

}

GameBoard::~GameBoard()
{
}

void GameBoard::UpdateCountDown()
{
	if (nullptr == countDownEntity)
	{
		// Create the entity and the text
		countDownEntity = new GameEngine::Entity();
		uiComponent = countDownEntity->AddComponent<UIRenderComponent>();
		uiComponent->SetFont("Montserrat-Regular.ttf");
		uiComponent->SetZLevel(zIndex::UI);
		uiComponent->SetCharacterSizePixels(20);
		GameEngine::GameEngineMain::GetInstance()->AddEntity(countDownEntity);
	}
	// Only update the text and position
	int n = numUpdates / 60;
	uiComponent->title = "Time Remaining: " + std::to_string(n) + "\nScore: " + std::to_string(score);
	uiComponent->timerPct = numUpdates / (20 * 60.f);

}
void GameBoard::ResetCountDown()
{
	numUpdates = 1200;
	UpdateCountDown();
}
void GameBoard::CheckWin(){
	win = (flag1Set && flag2Set);
}
void GameBoard::Update()
{
	CheckWin();
	if(button1Pressed) {
		if(platformGroup2.offset.x > -400) {
			platformGroup2.AddOffset(sf::Vector2f(-4, 0));
		}
	}

	if(button2Pressed) {
		if(platformGroup1.offset.x > -230) {
			platformGroup1.AddOffset(sf::Vector2f(-4, 0));
		}
	}

	// platformGroup1.AddOffset(sf::Vector2f(-1, 0));
	// platformGroup2.AddOffset(sf::Vector2f(1, 0));
	if(win){
		if(!hasWinScreen){
			uiComponent->title = std::string() + "You win!";
			uiComponent->subtitle = std::string() + "Score: " + std::to_string(score) + ", press R to restart";
			uiComponent->displayState = 2;
			hasWinScreen = true;
		} else{
			//std::cout << "win screen" << std::endl;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
				std::cout << "exit win screen" << std::endl;
				uiComponent->displayState = 0;
				win = false;
				newRound = true;
				score = 0;
				hasWinScreen = false;
				flag1Set = false; flag2Set = false; // do we set (in spawn player entity)
				firstMovementHistory.clear();
				firstBulletHistory.clear();
				secondMovementHistory.clear();
				secondBulletHistory.clear();
				if(phantom_1 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_1);
				if(phantom_2 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_2);
				phantom_1 = nullptr; phantom_2 = nullptr; 
				if(m_player != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_player);
				m_player = nullptr;
				numLives = 3;
				m_isGameOver = false;
				gameOverScreenIsUp = false;
				SpawnPlayerEntity();
				CreateGems();
				for(Game::EnemyEntity* e:  m_enemies){ 
					GameEngine::GameEngineMain::GetInstance() -> RemoveEntity(e);
				}
				m_enemies.clear();
				SpawnAllEnemies();
				ResetCountDown();

			}
		}
		return; // do nothing else if its on a win screen! 
	}

	if (!m_isGameOver)
	{
		numUpdates--;
		UpdateCountDown();
		UpdateEnemyDying();
		UpdatePlayerDying();
	}
	else if (!gameOverScreenIsUp)
	{
		if (numLives > 1)
		{
			//std::cout << 1 << std::endl;
			m_player->SetPos(sf::Vector2f(410.f, 500.f));

			numLives--;
			// if (gameOverEntity)
			// 	GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gameOverEntity);
			// gameOverEntity = new GameEngine::Entity();
			// GameEngine::TextRenderComponent *gameOverText = gameOverEntity->AddComponent<GameEngine::TextRenderComponent>();
			// gameOverText->SetFont("arial.ttf");
			// gameOverText->SetString(std::string(numUpdates <= 0 ? "Time's up!" : "You died!") + std::string(" You have " + std::to_string(numLives) + " lives left, press R to restart"));
			// gameOverText->SetZLevel(zIndex::UI);
			// gameOverText->SetCharacterSizePixels(20);
			// gameOverText->SetColor(sf::Color::Black);
			// // sf::Vector2f coords = 
			// // 			GameEngine::GameEngineMain::GetInstance()->GetRenderWindow()->mapPixelToCoords(sf::Vector2i(100, 100));
			// // std::cout << coords.x << " " << coords.y << std::endl;
			// gameOverEntity->SetPos(sf::Vector2f(400.f, 400.f));
			// //gameOverEntity -> SetSize(sf::Vector2f(300.f, 70.f));
			// GameEngine::GameEngineMain::GetInstance()->AddEntity(gameOverEntity);

			GameEngine::CollidablePhysicsComponent* playerCPC = m_player->GetComponent<GameEngine::CollidablePhysicsComponent>();
			playerCPC->vel = sf::Vector2f(0,0);
			playerCPC->accel = sf::Vector2f(0,0);
			
			m_player->m_playerMovementComponent->velocity.x = 0;

			// if numlives = 3 then dont remove (phantom should be null)
			// if lives = 2 then remove 1
			if(phantom_1 != nullptr) {
				// std::cout << "remove phantom 1 "<< std::endl;
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_1);
				phantom_1 = nullptr;
			}

			if (m_player != nullptr) {
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_player);
				m_player = nullptr;
			}
			
			uiComponent->title = std::string(numUpdates <= 0 ? "Time's up!" : "You died!");
			uiComponent->subtitle = std::string("You have " + std::to_string(numLives) + " lives left, press R to restart");
			uiComponent->displayState = 1;
			gameOverScreenIsUp = true;

		} else{
			//std::cout << "2" << std::endl;

			// if(gameOverEntity) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gameOverEntity);
			// gameOverEntity = new GameEngine::Entity();
			// GameEngine::TextRenderComponent *gameOverText = gameOverEntity->AddComponent<GameEngine::TextRenderComponent>();
			// gameOverText->SetFont("arial.ttf");
			// gameOverText->SetString(std::string(numUpdates <= 0 ? "Time's up!" : "You died!") + std::string(" You have no lives left, press R to restart"));
			// gameOverText->SetZLevel(zIndex::UI);
			// gameOverText->SetCharacterSizePixels(20);
			// // gameOverEntity->SetPos(
			// // GameEngine::GameEngineMain::GetInstance()->GetRenderWindow()->mapPixelToCoords(sf::Vector2i(100, 100)));
			// gameOverEntity->SetPos(sf::Vector2f(400.f, 400.f));

			// //gameOverEntity -> SetSize(sf::Vector2f(300.f, 50.f));
			// GameEngine::GameEngineMain::GetInstance()->AddEntity(gameOverEntity);

			if(phantom_1 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_1);
			if(phantom_2 != nullptr) GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_2);
			phantom_1 = nullptr; phantom_2 = nullptr; 

			if (m_player != nullptr) {
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_player);
				m_player = nullptr;
			}
			score = 0;
			uiComponent->title = std::string(numUpdates <= 0 ? "Time's up!" : "You died!");
			uiComponent->subtitle = std::string("You have no lives left, press R to restart");
			uiComponent->displayState = 1;

			gameOverScreenIsUp = true;
			numLives = 3;
			// m_player->SetPos(sf::Vector2f(410.f, 500.f));	
			// m_player->m_playerMovementComponent->velocity.x = 0;

			// GameEngine::CollidablePhysicsComponent* playerCPC = m_player->GetComponent<GameEngine::CollidablePhysicsComponent>();
			// playerCPC->vel = sf::Vector2f(0,0);
			// playerCPC->accel = sf::Vector2f(0,0);

			firstMovementHistory.clear();
			firstBulletHistory.clear();
			secondMovementHistory.clear();
			secondBulletHistory.clear();

		}
	} else if(gameOverScreenIsUp){
		//std::cout << "3" << std::endl;
		// Exit the game over screen 

		// if (!visitedOverScreen) {
		// 	cameraAdjustmentDummy = new PlayerEntity();
		// 	GameEngine::GameEngineMain::GetInstance()->AddEntity(cameraAdjustmentDummy);
		// 	cameraAdjustmentDummy->SetPos(sf::Vector2f(410.f, 500.f));
		// 	cameraAdjustmentDummy->SetSize(sf::Vector2f(72.f, 97.f));
		// 	visitedOverScreen = true;
		// } else {
		// 	if (cameraAdjustmentDummy != nullptr) {
		// 		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(cameraAdjustmentDummy);
		// 		cameraAdjustmentDummy = nullptr;
		// 	}
		// }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
						//std::cout << 4 << std::endl;

			SpawnAllEnemies();
			visitedOverScreen = false;
			flag1Set = false; flag2Set = false;
			// std::cout << "4" << std::endl;
			uiComponent->displayState = 0;
			gameOverScreenIsUp = false;
			m_isGameOver = false;
			SpawnPlayerEntity();
			CreateGems();
			for(Game::EnemyEntity* e:  m_enemies){ GameEngine::GameEngineMain::GetInstance() -> RemoveEntity(e);}
            	m_enemies.clear();
				SpawnAllEnemies();
			// m_player->SetPos(sf::Vector2f(410.f, 500.f));
			// m_player->m_playerMovementComponent->velocity.x = 0;
			// GameEngine::CollidablePhysicsComponent* playerCPC = m_player->GetComponent<GameEngine::CollidablePhysicsComponent>();
			// playerCPC->vel = sf::Vector2f(0,0);
			// playerCPC->accel = sf::Vector2f(0,0);
			// m_player->m_playerMovementComponent->frameCnt = 0;

			if (numLives <= 2) {
				phantom_1 = new PlayerEntity(1);
				// std::cout << "create P1" << std::endl;
				GameEngine::GameEngineMain::GetInstance()->AddEntity(phantom_1);
				phantom_1->SetPos(sf::Vector2f(410.f, 500.f));	
				phantom_1->SetSize(sf::Vector2f(72.f, 97.f));
				phantom_1->id = 1;
				phantom_1->m_playerMovementComponent->frameCnt = 0;
				phantom_1->m_renderComponent->SetTexture(GameEngine::eTexture::Player1);
				phantom_1->m_animComponent->PlayAnim(GameEngine::EAnimationId::Player1Walking);
				m_player->m_renderComponent->SetTexture(GameEngine::eTexture::Player2);
				m_player->m_animComponent->PlayAnim(GameEngine::EAnimationId::Player2Walking);
			} 
			if (numLives == 1) {
				phantom_2 = new PlayerEntity(2);
				// std::cout << "create P2" << std::endl;
				GameEngine::GameEngineMain::GetInstance()->AddEntity(phantom_2);
				phantom_2->SetPos(sf::Vector2f(410.f, 500.f));	
				phantom_2->SetSize(sf::Vector2f(72.f, 97.f));
				phantom_2->id = 2;

				phantom_2->m_renderComponent->SetTexture(GameEngine::eTexture::Player2);
				phantom_2->m_animComponent->PlayAnim(GameEngine::EAnimationId::Player2Walking);
				m_player->m_renderComponent->SetTexture(GameEngine::eTexture::Player3);
				m_player->m_animComponent->PlayAnim(GameEngine::EAnimationId::Player3Walking);
				
			}

			// GameEngine::GameEngineMain::GetInstance()->RemoveEntity(gameOverEntity);
			// gameOverEntity = nullptr;
			ResetCountDown();
			//ResetEnemiesToStart();
		}
	}
}

void GameBoard::ResetEnemiesToStart()
{
	std::cout << "ResetEnemiesToStart" << std::endl;
	for (Game::EnemyEntity *e : m_enemies)
	{
		e->SetPos(e->startPos);
	}
}
void GameBoard::UpdateEnemyDying()
{
	if (!m_enemies.empty())
	{
		for (int i = m_enemies.size() - 1; i >= 0; i--)
		{
			if (m_enemies.at(i)->hitBullet)
			{
				std::cout << "enemy shot" << std::endl;
				score++;
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(m_enemies.at(i));
				m_enemies.erase(m_enemies.begin() + i);
			}
		}
	}
}

void GameBoard::UpdatePlayerDying()
{
	if (m_player->GetPos().y > 1500 || numUpdates <= 0)
		m_isGameOver = true;
	if (m_player->hitEnemy)
	{
		std::cout << "hitEnemy" << std::endl;
		m_isGameOver = true;
		m_player->hitEnemy = false;
	}
	if(phantom_1!= nullptr && phantom_1->hitEnemy){
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_1);
		phantom_1 = nullptr;
	}	
	if(phantom_2!= nullptr && phantom_2->hitEnemy){
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(phantom_2);
		phantom_2 = nullptr;
	}	
	// GameEngine::CollidablePhysicsComponent* cpc = m_player->GetComponent<GameEngine::CollidablePhysicsComponent>();
	// GameEngine::CollidablePhysicsComponent* ecpc = m_enemy_entity->GetComponent<GameEngine::CollidablePhysicsComponent>();
	// if(cpc && ecpc){
	// 	GameEngine::AABBRect playerBox = cpc -> GetWorldAABB();
	// 	GameEngine::AABBRect enemyBox = ecpc -> GetWorldAABB();
	// 	if(playerBox.intersects(enemyBox)) m_isGameOver = true;
	// 	// if((cpc->collidingTop != nullptr && cpc->collidingTop->GetEntity()-> entityType == GameEngine::EntityType::Enemy)
	// 	// || (cpc->collidingBottom != nullptr && cpc->collidingBottom->GetEntity()-> entityType == GameEngine::EntityType::Enemy)
	// 	// || (cpc->collidingLeft != nullptr && cpc->collidingLeft->GetEntity()-> entityType == GameEngine::EntityType::Enemy)
	// 	// || (cpc->collidingRight != nullptr && cpc- >collidingRight->GetEntity()-> entityType == GameEngine::EntityType::Enemy))

	// }
}