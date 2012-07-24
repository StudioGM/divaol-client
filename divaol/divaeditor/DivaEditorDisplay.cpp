#include "divaeditor/DivaEditorDisplay.h"
#include "divacore/Component/DivaStandardDisplay.h"

namespace divaeditor
{
	DivaEditorDisplay::DivaEditorDisplay() : StandardDisplay()
	{
		myGrid = NULL;
	}



	void DivaEditorDisplay::render()
	{
		divacore::StandardDisplay::render();
		myGrid->render(110,90);
	}

	void DivaEditorDisplay::update(float dt)
	{
		divacore::StandardDisplay::update(dt);
		myGrid->update(dt);
	}

	void DivaEditorDisplay::gameStart()
	{
		if(!myGrid)
		{
			myGrid = new SoraSprite(SoraTexture::LoadFromFile("standard/pic/grid2.png"),0,0,1668,900);
			myGrid->fadeTo(0.8,0);
		}
	}
}