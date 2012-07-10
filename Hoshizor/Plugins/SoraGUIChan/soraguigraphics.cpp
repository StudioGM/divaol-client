#include "soraguigraphics.hpp"
#include "soraguiimage.hpp"

#include "guichan/cliprectangle.hpp"
#include "guichan/exception.hpp"
#include "guichan/rectangle.hpp"

#include "guichan/font.hpp"

#include "SoraSprite.h"

namespace gcn
{
    SoraGUIGraphics::SoraGUIGraphics():
    mHardwareColor(0),
    mClipNull(false),
    mGlobalTransparency(1.f) {
        sora = sora::SoraCore::Instance();

        mHardwareColor = 0;
    }

    SoraGUIGraphics::~SoraGUIGraphics() {
    }

    void SoraGUIGraphics::_beginDraw() {
        pushClipArea(Rectangle(0,
                               0,
                               sora->getScreenWidth(),
                               sora->getScreenHeight()));
    }

    void SoraGUIGraphics::_endDraw() {
        // pop the clip area pushed in _beginDraw
        popClipArea();
    }

    bool SoraGUIGraphics::pushClipArea(Rectangle area) {
        bool result = Graphics::pushClipArea(area);

        const ClipRectangle top = mClipStack.top();
        if (top.width == 0 || top.height == 0) {
            mClipNull = true;
        }
        else {
            mClipNull = false;

            sora->setTransform(0,
                               0,
                               (float)top.xOffset,
							   (float)top.yOffset,
							    0.f,
								1.f,
								1.f);
            
            sora->setClipping(top.x,
                              top.y,
                              top.width,
                              top.height);
        }
        return result;
    }

    void SoraGUIGraphics::popClipArea() {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            sora->setClipping();
            return;
        }
        else {
            const ClipRectangle top = mClipStack.top();

            if (top.width == 0 || top.height == 0) {
                mClipNull = true;
            }
            else {
                mClipNull = false;
                
                sora->setTransform(0,
                                   0,
                                   (float)top.xOffset,
								   (float)top.yOffset,
								   0.f,
								   1.f,
								   1.f);
                
                sora->setClipping(top.x,
								  top.y,
								  top.width,
								  top.height);
            }
        }
    }

    void SoraGUIGraphics::drawImage(const Image *image,
									int srcX,
									int srcY,
									int dstX,
									int dstY,
									int width,
									int height) {
        if (mClipNull) {
            return;
        }

        const SoraGUIImage *soraImage = static_cast<const SoraGUIImage*>(image);

        if (soraImage == NULL) {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an SoraImage.");
        }

       /* ClipRectangle const top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;*/

        soraImage->getSprite()->setTextureRect((float)srcX, (float)srcY, (float)width, (float)height);
        soraImage->getSprite()->setColor(mHardwareColor);
        soraImage->getSprite()->render((float)dstX, (float)dstY);
    }

    void SoraGUIGraphics::drawImage(const Image *image, int dstX, int dstY) {
        if (mClipNull) {
            return;
        }

        drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    }

    void SoraGUIGraphics::drawPoint(int x, int y) {
        if (mClipNull) {
            return;
        }

        sora->renderLine((float)x, (float)y, (float)x + 1.f, (float)y, mHardwareColor, GUI_Z);
    }

    void SoraGUIGraphics::drawLine(int x1, int y1, int x2, int y2) {
        if (mClipNull) {
            return;
        }

       
        if (y1 == y2 || x1 == x2) {
            x2++;
            y2++;
        }
        // If it's not a vertical or horizontal line it gets a little bit
        // trickier.
        else {
            // If y2 is greater than y1 then we know y2 will be omitted as
            // it will be a part of the last pixel coordinate.
            if (y2 > y1) {
                y2++;
            }
            // Else will y1 be omitted.
            else {
                y1++;
            }
            // The same thing applies for the x coordinates. If x2 is greater
            // than x1 then we know x2 will be omitted as it will be a part of
            // the last pixel coordinate.
            if (x2 > x1) {
                x2++;
            }
            // Else will x1 be omitted.
            else {
                x1++;
            }
        }
        
		sora->renderLine((float)x1, (float)y1, (float)x2, (float)y2, mHardwareColor, GUI_Z);
    }

    void SoraGUIGraphics::drawRectangle(const Rectangle &rectangle) {
        if (mClipNull) {
            return;
        }

        float x1 = (float)rectangle.x;
        float y1 = (float)rectangle.y;
        float x2 = (float)rectangle.x + rectangle.width;
        float y2 = (float)rectangle.y + rectangle.height;
        
        sora->renderLine(x1, y1, x2, y1+1.f, mHardwareColor, GUI_Z);
        sora->renderLine(x2, y1, x2+1.f, y2, mHardwareColor, GUI_Z);
        sora->renderLine(x2, y2, x1, y2+1.f, mHardwareColor, GUI_Z);
        sora->renderLine(x1, y2, x1+1.f, y1, mHardwareColor, GUI_Z);
    }

    void SoraGUIGraphics::fillRectangle(const Rectangle &rectangle) {
        if (mClipNull) {
            return;
        }

        float x1 = (float)rectangle.x;
        float y1 = (float)rectangle.y;
        float x2 = (float)rectangle.x + rectangle.width;
        float y2 = (float)rectangle.y + rectangle.height;

		sora::SoraQuad quad;

        quad.tex = NULL;

        quad.v[0].x   = x1;
        quad.v[0].y   = y1;
        quad.v[0].col = mHardwareColor;

        quad.v[1].x   = x2;
        quad.v[1].y   = y1;
        quad.v[1].col = mHardwareColor;

        quad.v[2].x   = x2;
        quad.v[2].y   = y2;
        quad.v[2].col = mHardwareColor;

        quad.v[3].x   = x1;
        quad.v[3].y   = y2;
        quad.v[3].col = mHardwareColor;
        
        printf("%d\n", CGETA(mHardwareColor));

        int i;
        for (i = 0; i < 4; ++i)	{
            quad.v[i].z = GUI_Z;
        }

        quad.blend = BLEND_DEFAULT;

		sora->renderQuad(quad);
    }

    void SoraGUIGraphics::setColor(const Color &color) {
        mColor = color;
        mColor.a *= mGlobalTransparency;
        
        mHardwareColor = CARGB(color.a * mGlobalTransparency, color.r, color.g, color.b);
    }

    const Color& SoraGUIGraphics::getColor() const {
        return  mColor;
    }
    
    void SoraGUIGraphics::setGlobalTransparency(float a) {
        mGlobalTransparency = a;
    }
    
    float SoraGUIGraphics::getGlobalTransparency() const {
        return mGlobalTransparency;
    }
}
