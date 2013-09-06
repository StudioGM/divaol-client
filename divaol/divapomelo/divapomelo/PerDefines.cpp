#include "PreDefines.h"

namespace divapomelo {
	const char* EventCode[] =  {
		"onEnter",
		"onLeave",
		"onChat",
		"onKick",
		"stage.onSetMode",
		"stage.onSetHook",
		"stage.onSetSong",
		"stage.onDraw",
		"stage.onReady",
		"stage.onUnready",
		"stage.onJoin",
		"stage.onLeave",
		"stage.onCreate",
		"stage.onClose",
		"stage.onAllInfo",
		"stage.onStart",
		"stage.onReturn",
		"game.onStart",
		"game.onHeartbeat",
		"game.onFailure",
		"game.onRenew",
		"game.relay.onVoidPeriod",
		"game.relay.onChange",
		"game.relay.onYouCan",
		"game.relay.onNewPlayer"
	};
	const char* RequestCode[] = {
		"lobby.chatHandler.send",
		"lobby.lobbyHandler.getStageList",
		"lobby.lobbyHandler.createStage",
		"lobby.lobbyHandler.joinStage",
		"lobby.lobbyHandler.leaveStage",
		"lobby.stageHandler.draw",
		"lobby.stageHandler.setSong",
		"lobby.stageHandler.setMode",
		"lobby.stageHandler.setHook",
		"lobby.stageHandler.ready",
		"lobby.stageHandler.start"
	};
}