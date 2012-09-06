/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widget.hpp"

#include "guichan/actionevent.hpp"
#include "guichan/actionlistener.hpp"
#include "guichan/basiccontainer.hpp"
#include "guichan/deathlistener.hpp"
#include "guichan/defaultfont.hpp"
#include "guichan/event.hpp"
#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/widgetlistener.hpp"
#include "guichan/birthListener.h"
#include "guichan/style.hpp"
#include "guichan/modifier.hpp"
#include "guichan/style.hpp"
#include "guichan/animation.hpp"
#include "guichan/widgetFactory.hpp"

#include <assert.h>
#include <algorithm>

namespace gcn
{
    Font* Widget::mGlobalFont = NULL;
    Style* Widget::mGlobalStyle = NULL;
    
    DefaultFont Widget::mDefaultFont;
    std::list<Widget*> Widget::mWidgets;

    Widget::Widget()
            : mFocusHandler(NULL),
              mInternalFocusHandler(NULL),
              mParent(NULL),
              mFrameSize(0),
              mFocusable(false),
              mVisible(true),
              mTabIn(true),
              mTabOut(true),
              mEnabled(true),
              mCurrentFont(NULL),
              mStyle(Style::DefaultStyle()),
              mAlpha(255)
    {
        mWidgets.push_back(this);
        
        mForegroundColor = mStyle->getForegroundColor();
        mBackgroundColor = mStyle->getBackgroundColor();
        mBaseColor = mStyle->getBaseColor();
        mSelectionColor = mStyle->getSelectionColor();
    }

    Widget::~Widget()
    {
        DeathListenerIterator iter;

        for (iter = mDeathListeners.begin(); iter != mDeathListeners.end(); ++iter)
        {
            Event event(this);
            (*iter)->death(event);
        }
		
		ModifierIterator itModifier = mModifiers.begin();
		while(itModifier != mModifiers.end()) {
			if((*itModifier)->isAutoRelease())
                (*itModifier)->release();
			
			++itModifier;
		}

        _setFocusHandler(NULL);

        mWidgets.remove(this);
    }

    void Widget::drawFrame(Graphics* graphics)
    {
        Color faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        int width = getWidth() + getFrameSize() * 2 - 1;
        int height = getHeight() + getFrameSize() * 2 - 1;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        unsigned int i;
        for (i = 0; i < getFrameSize(); ++i)
        {
            graphics->setColor(shadowColor);
            graphics->drawLine(i,i, width - i, i);
            graphics->drawLine(i,i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i,i + 1, width - i, height - i);
            graphics->drawLine(i,height - i, width - i - 1, height - i);
        }
    }

	bool Widget::checkIsEnabled()
	{
		Widget* now = this;
		while (now)
		{
			if (!now->isEnabled())
				return false;
			now = now->getParent();
		}
		return true;
	}

    void Widget::_setParent(Widget* parent)
    {
        mParent = parent;
    }

    Widget* Widget::getParent() const
    {
        return mParent;
    }

    void Widget::setWidth(int width)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;

        setDimension(newDimension);
    }

    int Widget::getWidth() const
    {
        return mDimension.width;
    }

    void Widget::setHeight(int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.height = height;

        setDimension(newDimension);
    }

    int Widget::getHeight() const
    {
        return mDimension.height;
    }

    void Widget::setX(int x)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;

        setDimension(newDimension);
    }

    int Widget::getX() const
    {
        return mDimension.x;
    }

    void Widget::setY(int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.y = y;

        setDimension(newDimension);
    }

    int Widget::getY() const
    {
        return mDimension.y;
    }

    void Widget::setPosition(int x, int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;
        newDimension.y = y;
        
        setDimension(newDimension);
    }

    void Widget::setDimension(const Rectangle& dimension)
    { 
        Rectangle oldDimension = mDimension;
        mDimension = dimension;

        if (mDimension.width != oldDimension.width
            || mDimension.height != oldDimension.height)
        {
            distributeResizedEvent();
        }

        if (mDimension.x != oldDimension.x
            || mDimension.y != oldDimension.y)
        {
            distributeMovedEvent();
        }
    }

    void Widget::setFrameSize(unsigned int frameSize)
    {
        mFrameSize = frameSize;
    }

    unsigned int Widget::getFrameSize() const
    {
        return mFrameSize;
    }

    const Rectangle& Widget::getDimension() const
    {
        return mDimension;
    }

    const std::string& Widget::getActionEventId() const
    {
        return mActionEventId;
    }

    void Widget::setActionEventId(const std::string& actionEventId)
    {
        mActionEventId = actionEventId;
    }

    bool Widget::isFocused() const
    {
        if (!mFocusHandler)
        {
            return false;
        }

        return (mFocusHandler->isFocused(this));
    }

    void Widget::setFocusable(bool focusable)
    {
        if (!focusable && isFocused())
        {
            mFocusHandler->focusNone();
        }

        mFocusable = focusable;
    }

    bool Widget::isFocusable() const
    {
        return mFocusable && isVisible() && isEnabled();
    }

    void Widget::requestFocus()
    {
        if (mFocusHandler == NULL)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");
        }

        if (isFocusable())
        {
            mFocusHandler->requestFocus(this);
        }
    }

    void Widget::requestMoveToTop()
    {
        if (mParent)
        {
            mParent->moveToTop(this);
        }
    }

    void Widget::requestMoveToBottom()
    {
        if (mParent)
        {
            mParent->moveToBottom(this);
        }
    }

    void Widget::setVisible(bool visible)
    {
        if (!visible && isFocused())
        {
            mFocusHandler->focusNone();
        }
        
        if (visible)
        {
            distributeShownEvent();
        }
        else if(!visible)
        {
            distributeHiddenEvent();
        }

        mVisible = visible;
    }

    bool Widget::isVisible() const
    {
        if (getParent() == NULL)
        {
            return mVisible;
        }
        else
        {
            return mVisible && getParent()->isVisible();
        }
    }

    void Widget::setBaseColor(const Color& color)
    {
        mBaseColor = color;
    }

    const Color& Widget::getBaseColor() const
    {
        return mBaseColor;
    }

    void Widget::setForegroundColor(const Color& color)
    {
        mForegroundColor = color;
    }

    const Color& Widget::getForegroundColor() const
    {
        return mForegroundColor;
    }

    void Widget::setBackgroundColor(const Color& color)
    {
        mBackgroundColor = color;
    }

    const Color& Widget::getBackgroundColor() const
    {
        return mBackgroundColor;
    }

    void Widget::setSelectionColor(const Color& color)
    {
        mSelectionColor = color;
    }

    const Color& Widget::getSelectionColor() const
    {
        return mSelectionColor;
    }    
    
    void Widget::_setFocusHandler(FocusHandler* focusHandler)
    {
        if (mFocusHandler)
        {
            releaseModalFocus();
            mFocusHandler->remove(this);
        }

        if (focusHandler)
        {
            focusHandler->add(this);
        }

        mFocusHandler = focusHandler;
    }

    FocusHandler* Widget::_getFocusHandler()
    {
        return mFocusHandler;
    }

    void Widget::addActionListener(ActionListener* actionListener)
    {
        mActionListeners.push_back(actionListener);
    }

    void Widget::removeActionListener(ActionListener* actionListener)
    {
        mActionListeners.remove(actionListener);
    }

    void Widget::addDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.push_back(deathListener);
    }

    void Widget::removeDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.remove(deathListener);
    }

    void Widget::addKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Widget::removeKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Widget::addFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.push_back(focusListener);
    }

    void Widget::removeFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.remove(focusListener);
    }

    void Widget::addMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.push_back(mouseListener);
    }

    void Widget::removeMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.remove(mouseListener);
    }

    void Widget::addWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.push_back(widgetListener);
    }

    void Widget::removeWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.remove(widgetListener);
    }
    
    void Widget::addBirthListener(BirthListener* birthListener)
    {
        mBirthListeners.push_back(birthListener);
    }
    
    void Widget::removeBirthListener(BirthListener* birthListener)
    {
        mBirthListeners.remove(birthListener);
    }

    void Widget::getAbsolutePosition(int& x, int& y) const
    {
        if (getParent() == NULL)
        {
            x = mDimension.x;
            y = mDimension.y;
            return;
        }

        int parentX;
        int parentY;

        getParent()->getAbsolutePosition(parentX, parentY);

        x = parentX + mDimension.x + getParent()->getChildrenArea().x;
        y = parentY + mDimension.y + getParent()->getChildrenArea().y;
    }

    Font* Widget::getFont() const
    {
        if (mCurrentFont == NULL)
        {
            if (mGlobalFont == NULL)
            {
                return &mDefaultFont;
            }

            return mGlobalFont;
        }

        return mCurrentFont;
    }

    void Widget::setGlobalFont(Font* font)
    {
        mGlobalFont = font;

        std::list<Widget*>::iterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
        {
            if ((*iter)->mCurrentFont == NULL)
            {
                (*iter)->fontChanged();
            }
        }
    }

    void Widget::setFont(Font* font)
    {
        mCurrentFont = font;
        fontChanged();
    }

    bool Widget::widgetExists(const Widget* widget)
    {
        bool result = false;

        std::list<Widget*>::iterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
        {
            if (*iter == widget)
            {
                return true;
            }
        }

        return result;
    }

    bool Widget::isTabInEnabled() const
    {
        return mTabIn;
    }

    void Widget::setTabInEnabled(bool enabled)
    {
        mTabIn = enabled;
    }

    bool Widget::isTabOutEnabled() const
    {
        return mTabOut;
    }

    void Widget::setTabOutEnabled(bool enabled)
    {
        mTabOut = enabled;
    }

    void Widget::setSize(int width, int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.width = width;
        newDimension.height = height;

        setDimension(newDimension);
    }

    void Widget::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool Widget::isEnabled() const
    {
        return mEnabled && isVisible();
    }

    void Widget::requestModalFocus()
    {
        if (mFocusHandler == NULL)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");
        }

        mFocusHandler->requestModalFocus(this);
    }

    void Widget::requestModalMouseInputFocus()
    {
        if (mFocusHandler == NULL)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");
        }

        mFocusHandler->requestModalMouseInputFocus(this);
    }

    void Widget::releaseModalFocus()
    {
        if (mFocusHandler == NULL)
        {
            return;
        }

        mFocusHandler->releaseModalFocus(this);
    }

    void Widget::releaseModalMouseInputFocus()
    {
        if (mFocusHandler == NULL)
        {
            return;
        }

        mFocusHandler->releaseModalMouseInputFocus(this);
    }

    bool Widget::isModalFocused() const
    {
        if (mFocusHandler == NULL)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");
        }

        if (getParent() != NULL)
        {
            return (mFocusHandler->getModalFocused() == this) 
                || getParent()->isModalFocused();
        }

        return mFocusHandler->getModalFocused() == this;
    }

    bool Widget::isModalMouseInputFocused() const
    {
        if (mFocusHandler == NULL)
        {
            throw GCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");
        }

        if (getParent() != NULL)
        {
            return (mFocusHandler->getModalMouseInputFocused() == this) 
                || getParent()->isModalMouseInputFocused();
        }

        return mFocusHandler->getModalMouseInputFocused() == this;
    }

    Widget *Widget::getWidgetAt(int x, int y)
    {
        return NULL;
    }

    const std::list<MouseListener*>& Widget::_getMouseListeners()
    {
        return mMouseListeners;
    }

    const std::list<KeyListener*>& Widget::_getKeyListeners()
    {
        return mKeyListeners;
    }

    const std::list<FocusListener*>& Widget::_getFocusListeners()
    {
        return mFocusListeners;
    }

    Rectangle Widget::getChildrenArea()
    {
        return Rectangle(0, 0, 0, 0);
    }

    FocusHandler* Widget::_getInternalFocusHandler()
    {
        return mInternalFocusHandler;
    }

    void Widget::setInternalFocusHandler(FocusHandler* focusHandler)
    {
        mInternalFocusHandler = focusHandler;
    }

    void Widget::setId(const std::string& id)
    {
        mId = id;
    }

    const std::string& Widget::getId()
    {
        return mId;
    }

    void Widget::distributeResizedEvent()
    {
        WidgetListenerIterator iter;

        Event event(this);
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            (*iter)->widgetResized(event);
        }
    }

    void Widget::distributeMovedEvent()
    {
        WidgetListenerIterator iter;

        Event event(this);
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            (*iter)->widgetMoved(event);
        }
    }

    void Widget::distributeHiddenEvent()
    {
        WidgetListenerIterator iter;

        Event event(this);
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            (*iter)->widgetHidden(event);
        }
    }

    void Widget::distributeActionEvent()
    {
        ActionListenerIterator iter;
        for (iter = mActionListeners.begin(); iter != mActionListeners.end(); ++iter)
        {
            ActionEvent actionEvent(this, mActionEventId);
            (*iter)->action(actionEvent);
        }
    }

    void Widget::distributeShownEvent()
    {
        WidgetListenerIterator iter;

        Event event(this);
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            (*iter)->widgetShown(event);
        }
    }
    
    void Widget::distributeBirthEvent() {
        BirthListenerIterator iter;
        
        Event event(this);
        for (iter = mBirthListeners.begin(); iter != mBirthListeners.end(); ++iter) {
            (*iter)->birth(event);
        }
    }

    void Widget::showPart(Rectangle rectangle)
    {
        if (mParent != NULL)
        {
            mParent->showWidgetPart(this, rectangle);
        }                
    }
	
	void Widget::logic() {
		updateModifierList();
	}
    
    void Widget::updateModifierList() {
		if(mModifiers.size() == 0) {
		
			return;
		}

        ModifierIterator iter = mModifiers.begin();
        ModifierIterator end = mModifiers.end();
        
        while(iter != end) {
            (*iter)->update(this);
            if((*iter)->isFinished()) {
                if((*iter)->isAutoRelease()) {
                    (*iter)->onRelease(this);
                    (*iter)->release();
                    iter = mModifiers.erase(iter);
                    continue;
                }
            }
			++iter;
        }
    }
	
	const std::list<Modifier*>& Widget::_getModifiers() {
		return mModifiers;
	}
    
    Modifier* Widget::findModifierByName(const std::string& name) const {
        ModifierList::const_iterator it = mModifiers.begin();
        while(it != mModifiers.end()) {
            if((*it)->getName() == name)
                return (*it);
            ++it;
        }
        return NULL;
    }
	
	void Widget::addModifier(Modifier* modifier) {
		assert(modifier);
        
        mModifiers.push_back(modifier);
        modifier->setOwner(this);
        modifier->onAdd(this);
	}
	
	void Widget::removeModifier()
	{
		while(mModifiers.size()>0)
			removeModifier((*mModifiers.begin()));
	}

	void Widget::removeModifier(Modifier* modifier) {
        assert(modifier);
        
        mModifiers.erase(std::remove(mModifiers.begin(), mModifiers.end(), modifier), mModifiers.end());
        modifier->onRemove(this);
        modifier->setOwner(NULL);

		if(!modifier->isAutoRelease())
		{
			modifier->onRelease(this);
            modifier->release();
		}
	}

	void Widget::removeModifierByName(const std::string& name)
	{
		Modifier *toRemove = findModifierByName(name);
		if(toRemove)
			removeModifier(toRemove);
	}

	
	void Widget::setAlpha(int alpha) {
		if(alpha < 0) alpha = 0; 
		else if(alpha > 255) alpha = 255;
		
		mForegroundColor.a = alpha;
		mBaseColor.a = alpha;
		mSelectionColor.a = alpha;
		mBackgroundColor.a = alpha;
        
        mAlpha = alpha;
	}
    
    int Widget::getAlpha() const {
        return mAlpha;
    }

    const std::list<BirthListener*>& Widget::_getBirthListeners() {
        return mBirthListeners;
    }
    
    void Widget::setStyle(Style* style) {
        mStyle = style;
    }
    
    Style* Widget::getStyle() const {
        if(mStyle == NULL)
            return mGlobalStyle;
        return mStyle;
    }
    
    void Widget::setGlobalStyle(Style* style) {
        mGlobalStyle = style;
    }
    
    Style* Widget::getGlobalStyle() {
        return mGlobalStyle;
    }
    
    void Widget::sendMessage(const std::string& mssg, const std::string& receiver) {
        if(receiver == getId() || receiver.empty())
            onMessage(Message(this, this, mssg));
    }
    
    void Widget::sendMessage(const Message& mssg) {
        if(mssg.getReceiver() == this || mssg.getReceiver() == NULL)
            onMessage(mssg);
    }
    
    void Widget::onMessage(const Message& mssg) {
       
    }
    
    void Widget::animationBegan(Animation* animation) {
        setEnabled(false);
    }
    
    void Widget::animationEnded(Animation* aniamtion) {
        setEnabled(true);
    }
    
    Message::Message(Widget* sender,
                     Widget* receiver,
                     const std::string& mssg):
    mSender(sender),
    mReceiver(receiver),
    mMessage(mssg) {
        
    }
    
    Widget* Message::getSender() const {
        return mSender;
    }
    
    Widget* Message::getReceiver() const {
        return mReceiver;
    }
    
    const std::string& Message::getMessage() const {
        return mMessage;
    }
    
    Widget* Widget::createWidget(const std::string& key) {
        return CreateWidget(key);
    }
    
}
