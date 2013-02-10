#ifndef DivaEditorWTextfield_H
#define DivaEditorWTextfield_H

#include "guichan/keylistener.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

#include <string>

namespace gcn
{
	/**
     * An implementation of a text field where a user can enter a line of text.
     */
    class GCN_CORE_DECLSPEC WTextField:
        public Widget,
        public MouseListener,
        public KeyListener
    {
    public:
        /**
         * Constructor.
         */
        WTextField();

        /**
         * Constructor. The text field will be automatically resized
         * to fit the text.
         *
         * @param text The default text of the text field.
         */
        WTextField(const std::wstring& text);

        /**
         * Sets the text of the text field.
         *
         * @param text The text of the text field.
         * @see getText
         */
        void setText(const std::wstring& text);

        /**
         * Gets the text of the text field.
         *
         * @return The text of the text field.
         * @see setText
         */
        const std::wstring& getText() const;

        /**
         * Adjusts the size of the text field to fit the text.
         */
        void adjustSize();

        /**
         * Adjusts the height of the text field to fit caption.
         */
        void adjustHeight();

        /**
         * Sets the caret position. As there is only one line of text
         * in a text field the position is the caret's x coordinate.
         *
         * @param position The caret position.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the caret position. As there is only one line of text
         * in a text field the position is the caret's x coordinate.
         *
         * @return The caret position.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;
		
		/**
		 *	Enable numeric input mode
		 *  When enabled, only numeric input would available in the field
		 *
		 **/
		virtual void enableNumericMode(bool flag);

		virtual void setPasswordMode(bool flag);

		virtual void setPasswordChar(wchar_t ch);
		
		virtual bool isNumericModeEnabled() const;
		
		/**
		 * Only available in numeric mode
		 **/
		virtual int getNumber() const;
		virtual float getFloat();
		virtual double getDouble();
		
        // Inherited from Widget
        virtual void fontChanged();

        virtual void draw(Graphics* graphics);


        // Inherited from MouseListener
        virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);
        

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);

    protected:
        /**
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics the Graphics object to draw with.
         * @param x the caret's x-position.
         */
        virtual void drawCaret(Graphics* graphics, int x);

        /**
         * Scrolls the text horizontally so that the caret shows if needed.
         * The method is used any time a user types in the text field so the
         * caret always will be shown.
         */
        void fixScroll();

        /**
         * Holds the text of the text box.
         */
        std::wstring mText;
		
		wchar_t password_char;

		bool mNumericMode;

		bool isPasswordMode;

        /**
         * Holds the caret position.
         */
        unsigned int mCaretPosition;
		unsigned int mCaretPositionFlashCount;
		static const unsigned int mCaretPositionFlashMax = 30;

        /**
         * Holds the amount scrolled in x. If a user types more characters than
         * the text field can display, due to the text field being to small, the
         * text needs to scroll in order to show the last type character.
         */
        int mXScroll;
    };
}






#endif