#ifndef EDITOR_INIT
#define EDITOR_INIT


#include "DivaEditor.h"
#include "divacore/Core/DivaCore.h"


namespace divaeditor
{
	namespace standard
	{
		EditorPtr initialize()
		{
			divacore::CorePtr core = divacore::standard::initialize("system");
			core->setSong("song/ǧ���@","ǧ���@_Hard.divaol");
			EditorPtr editor = INITIALIZE_EDITOR;
			editor->registerDivaCore(core);

			return editor;
		}
	}
}


#endif