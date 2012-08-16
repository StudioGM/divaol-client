#include "divaeditor/DivaEditorScene/DivaEditorWListBox.h"

#include "guichan/basiccontainer.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/listmodel.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/selectionlistener.hpp"

namespace gcn
{
    WListBox::WListBox()
        : mSelected(-1),
          mListModel(NULL),
          mWrappingEnabled(false)
    {
        setWidth(100);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    WListBox::WListBox(WListModel *listModel)
        : mSelected(-1),
          mWrappingEnabled(false)
    {
        setWidth(100);
        setListModel(listModel);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    void WListBox::draw(Graphics* graphics)
    {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

        if (mListModel == NULL)
        {
            return;
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
         
        // Check the current clip area so we don't draw unnecessary items
        // that are not visible.
        const ClipRectangle currentClipArea = graphics->getCurrentClipArea();
        int rowHeight = getRowHeight();
        
		// Calculate the number of rows to draw by checking the clip area.
		// The addition of two makes covers a partial visible row at the top
		// and a partial visible row at the bottom.
		int numberOfRows = currentClipArea.height / rowHeight + 2;

        if (numberOfRows > mListModel->getNumberOfElements())
        {
            numberOfRows = mListModel->getNumberOfElements();
        }

		// Calculate which row to start drawing. If the list box 
		// has a negative y coordinate value we should check if
		// we should drop rows in the begining of the list as
		// they might not be visible. A negative y value is very
		// common if the list box for instance resides in a scroll
		// area and the user has scrolled the list box downwards.
		int startRow;    	
		if (getY() < 0)
		{
			startRow = -1 * (getY() / rowHeight);
		}
		else
		{
			startRow = 0;
		}

		int i;
		// The y coordinate where we start to draw the text is
		// simply the y coordinate multiplied with the font height.
		int y = rowHeight * startRow;
        for (i = startRow; i < startRow + numberOfRows; ++i)
        {
            if (i == mSelected)
            {
                graphics->setColor(getSelectionColor());
                graphics->fillRectangle(Rectangle(0, y, getWidth(), rowHeight));
                graphics->setColor(getForegroundColor());
            }
			
			// If the row height is greater than the font height we
			// draw the text with a center vertical alignment.
			if (rowHeight > getFont()->getHeight())
			{
				graphics->drawTextW(mListModel->getElementAt(i), 1, y + rowHeight / 2 - getFont()->getHeight() / 2);
			}
			else
			{
				graphics->drawTextW(mListModel->getElementAt(i), 1, y);
			}

            y += rowHeight;
        }
    }

    void WListBox::logic()
    {
		Widget::logic();
        adjustSize();
    }

    int WListBox::getSelected() const
    {
        return mSelected;
    }

    void WListBox::setSelected(int selected)
    {
        if (mListModel == NULL)
        {
            mSelected = -1;
        }
        else
        {
            if (selected < 0)
            {
                mSelected = -1;
            }
            else if (selected >= mListModel->getNumberOfElements())
            {
                mSelected = mListModel->getNumberOfElements() - 1;
            }
            else
            {
                mSelected = selected;
            }
        }
        
        Rectangle scroll;

        if (mSelected < 0)
        {
            scroll.y = 0;
        }
        else
        {
            scroll.y = getRowHeight() * mSelected;
        }

        scroll.height = getRowHeight();
        showPart(scroll);

        distributeValueChangedEvent();
    }

    void WListBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::ENTER || key.getValue() == Key::SPACE)
        {
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (key.getValue() == Key::UP)
        {
            setSelected(mSelected - 1);

            if (mSelected == -1)
            {
                if (mWrappingEnabled)
                {
                    setSelected(getListModel()->getNumberOfElements() - 1);
                }
                else
                {
                    setSelected(0);
                }
            }
            
            keyEvent.consume();
        }
        else if (key.getValue() == Key::DOWN)
        {
            if (mWrappingEnabled
                && getSelected() == getListModel()->getNumberOfElements() - 1)
            {
                setSelected(0);
            }
            else
            {
                setSelected(getSelected() + 1);
            }
            
            keyEvent.consume();
        }
        else if (key.getValue() == Key::HOME)
        {
            setSelected(0);
            keyEvent.consume();
        }
        else if (key.getValue() == Key::END)
        {
            setSelected(getListModel()->getNumberOfElements() - 1);
            keyEvent.consume();
        }
    }

    void WListBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            setSelected(mouseEvent.getY() / getRowHeight());
            distributeActionEvent();
        }
    }

    void WListBox::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            if (getSelected() > 0 )
            {
                setSelected(getSelected() - 1);
            }

            mouseEvent.consume();
        }
    }

    void WListBox::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            setSelected(getSelected() + 1);

            mouseEvent.consume();
        }
    }

    void WListBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void WListBox::setListModel(WListModel *listModel)
    {
        mSelected = -1;
        mListModel = listModel;
        adjustSize();
    }

    WListModel* WListBox::getListModel()
    {
        return mListModel;
    }

    void WListBox::adjustSize()
    {
        if (mListModel != NULL)
        {
            setHeight(getRowHeight() * mListModel->getNumberOfElements());
        }
    }

    bool WListBox::isWrappingEnabled() const
    {
        return mWrappingEnabled;
    }

    void WListBox::setWrappingEnabled(bool wrappingEnabled)
    {
        mWrappingEnabled = wrappingEnabled;
    }
        
    void WListBox::addSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }
   
    void WListBox::removeSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void WListBox::distributeValueChangedEvent()
    {
        SelectionListenerIterator iter;

        for (iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); ++iter)
        {
            SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }

	unsigned int WListBox::getRowHeight() const
	{
		return getFont()->getHeight();
	}
}
