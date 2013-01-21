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
#include "ui/UI/Init/InitGameState.h"

#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaGNetTCPSystem.h"
#include "divacore/Utility/DivaInitialize.h"
#include "divacore/Utility/DivaSettings.h"

using namespace diva;



#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	) {
		diva::initialize_config(L"config/SettingConfig.json", L"config/config.json");
		//std::wstring ss = config[L"sdf"].asString();
		// initialize settings

#ifdef DIVA_GNET_OPEN
		NET_INFO.getServerInfo();
		divanet::NetworkManager::instance().init();
		//divanet::NetworkManager::instance().setScheduler(new divanet::TCPGNetworkSystem);
		//divanet::NetworkManager::instance().setCore(new divanet::TCPGNetworkSystem);
		AUTH_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		CHAT_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		SCHEDULER_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		STAGE_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);

		Base::Random::SetRandomSeed((uint32)time(0));
		sora::SoraCore::SetRandomSeed((uint32)time(0));

		divacore::standard::Initializer initializer("config",divacore::standard::Initializer::SINGLE, true);
		divacore::CorePtr core = initializer.get(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());

		MY_PLAYER_INFO.loadFromFile(Base::String(config[L"tmpInfoFile"].asString()));
		// only need these for core solo
		//divacore::Config core_config;
		//divacore::configloader::loadWithJson(core_config,"system/common.json");
		//core->setSong(core_config.getAsWString("song"),core_config.getAsWString("map"));
#endif

		divacore::Settings::instance().InitializeSettings(setConfig, config);

		sora::SoraGameAppDef def("config.xml");
		def.windowMode(setConfig[L"isWindowMode"].asBool());
		def.width(setConfig[L"windowWidth"].asInt());
		def.height(setConfig[L"windowHeight"].asInt());
		sora::SoraGameApp app(def);

		sora::SoraCore::Instance()->registerSoundSystem(new sora::SoraFMODSoundSystem());

		app.addState(new InitGameState, "init");
		app.addState(new TitleGameState, "title");
		app.addState(new diva::HouseUI::HouseGameState(), "house");
		app.addState(new diva::StageUI::StageGameState(), "stage");
		app.addState(new diva::MusicUI::MusicGameState(), "music");

#ifdef DIVA_GNET_OPEN
		app.addState(core, "core");
#endif
		app.run("init");

		return 0;
}
