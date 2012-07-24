#ifndef DIVA_EDITOR_DISPLAY
#define DIVA_EDITOR_DISPLAY

#include "divacore/Component/DivaStandardDisplay.h"
#include "SoraSprite.h"

namespace divaeditor
{
	using namespace divacore;

	class DivaEditorDisplay : public divacore::StandardDisplay
	{
	private:
		SoraSprite *myGrid;
	public:
		DivaEditorDisplay();

		virtual void render();
		virtual void update(float dt);

		virtual void gameStart();
	};

}


#endif