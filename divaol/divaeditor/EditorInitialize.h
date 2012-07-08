#ifndef EDITOR_INIT
#define EDITOR_INIT


#include "DivaEditor.h"
#include "divacore/Core/DivaCore.h"


namespace divaeditor
{
	namespace standard
	{
		EditorPtr initialize(CorePtr divaCore)
		{
			EditorPtr editor = INITIALIZE_EDITOR;
			editor->registerDivaCore(divaCore);

			return editor;
		}
	}
}


#endif