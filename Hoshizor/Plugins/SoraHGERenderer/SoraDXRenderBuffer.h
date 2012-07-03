#include "SoraPlatform.h"
#include "SoraRenderBuffer.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace sora {

	class SoraDXVertexBuffer: public SoraRenderBuffer {
    public:
        SoraDXVertexBuffer(SoraRenderBuffer::Access access, 
                            SoraRenderBuffer::Usage usage, 
                            uint32 desired_count,
                            void* initData,
                            const SoraVertexFormat& format);
        
        virtual ~SoraDXVertexBuffer();
        
        virtual void* map();
        virtual void unmap();
        
        virtual void active();
        
        SoraVertexFormat& format();
        const SoraVertexFormat& format() const;
        
        uint32 count() const;
        void resize(uint32 desired_count);
        
    private:
		IDirect3DVertexBuffer9* mVertexBuffer;
        
        uint32 mCount;
        SoraVertexFormat mFormat;
    };
    
    class SoraDXIndexBuffer: public SoraRenderBuffer {
    public:
        SoraDXIndexBuffer(SoraRenderBuffer::Access access, 
                           SoraRenderBuffer::Usage usage, 
                           uint32 desired_count,
                           void* initData);
        
        virtual ~SoraDXIndexBuffer();
        
        virtual void* map();
        virtual void unmap();
        
        virtual void active();
        
        uint32 count() const;
        void resize(uint32 desired_count);
        
    private:
        IDirect3DIndexBuffer9* mIndexBuffer;
        
        uint32 mCount;
    };


} // namespace sora