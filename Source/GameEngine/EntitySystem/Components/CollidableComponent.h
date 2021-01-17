#pragma once
#include "GameEngine/EntitySystem/Component.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace GameEngine
{
	typedef sf::Rect<float> AABBRect;

	class CollidableComponent : public Component
	{
	public:
		CollidableComponent();
		virtual ~CollidableComponent();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;
		virtual void Update() override;

		void MoveToNewLocation(sf::Vector2f newLoc);

		const AABBRect GetLocalAABB() const { return m_AABBBox; }
		const AABBRect GetWorldAABB() const;

		void SetBoundingBox(sf::Vector2f size);

	protected:
		void SetupDefaultBoundingBox();
		
		bool	 m_useDefaultBox;
		AABBRect m_AABBBox;

	public:
		bool hasLocationUpdate = false;
		sf::Vector2f updatedLoc;
	};
}
