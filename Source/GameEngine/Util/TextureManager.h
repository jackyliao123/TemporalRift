#pragma once
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace GameEngine
{
	namespace eTexture
	{
		enum type
		{
			None = -1,
			Background = 0,
			Button,
			Flag,
			Gem,
			BlockGrass,
			Ladder,
			FoliageBush,
			FoliageDirt,
			FoliageGrass,
			FoliagePebble,
			FoliageRock,
			FoliageRockMoss,
			FoliageRockMossAlt,
			Cloud1,
			Cloud2,
			Cloud3,
			EnemyFly,
			EnemySlime,
			Player1,
			Player2,
			Player3,
			Raygun,
			RaygunLaser,
			Count,
		};
	}	

	inline const char* GetPath(eTexture::type texture)
	{
		switch(texture) {
			case eTexture::Background:
				return "background/bg_sky.png";
			case eTexture::Button:
				return "blocks/button_spritesheet.png";
			case eTexture::Flag:
				return "blocks/flag_spritesheet.png";
			case eTexture::Gem:
				return "blocks/gem.png";
			case eTexture::BlockGrass:
				return "blocks/grass_spritesheet.png";
			case eTexture::Ladder:
				return "blocks/ladder_spritesheet.png";
			case eTexture::FoliageBush:
				return "blocks/foliage/bush.png";
			case eTexture::FoliageDirt:
				return "blocks/foliage/dirt.png";
			case eTexture::FoliageGrass:
				return "blocks/foliage/grass.png";
			case eTexture::FoliagePebble:
				return "blocks/foliage/pebble.png";
			case eTexture::FoliageRock:
				return "blocks/foliage/rock.png";
			case eTexture::FoliageRockMoss:
				return "blocks/foliage/rockMoss.png";
			case eTexture::FoliageRockMossAlt:
				return "blocks/foliage/rockMossAlt.png";
			case eTexture::Cloud1:
				return "blocks/foliage/cloud1.png";
			case eTexture::Cloud2:
				return "blocks/foliage/cloud2.png";
			case eTexture::Cloud3:
				return "blocks/foliage/cloud3.png";
			case eTexture::EnemyFly:
				return "enemies/fly_spritesheet.png";
			case eTexture::EnemySlime:
				return "enemies/slime_spritesheet.png";
			case eTexture::Player1:
				return "players/p1_spritesheet.png";
			case eTexture::Player2:
				return "players/p2_spritesheet.png";
			case eTexture::Player3:
				return "players/p3_spritesheet.png";
			case eTexture::Raygun:
				return "gun/raygunBig.png";
			case eTexture::RaygunLaser:
				return "gun/laserPurpleFilled.png";
			default:
				return "UnknownTexType";
		}
	}

	class TextureManager
	{
	public:
		static TextureManager* GetInstance() { if (!sm_instance) sm_instance = new TextureManager(); return sm_instance; }
		~TextureManager();

		void LoadTextures();
		void UnLoadTextures();

		sf::Texture* GetTexture(eTexture::type texture) const { return m_textures[(int)texture]; }

	private:
		TextureManager();
		static TextureManager* sm_instance;

		sf::Texture* m_textures[eTexture::Count];
	};
}

namespace TextureHelper
{
	sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture);
}

