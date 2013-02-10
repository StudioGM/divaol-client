#ifndef SORA_GUI_H
#define SORA_GUI_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraGUIListener.h"
#include "SoraGUIResponser.h"

#include <map>

namespace sora {

	enum SoraGUIResponseType {
		RESPONSEACTION = 1,
		RESPONSEKEY = 2,
		RESPONSEDEATH = 4,
		RESPONSEFOCUS = 8,
		RESPONSEMOUSE = 16,
		RESPONSELUA = 32,
		RESPONSESELECTION = 64,
		RESPONSEALL = 63,
	};

//	const int RESPONSEALL = RESPONSEACTION | RESPONSEKEY | RESPONSEDEATH | RESPONSEFOCUS | RESPONSEMOUSE;

	class SoraGUI: public SoraSingleton<SoraGUI> {
        friend class XmlGui;
		friend class SoraSingleton<SoraGUI>;
        
    public:
		SoraGUI() {
			pActionListener = new SoraGUIActionListener;
			pKeyListener = new SoraGUIKeyListener;
			pDeathListener = new SoraGUIDeathListener;
			pFocusListener = new SoraGUIFocusListener;
			pMouseListener = new SoraGUIMouseListener;
			pSelectionListener = new SoraGUISelectionListener;
		}
		
		~SoraGUI() {
			delete pActionListener;
			delete pKeyListener;
			delete pDeathListener;
			delete pFocusListener;
			delete pMouseListener;
			delete pSelectionListener;
		}
        
		void registerGUIResponser(gcn::Widget* pWidget, SoraGUIResponser* pResponser, const SoraString& responserID, int iResponceType) {
			if(iResponceType & RESPONSEACTION) pWidget->addActionListener(pActionListener);
			if(iResponceType & RESPONSEDEATH) pWidget->addDeathListener(pDeathListener);
			if(iResponceType & RESPONSEKEY) pWidget->addKeyListener(pKeyListener);
			if(iResponceType & RESPONSEMOUSE) pWidget->addMouseListener(pMouseListener);
			if(iResponceType & RESPONSEFOCUS) pWidget->addFocusListener(pFocusListener);
			if(iResponceType & RESPONSESELECTION) {
				gcn::ListBox* plb = dynamic_cast<gcn::ListBox*> (pWidget);
				if(plb) {
					plb->addSelectionListener(pSelectionListener);
				}
				else {
					gcn::DropDown* pdd = dynamic_cast<gcn::DropDown*> (pWidget);
					if(pdd) pdd->addSelectionListener(pSelectionListener);
				}
			}
            
			SoraGUIResponserMap::Instance()->registerResponser(responserID, pResponser);
		}
        
        void registerExternalGUIResponser(gcn::Widget* pWidget, SoraGUIResponserExtern* pResponser, const SoraString& responserID, const SoraString& handleSrc, int iResponceType) {
            if(iResponceType & RESPONSEACTION) pWidget->addActionListener(pActionListener);
			if(iResponceType & RESPONSEDEATH) pWidget->addDeathListener(pDeathListener);
			if(iResponceType & RESPONSEKEY) pWidget->addKeyListener(pKeyListener);
			if(iResponceType & RESPONSEMOUSE) pWidget->addMouseListener(pMouseListener);
			if(iResponceType & RESPONSEFOCUS) pWidget->addFocusListener(pFocusListener);
			if(iResponceType & RESPONSESELECTION) {
				gcn::ListBox* plb = dynamic_cast<gcn::ListBox*> (pWidget);
				if(plb)
					plb->addSelectionListener(pSelectionListener);
				else {
					gcn::DropDown* pdd = dynamic_cast<gcn::DropDown*> (pWidget);
					if(pdd) pdd->addSelectionListener(pSelectionListener);
				}
			}
            
            SoraGUIResponserMap::Instance()->registerHandleSrc((SoraHandle)pWidget, handleSrc);
			SoraGUIResponserMap::Instance()->registerExternalResponser(responserID, pResponser);
        }
       
    private:
        
		SoraGUIActionListener* pActionListener;
		SoraGUIKeyListener* pKeyListener;
		SoraGUIDeathListener* pDeathListener;
		SoraGUIFocusListener* pFocusListener;
		SoraGUIMouseListener* pMouseListener;
		SoraGUISelectionListener* pSelectionListener;
	};
} // namespace sora

#endif