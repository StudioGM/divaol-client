#include "TitleScreenPressAnyKey.h"

namespace diva
{
	TitleScreenPressAnyKey::TitleScreenPressAnyKey()
	{
		now = 0;
		period = 0;
	}

	void TitleScreenPressAnyKey::restart()
	{
		now = 0;
	}

	void TitleScreenPressAnyKey::setPara(float period)
	{
		this->period = period;
	}

	void TitleScreenPressAnyKey::update(float dt)
	{
		//sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		now += dt;
		float t = now - int(now / period) * period;
		float h = period / 2;
		if (t<=h)
			setAlpha(int(255 * (t/h))); 
		else
			setAlpha(int(255 * ((2*h-t)/h))); 
	}
}