//
//  SoraGraphicAlgorithm.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/19/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGraphicAlgorithm.h"

#include "SoraException.h"

#include "SoraSprite.h"
#include "SoraTexture.h"

#include "SoraCore.h"

namespace sora {

    namespace cpu_graphic_effect {
        
        int gaussBlur(uint32 *data, int width, int height, double sigma, int radius) {    
            double *gaussMatrix, gaussSum = 0.0, _2sigma2 = 2 * sigma * sigma;    
            int x, y, xx, yy, xxx, yyy;    
            double *pdbl, a, r, g, b, d;    
            unsigned char *bbb, *pout, *poutb, *pczdata = (unsigned char*)data;    
            pout = poutb = (unsigned char *)malloc(width * height * 4);    
            if (!pout) return 0;    
            gaussMatrix = pdbl = (double *)malloc((radius * 2 + 1) * (radius * 2 + 1) * sizeof(double));    
            if (!gaussMatrix) {    
                free(pout);    
                return 0;    
            }    
            for (y = -radius; y <= radius; y++) {    
                for (x = -radius; x <= radius; x++) {    
                    a = exp(-(double)(x * x + y * y) / _2sigma2);    
                    *pdbl++ = a;    
                    gaussSum += a;    
                }    
            }    
            pdbl = gaussMatrix;    
            for (y = -radius; y <= radius; y++) {    
                for (x = -radius; x <= radius; x++) {    
                    *pdbl++ /= gaussSum;    
                }    
            }    
            for (y = 0; y < height; y++) {    
                for (x = 0; x < width; x++) {    
                    a = r = g = b = 0.0;    
                    pdbl = gaussMatrix;    
                    for (yy = -radius; yy <= radius; yy++) {    
                        yyy = y + yy;    
                        if (yyy >= 0 && yyy < height) {    
                            for (xx = -radius; xx <= radius; xx++) {    
                                xxx = x + xx;    
                                if (xxx >= 0 && xxx < width) {
                                    bbb = &pczdata[(xxx + yyy * width)*4];    
                                    d = *pdbl;    
                                    b += d * bbb[0];    
                                    g += d * bbb[1];    
                                    r += d * bbb[2];    
                                    a += d * bbb[3];    
                                }    
                                pdbl++;    
                            }    
                        } else {    
                            pdbl += (radius * 2 + 1);    
                        }    
                    }    
                    *pout++ = (unsigned char)b;    
                    *pout++ = (unsigned char)g;    
                    *pout++ = (unsigned char)r;    
                    *pout++ = (unsigned char)a;    
                }    
            }    
            memcpy(data, poutb, width * height * 4);    
            free(gaussMatrix);    
            free(poutb);    
            return 1;    
        }  
        
        int gray(uint32 *data, int width, int height) {
            unsigned char a, r, g, b;    
            unsigned char* pout, *poutb;   
            unsigned char* pczdata = (unsigned char*)data;
            pout = poutb =(unsigned char *)malloc(width * height * 4);    
            if (!pout) return 0;   
            
            int y, x;
            double gray;
            for (y = 0; y < height; y++) {    
                for (x = 0; x < width; x++) {    
                    a = r = g = b = 0;    			
                    b = *pczdata++;
                    g = *pczdata++;
                    r = *pczdata++;
                    a = *pczdata++;
                    
                    gray = r*0.3+g*0.59+b*0.11;
                    
                    *pout++ = (unsigned char)gray;    
                    *pout++ = (unsigned char)gray;    
                    *pout++ = (unsigned char)gray;    
                    *pout++ = (unsigned char)a;    
                }    
            }    
            memcpy(data, poutb, width * height * 4);    
            free(poutb);    
            return 1; 
        }
        
        int reverse(uint32* data, int width, int height) {
            unsigned char a, r, g, b;    
            unsigned char* pout, *poutb, *pczdata = (unsigned char*)data;    
            pout = poutb =(unsigned char *)malloc(width * height * 4);    
            if (!pout) return 0;   
            
            int y, x;
            for (y = 0; y < height; y++) {    
                for (x = 0; x < width; x++) {    
                    a = r = g = b = 0;    
                    
                    b = *pczdata++;
                    g = *pczdata++;
                    r = *pczdata++;
                    a = *pczdata++;
                    
                    *pout++ = (unsigned char)(255-b);    
                    *pout++ = (unsigned char)(255-g);    
                    *pout++ = (unsigned char)(255-r);    
                    *pout++ = (unsigned char)a;    
                }    
            }    
            memcpy(data, poutb, width * height * 4);    
            free(poutb);    
            return 1; 
        }
        
        int alphaMix(uint32* data1, uint32* data2, int width, int height, int width2, int height2, double a, int posx, int posy) {
            double sa, sr, sg, sb, da, dr, dg, db;    
            unsigned char* pout, *poutb, *pczdata1 = (unsigned char*)data1, *pczdata2 = (unsigned char*)data2;    
            pout = poutb =(unsigned char *)malloc(width * height * 4);    
            if (!pout) return 0;   
            
            int y, x;
            int rh, rw;
            
            width2+posx>=width?rw=width:rw=width2+posx;
            height2+posy>=height?rh=height:rh=height2+posx;
            
            for (y = 0; y < height; y++) {    
                for (x = 0; x < width; x++) {    
                    if(x >= posx && y >= posy && x < rw && y < rh) {
                        sb = *pczdata1++;
                        sg = *pczdata1++;
                        sr = *pczdata1++;
                        sa = *pczdata1++;
                        
                        db = *pczdata2++;
                        dg = *pczdata2++;
                        dr = *pczdata2++;
                        da = *pczdata2++;
                        
                        if(da != 0.f) {
                            *pout++ = (unsigned char)(sb*a+(1-a)*db);    
                            *pout++ = (unsigned char)(sg*a+(1-a)*dg);    
                            *pout++ = (unsigned char)(sr*a+(1-a)*dr);    
                            *pout++ = (unsigned char)sa;
                        } else {
                            *pout++ = (unsigned char)sb;    
                            *pout++ = (unsigned char)sg;    
                            *pout++ = (unsigned char)sr;    
                            *pout++ = (unsigned char)sa;
                        }
                    } else {
                        *pout++ = (unsigned char)*pczdata1++;    
                        *pout++ = (unsigned char)*pczdata1++;    
                        *pout++ = (unsigned char)*pczdata1++;    
                        *pout++ = (unsigned char)*pczdata1++;
                    }
                }    
            }    
            
            memcpy(data1, poutb, width * height * 4);    
            free(poutb);    
            return 1; 
        }
        
        // faster and better average algorithm
#define AVERAGE(a, b)   ( (((a) & 0xfefefeffUL) + ((b) & 0xfefefeffUL)) >> 1 )
        
        inline void scaleLineAvg(uint32 *Target, uint32 *Source, int SrcWidth, int TgtWidth) {
            /* N.B. because of several simplifications of the algorithm,
             *      the zoom range is restricted between 0.5 and 2. That
             *      is: TgtWidth must be >= SrcWidth/2 and <= 2*SrcWidth.
             */
            int NumPixels = TgtWidth;
            int Mid = TgtWidth / 2;
            int E = 0;
            uint32 p;
            
            if (TgtWidth > SrcWidth)
                NumPixels--;
            while (NumPixels-- > 0) {
                p = *Source;
                if (E >= Mid)
                    p = AVERAGE(p, *(Source+1));
                *Target++ = p;
                E += SrcWidth;
                if (E >= TgtWidth) {
                    E -= TgtWidth;
                    Source++;
                } /* if */
            } /* while */
            if (TgtWidth > SrcWidth)
                *Target = *Source;
        }
        
        void ScaleLine(uint32 *Target, uint32 *Source, int SrcWidth, int TgtWidth)
        {
            int NumPixels = TgtWidth;
            int IntPart = SrcWidth / TgtWidth;
            int FractPart = SrcWidth % TgtWidth;
            int E = 0;
            
            while (NumPixels-- > 0) {
                *Target++ = *Source;
                Source += IntPart;
                E += FractPart;
                if (E >= TgtWidth) {
                    E -= TgtWidth;
                    Source++;
                } /* if */
            } /* while */
        }
        
        int bresenhamScale(uint32* Target, uint32* Source, int32 SrcWidth, int32 SrcHeight, int32 TgtWidth, int32 TgtHeight) {
            int NumPixels = TgtHeight;
            int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
            int FractPart = SrcHeight % TgtHeight;
            int E = 0;
            uint32 *PrevSource = NULL;
            
            while (NumPixels-- > 0) {
                if (Source == PrevSource) {
                    memcpy(Target, Target-TgtWidth, TgtWidth*sizeof(*Target));
                } else {
                    ScaleLine(Target, Source, SrcWidth, TgtWidth);
                    PrevSource = Source;
                } /* if */
                Target += TgtWidth;
                Source += IntPart;
                E += FractPart;
                if (E >= TgtHeight) {
                    E -= TgtHeight;
                    Source += SrcWidth;
                } /* if */
            } /* while */
            return 1;
        }
        
        
    }
    
    SoraSprite* SoraGraphicAlgorithm::gaussBlurNew(SoraSprite* in, double sigma, int radius) {
        SoraTexture* tex = (SoraTexture*)in->getTexture();
        
        SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
        uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        uint32* data = in->getPixelData();
        memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        in->unlockPixelData();
        if(!::sora::cpu_graphic_effect::gaussBlur(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight, sigma, radius))
            THROW_SORA_EXCEPTION(RuntimeException, "failed gauss blur sprite");
        SoraSprite* out = new SoraSprite((SoraTextureHandle)outtex);
        uint32* odata = out->getPixelData();
        memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        free(outputData);
        out->unlockPixelData();
        if(!out)
            THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
        
        return out;
    }
    
    void SoraGraphicAlgorithm::gaussBlur(SoraSprite* in, double sigma, int radius) {
        uint32* data = in->getPixelData();
        if(!::sora::cpu_graphic_effect::gaussBlur(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight(), sigma, radius))
            THROW_SORA_EXCEPTION(RuntimeException, "failed gauss blur sprite");
        in->unlockPixelData();
    }
    
    SoraSprite* SoraGraphicAlgorithm::grayNew(SoraSprite* in) {
        SoraTexture* tex = (SoraTexture*)in->getTexture();
        
        SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
        uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        uint32* data = in->getPixelData();
        memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        in->unlockPixelData();
        if(!::sora::cpu_graphic_effect::gray(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight))
            THROW_SORA_EXCEPTION(RuntimeException, "failed gray sprite");
        SoraSprite* out = new SoraSprite((SoraTextureHandle)outtex);
        uint32* odata = out->getPixelData();
        memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        free(outputData);
        out->unlockPixelData();
        if(!out)
            THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
        
        return out;
    }
    
    void SoraGraphicAlgorithm::gray(SoraSprite* in) {
        uint32* data = in->getPixelData();
        if(!::sora::cpu_graphic_effect::gray(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight()))
            THROW_SORA_EXCEPTION(RuntimeException, "failed gray sprite");
        in->unlockPixelData();
    }
    
    SoraSprite* SoraGraphicAlgorithm::reverseNew(SoraSprite* in) {
        SoraTexture* tex = (SoraTexture*)in->getTexture();
        
        SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
        uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        uint32* data = in->getPixelData();
        memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        in->unlockPixelData();
        if(!::sora::cpu_graphic_effect::reverse(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight))
            THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
        SoraSprite* out = new SoraSprite((SoraTextureHandle)outtex);
        uint32* odata = out->getPixelData();
        memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        free(outputData);
        out->unlockPixelData();
        if(!out)
            THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
        
        return out;
    }
    
    void SoraGraphicAlgorithm::reverse(SoraSprite* in) {
        uint32* data = in->getPixelData();
        if(!::sora::cpu_graphic_effect::reverse(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight()))
            THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
        in->unlockPixelData();
    }
    
    SoraSprite* SoraGraphicAlgorithm::alphaMixNew(SoraSprite* in, SoraSprite* src, double a, int posx, int posy) {
        SoraTexture* tex = (SoraTexture*)in->getTexture();
        
        SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
        uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        uint32* data = in->getPixelData();
        memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        in->unlockPixelData();
        if(!::sora::cpu_graphic_effect::alphaMix(outputData, src->getPixelData(), (int32)tex->mTextureWidth, (int32)tex->mTextureHeight, 
                       (int32)src->getTextureWidth(), (int32)src->getTextureHeight(), a, posx, posy))
            THROW_SORA_EXCEPTION(RuntimeException, "failed alphaMix sprite");
        SoraSprite* out = new SoraSprite((SoraTextureHandle)outtex);
        uint32* odata = out->getPixelData();
        memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
        free(outputData);
        out->unlockPixelData();
        src->unlockPixelData();
        if(!out)
            THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
        
        return out;
    }
    
    void SoraGraphicAlgorithm::alphaMix(SoraSprite* in, SoraSprite* src, double a, int posx, int posy) {
        uint32* data1 = in->getPixelData();
        uint32* data2 = src->getPixelData();
        if(!::sora::cpu_graphic_effect::alphaMix(data1, data2, (int32)in->getTextureWidth(), (int32)in->getTextureHeight(), 
                       (int32)src->getTextureWidth(), (int32)src->getTextureHeight(),
                       a, posx, posy))
            THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
        in->unlockPixelData();
        src->unlockPixelData();
    }
    
    SoraSprite* SoraGraphicAlgorithm::scaleNew(SoraSprite* in, float rate) {
        sora_assert(rate > 0.0 && rate <= 1.0);
        
        if(rate == 1.0)
            return new SoraSprite(in->getTexture());
        
        uint32* indata = in->getPixelData();
        SoraTextureHandle outtex = SoraCore::Instance()->createTextureWH(in->getTextureWidth(true) * rate, 
                                                                         in->getTextureHeight(true) * rate);
        sora_assert(outtex);
        
        uint32* outdata = SoraCore::Instance()->textureLock(outtex);
        sora_assert(outdata);
        
        ::sora::cpu_graphic_effect::bresenhamScale(outdata, 
                                                   indata, 
                                                   in->getTextureWidth(true), 
                                                   in->getTextureHeight(true), 
                                                   in->getTextureWidth(true)*rate, 
                                                   in->getTextureHeight(true)*rate);
        
        SoraCore::Instance()->textureUnlock(outtex);
        in->unlockPixelData();
        
        return new SoraSprite(outtex);
    }
    
    void SoraGraphicAlgorithm::scale(SoraTextureHandle dst, SoraSprite* src) {
        int32 dstWidth = SoraTexture::GetOriginalWidth(dst);
        int32 dstHeight = SoraTexture::GetOriginalHeight(dst);
        
        uint32* dstData = SoraCore::Instance()->textureLock(dst);
        sora_assert(dstData);
        
        uint32* inData = src->getPixelData();
        sora_assert(inData);
        
        ::sora::cpu_graphic_effect::bresenhamScale(dstData, 
                                                   inData, 
                                                   src->getTextureWidth(true), 
                                                   src->getTextureHeight(true), 
                                                   dstWidth, 
                                                   dstHeight);
        
        SoraCore::Instance()->textureUnlock(dst);
        src->unlockPixelData();
    }
    
} // namespace sora