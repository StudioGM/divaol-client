#ifndef GCN_SORAIMAGEFONT_HPP
#define GCN_SORAIMAGEFONT_HPP

#include <string>

#include "guichan/font.hpp"
#include "guichan/platform.hpp"

#include "SoraCore.h"

namespace gcn
{
    class Graphics;

    class GCN_EXTENSION_DECLSPEC SoraGUIFont : public Font
    {
    public:
		SoraGUIFont(const std::wstring& lpsFontName, int nFaceSize);
        
		/**
         * Destructor.
         */
        ~SoraGUIFont();
        virtual sora::SoraFont *getFont() const;

        /**
         * Gets the width from a character.
         *
         * @param character the character to get the width from.
         */
        int getWidth(char character) const;


        //Inherited from Font

        int getWidth(const std::string &text) const;

        int getHeight() const;

        int getStringIndexAt(const std::string &text, int x) const;

        void drawString(Graphics *graphics, const std::string &text, int x, int y);

    protected:
        //hgeFont *mHGEFont;
		sora::SoraCore* sora;
		sora::SoraFont *mSoraFont;
		
    };
}

#endif // end GCN_HGEIMAGEFONT_HPP
