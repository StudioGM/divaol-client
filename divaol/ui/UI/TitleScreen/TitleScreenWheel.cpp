#include "TitleScreenWheel.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

namespace diva
{

	TitleScreenWheel::TitleScreenWheel()
	{
		rotation = 0;
	}

	void TitleScreenWheel::restart()
	{
		rotation = 0;
	}

	void TitleScreenWheel::setRSpeed(float speed)
	{
		this->speed = speed;
	}

	void TitleScreenWheel::update(float dt)
	{
		sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		rotation += speed;
		sprite->setRotation(rotation);
	}

}