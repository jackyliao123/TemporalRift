#include "TextureManager.h"

using namespace GameEngine;

TextureManager *TextureManager::sm_instance = nullptr;

TextureManager::TextureManager()
{
	for (int a = 0; a < (int)eTexture::Count; ++a)
	{
		m_textures[a] = nullptr;
	}
}

TextureManager::~TextureManager()
{
	UnLoadTextures();
}

void TextureManager::LoadTextures()
{
	for (int a = 0; a < (int)eTexture::Count; ++a)
	{
		sf::Texture *texture = new sf::Texture();
		std::string filePath;
		filePath.append("Resources/img/");
		filePath.append(GetPath((eTexture::type)a));
		texture->loadFromFile(filePath);

		m_textures[a] = texture;
	}
}

void TextureManager::UnLoadTextures()
{
	for (int a = 0; a < (int)eTexture::Count; ++a)
	{
		delete m_textures[a];
		m_textures[a] = nullptr;
	}
}

namespace TextureHelper
{
	sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture)
	{
		switch (texture)
		{
		case eTexture::Flag:
		case eTexture::Ladder:
		case eTexture::Button:
		case eTexture::BlockGrass:
			return sf::Vector2f(70.f, 70.f);
		case eTexture::EnemyFly:
			return sf::Vector2f(75.f, 36.f);
		case eTexture::EnemySlime:
			return sf::Vector2f(59.f, 30.f);
		case eTexture::Player1:
		case eTexture::Player2:
		case eTexture::Player3:
			return sf::Vector2f(72.f, 97.f);
		default:
			return sf::Vector2f(-1.f, -1.f);
		}
	}
} // namespace TextureHelper
