#include <winsock2.h>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraDirectoryIterator.h"
#include "SoraFileUtility.h"
#include "SoraFont.h"
#include "SoraText.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"

#include "SoraResourceFile.h"

#include "SoraFMODSoundSystem.h"
#include "ui/Config/DivaUIConfig.h"
#include "ui/UI/House/HouseGameState.h"
#include "ui/UI/Stage/StageGameState.h"
#include "ui/UI/Music/MusicGameState.h"
#include "ui/UI/TitleScreen/TitleGameState.h"

#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaGNetTCPSystem.h"
#include "divacore/Utility/DivaInitialize.h"
using namespace diva;



#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	) {
#ifdef DIVA_GNET_OPEN
		divanet::NetworkManager::instance().init();
		divanet::NetworkManager::instance().setAuth(new divanet::TCPGNetworkSystem);
		divanet::NetworkManager::instance().setChat(new divanet::TCPGNetworkSystem);
		divanet::NetworkManager::instance().setScheduler(new divanet::TCPGNetworkSystem);
		divanet::NetworkManager::instance().setCore(new divanet::TCPGNetworkSystem);

		sora::SoraCore::SetRandomSeed((uint32)time(0));

		divacore::standard::Initializer initializer("system",divacore::standard::Initializer::MULTI, true);
		divacore::CorePtr core = initializer.get();

		divacore::Config core_config;
		divacore::configloader::loadWithJson(core_config,"system/common.json");
		core->setSong(core_config.getAsWString("song"),core_config.getAsWString("map"));

		core->myPlayerInfo().loadFromFile("system/playerInfo.json");
#endif
		diva::initialize_config(L"uiconfig/config.json");

		sora::SoraGameAppDef def("config.xml");
		def.width(config[L"windowWidth"].asInt());
		def.height(config[L"windowHeight"].asInt());
		sora::SoraGameApp app(def);

		sora::SoraCore::Instance()->registerSoundSystem(new sora::SoraFMODSoundSystem());
		app.addState(new GameInitState, "init");
		app.addState(new diva::HouseUI::HouseGameState(), "house");
		app.addState(new diva::StageUI::StageGameState(), "stage");
		app.addState(new diva::MusicUI::MusicGameState(), "music");

#ifdef DIVA_GNET_OPEN
		app.addState(core, "core");
#endif

		app.run("init");

		return 0;
}
