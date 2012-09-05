#include "TitleScreenMovingShader.h"

#include "ui/Config/DivaUIConfig.h"

namespace diva
{
	TitleScreenMovingShader::TitleScreenMovingShader()
	{
		now = 0;
		period = 0;
	}

	void TitleScreenMovingShader::restart()
	{
		now = 0;
	}

	void TitleScreenMovingShader::setPara(float period)
	{
		this->period = period;
	}

	void TitleScreenMovingShader::update(float dt)
	{
		//sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		now += dt;
		float t = now - int(now / period) * period;
		float gameWidth = config[L"gameWidth"].asInt();
		float bar = getWidth() - 2 * gameWidth; 
		dx = int(t / period * (bar + gameWidth)) - bar - gameWidth;
		dy = 0;
	}
}