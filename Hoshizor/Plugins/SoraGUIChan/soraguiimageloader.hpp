#ifndef GCN_SORAIMAGELOADER_HPP
#define GCN_SORAIMAGELOADER_HPP

#include "guichan/platform.hpp"
#include "guichan/imageloader.hpp"
#include "SoraCore.h"

namespace gcn
{
    class Image;

    class GCN_EXTENSION_DECLSPEC SoraGUIImageLoader : public ImageLoader
    {
    public:

        /**
         * Constructor.
         */
        SoraGUIImageLoader();

        /**
         * Destructor.
         */
        ~SoraGUIImageLoader();


        // Inherited from ImageLoader

        virtual Image* load(const std::string &filename, bool convertToDisplayFormat = true);
		virtual Image* load(const std::wstring &filename, bool convertToDisplayFormat = true);

    protected:
        sora::SoraCore* sora;
    };
}

#endif // end GCN_HGEIMAGELOADER_HPP
