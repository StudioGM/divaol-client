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
		//if(myGrid)
		myGrid->render(0,0);
	}

	void DivaEditorDisplay::update(float dt)
	{
		divacore::StandardDisplay::update(dt);
	}

	void DivaEditorDisplay::gameStart()
	{
		if(!myGrid)
			myGrid = new SoraSprite(SoraTexture::LoadFromFile("standard/pic/MainGameMenu.png"),0,0,1557,781);
	}
}