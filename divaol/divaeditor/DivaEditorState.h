/*
 *  DivaCoreState.h
 *
 *  Created by CK on 2012/7/6.
 *  Copyright 2012 CK. All rights reserved.
 *
 */

#ifndef DIVA_EDITOR_STATE
#define DIVA_EDITOR_STATE

#include "DivaEditor.h"

#include "app/SoraGameState.h"
#include "SoraLogger.h"

namespace divaeditor
{
	using namespace sora;

	class Editor;

	typedef Editor* EditorPtr;

	/*
	EditorState

	Specific four edition steps
	

	*/

	class EditorState : public SoraGameState, public sora::SoraEventHandler
	{
	protected:
		EditorPtr editor;

		void setEditorState(int state);
	public:
		void setEditor(EditorPtr editor) {this->editor=editor;}
	};

}







#endif