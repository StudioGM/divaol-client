/*
 *  gcnImageButton2.h
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef GCN_IMAGE_BUTTON_2_
#define GCN_IMAGE_BUTTON_2_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	/*
	 image based button
	 with sound support by Sound extension
	 */
	class ImageButton2: public Button, public ActionListener {
	public:
		ImageButton2();
		virtual ~ImageButton2();
		
		ImageButton2(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed);
		
		virtual void setImage(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed);
		virtual void setImage(Image* image, const Rectangle& normal, const Rectangle& pushed);
		
		void setClickSound(const std::string& soundName);
		void setEnterSound(const std::string& soundName);
		
		void mouseEntered(MouseEvent& mouseEvent);
		void action(const ActionEvent& actionEvent);
		
		/*
		 this would used by isFocused
		 */
		void setFocusRect(const Rectangle& normal);
		Rectangle getFocusRect() const;
		/*
		 this would used by isEnabled
		 */
		void setUnavailableRect(const Rectangle& normal);
		Rectangle getUnavailableRect() const;
		
		void setDrawRect(const Rectangle& normal, const Rectangle& pushed);
		Rectangle getNormalRect() const;
		Rectangle getPushedRect() const;
		Image* getImage() const;
		
		virtual void draw(Graphics* graphics);
		virtual void drawFrame(Graphics* graphics);
		
		void enableFrameWhenMouseEntered(bool flag);
		bool isFrameEnabledWhenMouseEntered() const;
		
		virtual void adjustSize();
		
	protected:
		Image* mImage;
		
		Rectangle normalTex;
		Rectangle pushedTex;
		Rectangle unavailTex;
		Rectangle focusTex;
		
		Sound* mClickSound;
		Sound* mEnterSound;
		
		bool mInternalImage;
		bool mFrameMouseEntered;
	};
} // namespace gcn

#endif