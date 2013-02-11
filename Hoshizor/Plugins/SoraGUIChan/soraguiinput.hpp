#ifndef GCN_SORAINPUT_HPP
#define GCN_SORAINPUT_HPP

#include <queue>


#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/platform.hpp"

#include "SoraCore.h"
#include "SoraKeyInfo.h"

namespace gcn
{
    /**
     * HGE implementation of Input.
     *
     * @author Kevin Lynx
     * @since 0.6.1
     */
    class GCN_EXTENSION_DECLSPEC SoraGUIInput : public Input
    {
    public:

        /**
         *  Constructor.
         */
        SoraGUIInput();

        /**
         *  Destructor.
         */
        virtual ~SoraGUIInput();


        //Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual bool isMouseQueueEmpty();
    
        virtual KeyInput dequeueKeyInput();
    
        virtual MouseInput dequeueMouseInput();

        virtual void _pollInput();

		void onIMEKey( int value );

    protected:
        /**
         * Handles key input. The function is called by _pollInput.
         *
         * @param ki the hge input event to handle.
         */
        void pollKeyInput(sora::SoraKeyEvent &ki);
    
        /**
         * Handles mouse input. The function is called by _pollInput.
         *
         * This function directly deal with the mouse input , and it avoid 
         * the odd things in HGE.
         */
        void pollMouseInput();

        /**
         * Converts the keycode and characters to a Key object.
         *
         * @param key The key to convert.
         * @param chr The character to convert.
         */
        Key convertToKey(int key, int chr);
    
        /**
         * Checks if a keyCode is numeric.
         *
         * @param keyCode The key code to check.
         */
        bool isNumericPad(int keyCode);

    protected:
		sora::SoraCore* sora;

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        float mMouseX;
        float mMouseY;

        bool mLeftMouseButtonDown;
        bool mRightMouseButtonDown;
        bool mMiddleMouseButtonDown;

		// ak add
	public:
		static void setSInput(SoraGUIInput* in);
    };
}

#endif // end GCN_HGEINPUT_HPP
