/*
 *  ModifierBase.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_MODIFIER_BASE_H_
#define SORA_GCN_MODIFIER_BASE_H_

#include <string>

namespace gcn {
	
	class Widget;
	
	/**
	 * The base class represents a modifier that modify the contents of a widget
     * You can use this struct to modify a widget outside of it conveniently
     * Or you can make some kind of modifier that links to widgets together
     * Like a label which would show the value of a scrollbar
     *
     * @author Robert Bu(darkfall)
     * @Since GuiChan for Hoshizora
	 */
		
	class GCN_CORE_DECLSPEC Modifier {
	public:
		Modifier(bool autorelease=true);
        
        virtual ~Modifier() { }
        
        /**
         * Modifer update function
         * 
         * @param The owner widget of this modifier
         **/
        virtual void update(Widget* widget) = 0;
        
        /**
         * Modifer release function
         **/
        virtual void release();
        
        /**
         * Set the flag that shows the modifier has finished it's task
         * During the next logic loop 
         * The widget would be released if AutoRelease flag is true
         * You can also override the release function by yourself
         *
         * @param The Finished flag
         * @see release
         * @see isFinished
         */
        virtual void setFinish(bool flag);
		
        bool isFinished() const;
        
        /**
         * Set the AutoRelease flag
         * When flag is true, the widget will release itself when 
         * the Finished flag is true during the next logic loop
         * 
         * @param The AutoRelease flag
         * @see release
         * @see setFinished
         **/
        virtual void setAutoRelease(bool flag);
        
        bool isAutoRelease() const;
        
        /**
         * Set the owner widget of this modifier
         * Notice when you use a modifier in multiple widgets
         * The owner would represent the last widget which added the modifier
         **/
        virtual void setOwner(Widget* widget);
        
        Widget* getOwner() const;
        
        /**
         * Would be called automatically when added to a widget
         **/
        virtual void onAdd(Widget* owner) { }

        /**
         * Would be called automatically when removed from a widget
         **/
        virtual void onRemove(Widget* owner) { }
        
        /**
         * Would be called automatically before release() is called
         **/
        virtual void onRelease(Widget* owner) { }
        
        /**
         * Get the name of the modifier
         * Should be unique so better be the class name
         * This name also would be used in ModifierFactory
         * Used in Widget::findModifierByName
         **/
        virtual std::string getName() const = 0;
        
	protected:
		bool mFinished;
        bool mAutoRelease;
        
        Widget* mOwner;
	};
		
} // namespace gcn
		
#endif // SORA_GCN_MODIFIER_BASE_H_