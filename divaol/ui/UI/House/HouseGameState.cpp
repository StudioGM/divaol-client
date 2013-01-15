#include "ui/UI/House/HouseGameState.h"
#include "divacore/Core/DivaCore.h"
#include "divacore/Mode/DivaSinglePlay.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace HouseUI
	{

			void HouseGameState::onKeyPressed(sora::SoraKeyEvent& keyEvent) {
				if(keyEvent.getKey()==SORA_KEY_P) {
					if(houseUI->getState() == HouseUI::STATE_ROOM)
					{
						if(houseUI->mgr->GetMB()->isTopWindow())
							houseUI->mgr->CloseTopWindow();
						if(MAPMGR.GetSelectedMaps().size()==0)
							return;

						Base::Path songPath = MAPMGR.GetDivaOLFilePath(MAPMGR.GetSelectedMaps()[0].id, static_cast<divamap::DivaMap::LevelType>(MAPMGR.GetSelectedMaps()[0].level)); 
						if(!MAPMGR.isMapLeagal(MAPMGR.GetSelectedMaps()[0].id, static_cast<divamap::DivaMap::LevelType>(MAPMGR.GetSelectedMaps()[0].level)))
						{
							houseUI->mgr->GetMB()->Show(L"您的视听文件未通过CK大大验证！", L"提示", gcn::MessageBoxEx::TYPE_OK); 
							return;
						}
					
			
						CORE_PTR->setSong(songPath.filePath().str(), songPath.fileName());

						CORE_PTR->setInitState("load");
						CORE_PTR->registerGameMode(new divacore::SinglePlay);

						NextState = "core";
					}
				}
			}
	}
}