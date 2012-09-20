//#define NET
//#define EDIT

#include <winsock2.h>

#include "SoraPlatform.h"

#include "app/SoraGameApp.h"
#include "Utility/DivaInitialize.h"

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")

/////////////////////
#include "divasongmgr/DivaMapManager.h"
#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaGNetTCPSystem.h"
/////////////////////

#ifdef OS_WIN32
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
  int nCmdShow
) {
#else
    int main(int argc, const char** argv) {
        
#endif
		///////////////////////////////
		
		std::list<divamap::DivaMapEventMessage> listMsgOut;
		MAPMGR.registerMapEventMessageQueue(&listMsgOut);
		//MAPMGR.PrepareDivaMapData(1, true);
		MAPMGR.PrepareDivaMapDataFromFile(L"song/MAP_1_noVideo.divaolpack");
		bool fini=false;
		while(true)
		{
			MAPMGR.update(0);
			while(listMsgOut.size()!=0)
			{
				divamap::DivaMapEventMessage msg = listMsgOut.front();
				listMsgOut.pop_front();
				if(msg.finish)
				{
					MessageBoxW(NULL, L"download over", L"download", MB_OK);
					fini=true;
				}
				else
				{
					printf("%lf", msg.downloadProgress);
				}
			}
			//if(fini)
				//break;
		}
		
		NET_MANAGER.setCoreNet(new divanet::TCPGNetworkSystem);
		///////////////////////////////

	try
	{
		sora::SoraCore::SetRandomSeed((uint32)time(0));

		divacore::standard::Initializer initializer("system",divacore::standard::Initializer::MULTI, true);
		divacore::CorePtr core = initializer.get();

		divacore::Config config;
		divacore::configloader::loadWithJson(config,"system/common.json");
		core->setSong(config.getAsWString("song"),config.getAsWString("map"));

		core->myPlayerInfo().loadFromFile("system/playerInfo.json");

		sora::SoraGameAppDef def;
		sora::SoraGameApp app(def);
		app.addState(core, "core");
        
        sora::SoraCore::Instance()->setSystemFont(sora::SoraFont::LoadFromFile("simhei.ttf", 20));

		def.width(config.getAsInt("windowWidth"));
		def.height(config.getAsInt("windowHeight"));

		app.run("core");

		//sora::SoraTaskManager::defaultManager(true).joinAll();
	}
	catch (divacore::Exception&ev)
	{
		LOGGER->error(ev);
		MessageBox(
			NULL,
			sora::s2ws(ev.getContent()).c_str(),
			sora::s2ws(ev.getModuleName()).c_str(),
			MB_ICONERROR | MB_DEFBUTTON2
			);
        //sora::SoraCore::Instance()->messageBox(ev.getContent(), ev.getModuleName(), 0);
	}

	//ExitProcess(0);

	return 0;

}