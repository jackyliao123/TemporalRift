#pragma once
#include "GameEngine/EntitySystem/Component.h"

namespace Game
{	

	class ProjectileComponent: public GameEngine::Component
	{
	public:
		ProjectileComponent();
		~ProjectileComponent();

		virtual void Update() override;
		void SetLifeTime(int frames) { frameLifeTime = frames; }
		void SetDirection(int right) { direction = right; }

	private:
	    int frameLifeTime;
		bool direction; // Right is true, left is false
	};
}


