#include <winsock2.h>

//#define NET
//#define EDIT

#include "app/SoraGameApp.h"
#include "Utility/DivaInitialize.h"

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
  int nCmdShow
) {  
   

	try
	{
		divacore::standard::Initializer initializer("system",divacore::standard::Initializer::EDIT, false);
		divacore::CorePtr core = initializer.get();

		divacore::Config config;
		divacore::configloader::loadWithJson(config,"system/common.json");
		core->setSong(config.getAsString("song"),config.getAsString("map"));

		sora::SoraGameAppDef def;
		sora::SoraGameApp app(def);
		app.addState(core, "core");

		def.width(config.getAsInt("windowWidth"));
		def.height(config.getAsInt("windowHeight"));

		app.run("core");

		//sora::SoraTaskManager::defaultManager(true).joinAll();
	}
	catch (divacore::Exception&ev)
	{
		divacore::LOGGER->error(ev);
		MessageBox(0,sora::s2ws(ev.getContent()).c_str(),sora::s2ws(ev.getModuleName()).c_str(),0);
	}

	//ExitProcess(0);

	return 0;

}