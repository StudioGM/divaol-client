#include "soraguiimage.hpp"
#include "soraguiimageloader.hpp"
#include "guichan/exception.hpp"

#include "SoraStringConv.h"

namespace gcn
{

    SoraGUIImageLoader::SoraGUIImageLoader()
    {
        sora = sora::SoraCore::Instance();
    }
  
    SoraGUIImageLoader::~SoraGUIImageLoader()
    {
    }

    Image *SoraGUIImageLoader::load(const std::string &filename, bool convertToDisplayFormat)
    {
        sora::SoraTexture* tex = (sora::SoraTexture*)sora->createTexture(sora::s2ws(filename));
		
        if (tex == NULL)
        {
            throw GCN_EXCEPTION(std::string("Unable to load: ") + filename);
        }

        SoraGUIImage *image = new SoraGUIImage((sora::SoraTextureHandle)tex);
		image->setName(filename);
		
        if (convertToDisplayFormat)
        {
            image->convertToDisplayFormat();
        }

        return image; 
    }

	Image *SoraGUIImageLoader::load(const std::wstring &filename, bool convertToDisplayFormat)
	{
		sora::SoraTexture* tex = (sora::SoraTexture*)sora->createTexture(filename);

		if (tex == NULL)
		{
			throw GCN_EXCEPTION(std::string("Unable to load: ") + sora::ws2s(filename));
		}

		SoraGUIImage *image = new SoraGUIImage((sora::SoraTextureHandle)tex);
		image->setName(sora::ws2s(filename));

		if (convertToDisplayFormat)
		{
			image->convertToDisplayFormat();
		}

		return image; 
	}
}
