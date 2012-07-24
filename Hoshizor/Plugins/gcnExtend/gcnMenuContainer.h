/*
 *  gcnMenuContainer.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_MENU_CONTAINER_H_
#define GCN_MENU_CONTAINER_H_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	/*
	 a class that containes widget
	 and a title (optical) for a simple menu
	 mouse event would be handled by widget itself
	 only handle keyboard events
	 */
	class MenuContainer: public Container, public KeyListener {
	public:
		enum MenuItemAlignment {
			ALIGN_LEFT,
			ALIGN_MIDDLE,
			ALIGN_RIGHT,
		};
		
		MenuContainer();
		/*
		 is bIsImage == true,
		 then the title would be treated as a path to a image file
		 otherwise title would be rendered as string
		 */
		MenuContainer(const std::string& title, bool bIsImage=true);
		~MenuContainer();
		
		void setTitleImage(const std::string& title, bool bIsImage=true);
		
		/* 
		 inheritated from KeyListener 
		 */
		virtual void keyPressed(KeyEvent& keyEvent);
        virtual void keyReleased(KeyEvent& keyEvent);
		
		/*
		 inheritated from Container
		 widget position would be automatically adjust by container
		 depends on alignment mode
		 */
		virtual void add(Widget* widget);
		
		void setAlignment(MenuItemAlignment align);
		MenuItemAlignment getAlignment() const;
		
		void setMoveSound(const std::string& soundName);		
		
	private:
		Image* titleImage;
		std::string titleString;
		
		int bottomPosX, bottomPosY;
		
		MenuItemAlignment alignment;
		
		Sound* mMoveSound;
	};
	
} // namespace gcn


#endif // GNC_MENU_CONTAINER_H_
