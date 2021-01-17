#include "AnimationManager.h"

using namespace GameEngine;

AnimationManager *AnimationManager::sm_instance = nullptr;

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::InitStaticGameAnimations()
{
	// Player 1
	{
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player1Idle,
				eTexture::Player1,
				sf::Vector2i(0, 1),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player1Damage,
				eTexture::Player1,
				sf::Vector2i(0, 2),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player1Jumping,
				eTexture::Player1,
				sf::Vector2i(0, 3),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player1Walking,
				eTexture::Player1,
				sf::Vector2i(0, 4),
				11,
				22));
	}

	// Player 2
	{
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player2Idle,
				eTexture::Player2,
				sf::Vector2i(0, 1),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player2Damage,
				eTexture::Player2,
				sf::Vector2i(0, 2),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player2Jumping,
				eTexture::Player2,
				sf::Vector2i(0, 3),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player2Walking,
				eTexture::Player2,
				sf::Vector2i(0, 4),
				11,
				22));
	}

	// Player 3
	{
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player3Idle,
				eTexture::Player3,
				sf::Vector2i(0, 1),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player3Damage,
				eTexture::Player3,
				sf::Vector2i(0, 2),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player3Jumping,
				eTexture::Player3,
				sf::Vector2i(0, 3),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Player3Walking,
				eTexture::Player3,
				sf::Vector2i(0, 4),
				11,
				22));
	}

	// Enemy
	{
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::EnemyFlyMoving,
				eTexture::EnemyFly,
				sf::Vector2i(0, 0),
				2,
				2));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::EnemyFlyDamage,
				eTexture::EnemyFly,
				sf::Vector2i(0, 2),
				1,
				1));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::EnemySlimeMoving,
				eTexture::EnemySlime,
				sf::Vector2i(0, 0),
				2,
				2));
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::EnemySlimeDamage,
				eTexture::EnemySlime,
				sf::Vector2i(0, 2),
				1,
				1));
	}

	// World
	{
		m_animDefinitions.push_back(
			SAnimationDefinition(
				EAnimationId::Flag,
				eTexture::Flag,
				sf::Vector2i(0, 0),
				2,
				2));
	}
}

void AnimationManager::ReleaseStaticGameAnimations()
{
	m_animDefinitions.clear();
}

const SAnimationDefinition *AnimationManager::GetAnimDefinition(EAnimationId::type animId) const
{
	for (int a = 0; a < m_animDefinitions.size(); ++a)
	{
		if (m_animDefinitions[a].m_animId == animId)
			return &m_animDefinitions[a];
	}

	return nullptr;
}
