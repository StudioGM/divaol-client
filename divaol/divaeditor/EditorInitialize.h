#ifndef EDITOR_INIT
#define EDITOR_INIT


#include "divaeditor/Core/DivaEditor.h"

#include "divacore/Core/DivaCore.h"
#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaGNetTCPSystem.h"

namespace divaeditor
{
	namespace standard
	{
		EditorPtr initialize()
		{
			NET_MANAGER.setCore(new divanet::TCPGNetworkSystem);
			divacore::standard::Initializer initializer("system",divacore::standard::Initializer::EDIT);
			
			divacore::CorePtr core = initializer.get();
			//divacore::CorePtr core = divacore::standard::initialize("system");
			core->myPlayerInfo().loadFromFile("system/playerInfo.json");
			EditorPtr editor = INITIALIZE_EDITOR;
			editor->initDivaCoreForEditor();
			
			return editor;
		}
	}
}


#endif