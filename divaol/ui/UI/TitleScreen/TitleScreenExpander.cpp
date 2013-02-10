#include "TitleScreenExpander.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

namespace diva
{

	TitleScreenExpander::TitleScreenExpander()
	{
		period = 0;
		fPeriod = 0;
		now = 0;
	}

	TitleScreenExpander::~TitleScreenExpander()
	{

	}

	void TitleScreenExpander::restart()
	{
		now = 0;
	}

	void TitleScreenExpander::setPara(float period, float fPeriod, float sc)
	{
		this->period = period;
		this->fPeriod = fPeriod;
		this->sc = sc;
	}

	void TitleScreenExpander::update(float dt)
	{
		now += dt;
		sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		float t = now - (int(now / period)) * period;
		float h = fPeriod / 2;
		if (t >= fPeriod)
			sprite->setScale(1.0f, 1.0f);
		else if (t<h)
			sprite->setScale((sc - 1.0) * t / h + 1.0, (sc - 1.0) * t / h + 1.0);
		else
			sprite->setScale((sc - 1.0) * (2*h - t) / h + 1.0, (sc - 1.0) * (2*h - t) / h + 1.0);
	}

}