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
#include "SoraCanvas.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraFMODSoundSystem.h"
#include "ui/Config/DivaUIConfig.h"
#include "ui/UI/House/HouseGameState.h"
#include "ui/UI/Stage/StageGameState.h"
#include "ui/UI/Music/MusicGameState.h"
#include "ui/UI/TitleScreen/TitleScreenUI.h"

#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaGNetTCPSystem.h"
using namespace diva;

class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
public:
	GameInitState() {
		//this->app = app;
		isInitialized = false;
		//nextState = "";
	}

	~GameInitState()
	{
		if (uiCanvas)
			delete uiCanvas;
	}

	void onRender() {


		uiCanvas->beginRender(0xFFFFFFFF);

		//room->Render();
		titleScreen->Render();
		sora::GCN_GLOBAL->gcnDraw();

		uiCanvas->finishRender();



		sora::SoraGameApp::BeginScene();

		sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
		sprite->setScale(double(config[L"windowWidth"].asInt()) / config[L"gameWidth"].asInt(), double(config[L"windowHeight"].asInt()) / config[L"gameHeight"].asInt());
		sprite->render(0, 0);

		sora::SoraGameApp::EndScene();


	}

	void onLeave()
	{
		titleScreen->Leave();
	}

	void onUpdate(float dt) {   
		//DivaCEGUI::Instance()->Update(dt);
		//room->Update(dt);
		if (NextState!="")
		{
			getGameApp()->setState(NextState);
			NextState = "";
			return;
		}
		titleScreen->Update(dt);
	}


	void onKeyPressed(sora::SoraKeyEvent& keyEvent) {
		//getGameApp()->setState("house");
		NextState = "house";
	}

	void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
		//y += to.x - from.x;
		//getGameApp()->setState("house");
	}

	void onEnter() {

		if (!isInitialized)
		{
			//diva::MapMgr::Instance()->Load("map_manifest.json");

			//room = UIScreenRoom::Instance();
			sora::GCN_GLOBAL->initGUIChan(L"msyh.ttf", 20);
			titleScreen = TitleScreenUI::Instance();

			uiCanvas = new sora::SoraBaseCanvas(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());

			isInitialized = true;
		}

		sora::SoraCore::Ptr->setFPS(60);
		titleScreen->Enter();

	}

private:

	sora::SoraBaseCanvas* uiCanvas;
	//std::string nextState;
	TitleScreenUI* titleScreen;
	bool isInitialized;
	//sora::SoraGameApp* app;
};

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


		app.run("init");

		return 0;
}
