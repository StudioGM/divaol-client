#include "divaeditor/DivaEditorDisplay.h"
#include "divaeditor/DivaEditorCommon.h"

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
		if(EDITCONFIG->display_grid)
			myGrid->render(174,154);
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
			myGrid = new SoraSprite(SoraTexture::LoadFromFile("standard/pic/grid2.png"),0,0,1540,772);
			//myGrid->fadeTo(0.8,0);
		}
	}
}