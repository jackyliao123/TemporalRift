#pragma once
#include "GameEngine/Util/TextureManager.h"
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include "GameEngine/Util/MovableGroup.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <queue>
#include <utility>

namespace Game
{
	//Used for storing and controlling all game related entities, as well as providing an entry point for the "game" side of application
	class PlayerEntity;
	class EnemyEntity;
	class ButtonEntity;
	class GemEntity;
	class FlagEntity;
	class FoliageEntity;
	class UIRenderComponent;

	enum zIndex
	{
		Background = 100,
		Clouds,
		Blocks,
		BackFoliage,
		Foliage,
		Enemies,
		Objectives,
		Player1,
		Player2,
		Player3,
		FrontFoliage,
		Monitor,
		UI,
	};

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();

		void Update();
		bool IsGameOver() { return false; }
		void SpawnNewObstacle(const sf::Vector2f &pos, const sf::Vector2f &size);

		void CreateTestMap();
		void CreateMap();
		void CreateBackGround();

		FoliageEntity* SpawnFoliage(
			GameEngine::eTexture::type texture, 
			zIndex z,
			const sf::Vector2f &pos
		);

		void SpawnPlayerEntity();
		void SpawnEnemy(sf::Vector2f start, sf::Vector2f end, int id, sf::Vector2f initialPos);
		void SpawnAllEnemies();
		void UpdatePlayerDying();
		void UpdateCountDown();
		void ResetCountDown();
		void UpdateEnemyDying();
		void ResetEnemiesToStart();
		void CreateGems();
		void CheckWin();

		PlayerEntity* m_player;
		PlayerEntity* cameraAdjustmentDummy;
		std::vector<EnemyEntity*>m_enemies; // change to vector of enemies later?
		PlayerEntity* phantom_1;
		PlayerEntity* phantom_2;

		std::vector<GameEngine::Entity*> m_obstacles;
		bool  m_isGameOver;
		int numLives;
		bool gameOverScreenIsUp;
		GameEngine::Entity* gameOverEntity;
		GameEngine::Entity* countDownEntity;
		UIRenderComponent* uiComponent;
		int numUpdates;

		bool visitedOverScreen = false;

		bool newRound = true; //Only true for the first frame of any given round
		std::vector<std::pair<int, sf::Keyboard::Key>> firstMovementHistory;
		std::vector<std::pair<int, sf::Vector2f>> firstBulletHistory;
		
		std::vector<std::pair<int, sf::Keyboard::Key>> secondMovementHistory;
		std::vector<std::pair<int, sf::Vector2f>> secondBulletHistory;

		GemEntity* gem1 = nullptr, *gem2 = nullptr, *gem3 = nullptr, *gem4 = nullptr;
		GameEngine::MovableGroup platformGroup1;
		GameEngine::MovableGroup platformGroup2;

		ButtonEntity *button1 = nullptr, *button2 = nullptr;
		bool button1Pressed = false, button2Pressed = false;

		FlagEntity* flag1 = nullptr, *flag2 = nullptr;
		bool flag1Set = false, flag2Set = false;
		
		int score = 0;
		bool win = false;
		bool hasWinScreen = false;

	};
	
} // namespace Game
