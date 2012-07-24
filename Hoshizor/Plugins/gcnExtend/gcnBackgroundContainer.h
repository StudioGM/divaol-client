/*
 *  gcnBackgroundContainer.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_BACKGROUND_CONTAINER_H_
#define GCN_BACKGROUND_CONTAINER_H_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	/*
		a simple container with background image support
	 */
	class BackgroundContainer: public Container {
	public:
		BackgroundContainer();
		BackgroundContainer(const std::string& backgroundImage);
		
		virtual void draw(Graphics* graphics);
		
		void setImage(const std::string& backgroundImage);
		virtual void adjustSize();
		
		Image* getImage() const;
		
	protected:
		Image* pBackgroundImage;
	};
	
	
} // namespace gcn


#endif