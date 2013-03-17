#ifndef WListBox_H
#define WListBox_H

#include <list>

#include "divaeditor/Widgets/WListmodel.h"

#include "guichan/keylistener.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"


namespace gcn
{
    class SelectionListener;

    class GCN_CORE_DECLSPEC WListBox :
        public Widget,
        public MouseListener,
        public KeyListener
    {
    public:
        /**
         * Constructor.
         */
        WListBox();

        WListBox(WListModel *listModel);

        virtual ~WListBox() { }

        int getSelected() const;
		WListModel *getListModel();

        void setSelected(int selected, bool trigger = true);
        void setListModel(WListModel *listModel);

        void adjustSize();

        bool isWrappingEnabled() const;

        void setWrappingEnabled(bool wrappingEnabled);

        void addSelectionListener(SelectionListener* selectionListener);

        void removeSelectionListener(SelectionListener* selectionListener);

        virtual unsigned int getRowHeight() const;


        // Inherited from Widget

        virtual void draw(Graphics* graphics);

        virtual void logic();


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

		/**
         * The list model to use.
         */
        WListModel *mListModel;

    protected:
        /**
         * Distributes a value changed event to all selection listeners
         * of the list box.
         *
         * @since 0.8.0
         */
        void distributeValueChangedEvent();

        /**
         * The selected item as an index in the list model.
         */
        int mSelected;

        

        /**
         * True if wrapping is enabled, false otherwise.
         */
        bool mWrappingEnabled;

        typedef std::list<SelectionListener*> SelectionListenerList;

        SelectionListenerList mSelectionListeners;

        typedef SelectionListenerList::iterator SelectionListenerIterator;
    };
}

#endif // end GCN_LISTBOX_HPP
