//
//  animation.h
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_animation_h
#define Sora_animation_h

#include "guichan/platform.hpp"
#include "guichan/modifier.hpp"

#include "Hoshizora.h"

namespace gcn {
    
    /**
     * The base class represents a kind of gui animation
     * Animation details should be implemented by subclasses
     * It's also a kind of modifier so it's easy to apply a animation 
     *  to a widget by calling the addModifier function of the widget
     * 
     * @author Robert Bu(darkfall)
	 * @Animation Extended by CK
     * @since GuiChan for Hoshizora
     */

	using namespace sora;

	enum GUIAnimation_Mode
	{
		NONE,
		PINGPONG,
		REPEAT
	};

    
    class GCN_CORE_DECLSPEC Animation: public Modifier {
	protected:
		Animation *_nextAnimation;
		SoraFunction<void(Widget*)> _AnimationEnded;
		GUIAnimation_Mode _animationMode;

    public:
        // constant animation message strings
        static std::string MessageStart;
        static std::string MessageEnd;
        
        Animation(int time, GUIAnimation_Mode animationMode, Animation *nextAnimation, SoraFunction<void(Widget*)> AnimationEnded);
        
        virtual ~Animation() {}
        
        /**
         * Inherited from Modifier
         * Would update the time info of the animation
         * Subclasses should call this during their update to get corretly time info
         * for example
         *
         *  void myUpdate(Widget* widget) {
         *      Animation::update(widget);
         *      float ratio = getCurrTimeRatio();
         *     
         *      // set the alpha of the widget
         *      widget->setAlpha(ratio * 255);
         *  }
         */
        virtual void update(Widget* widget);
        
        /**
         * Reset the animation
         * 
         * @param The new total time, default = 0, use the same total time as before
         */
        virtual void reset(int time=0);
        
        /**
         * Inherited from Modifier
         */
        virtual void setFinish(bool flag);
       
        /**
         * Get the total time of the animation
         */
        int getTotalTime() const;
        
        /**
         * Get the current time of the animation
         * Current time would increase by 1 every update loop
         */
        int getCurrTime() const;
        
        /**
         * Get the current time ratio of the aniamtion
         * Equals to getCurrTime() / getTotalTime()
         * Would return 1 if the animation had finished
         */
        float getCurrTimeRatio() const;
        
        // inherited from Modifier
        virtual void onAdd(Widget* widget);
        virtual void onRemove(Widget* widget);
        virtual void onRelease(Widget* widget);
		virtual void release();
        
        virtual std::string getName() const = 0;
        
    protected:
        int mTotalTime;
        int mCurrTime;
    };
    
} // namespace gcn



#endif
