#include "SoraDXRenderBuffer.h"

#include "SoraRenderSystem.h"
#include "SoraCore.h"

namespace sora {

	SoraDXVertexBuffer::SoraDXVertexBuffer(SoraRenderBuffer::Access access, 
									SoraRenderBuffer::Usage usage, 
									uint32 desired_count,
									void* initData,
									const SoraVertexFormat& format):
	SoraRenderBuffer(access, usage),
	mFormat(format),
	mVertexBuffer(0) {
		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		sora_assert(device);

		DWORD dusage;
		DWORD dflag = 0;

		switch(usage) {
			case WriteOnly:
				dusage = D3DUSAGE_WRITEONLY;
				break;
			default:
				dusage = 0;
				break;
		}
		if(format.checkFormat(VertexFormat::VertexColor))
			dflag = dflag | D3DFVF_DIFFUSE;
		if(format.checkFormat(VertexFormat::VertexColor2))
			dflag = dflag | D3DFVF_SPECULAR;
		if(format.checkFormat(VertexFormat::VertexNormal))
			dflag = dflag | D3DFVF_NORMAL;
		if(format.checkFormat(VertexFormat::VertexXYZ))
			dflag = dflag | D3DFVF_XYZ;
		if(format.checkFormat(VertexFormat::VertexUV))
			dflag = dflag | D3DFVF_TEX1;

	    device->CreateVertexBuffer(desired_count*format.totalSize(), dusage, dflag, D3DPOOL_DEFAULT, &mVertexBuffer, NULL);
		sora_assert(mVertexBuffer);

		mCount = desired_count;
	}
        
    SoraDXVertexBuffer::~SoraDXVertexBuffer() {
		if(mVertexBuffer)
			mVertexBuffer->Release();
	}
        
    void* SoraDXVertexBuffer::map() {
		void* data;
	    mVertexBuffer->Lock(0, mCount*mFormat.totalSize(), &data, 0);
		return data;
	}

    void SoraDXVertexBuffer::unmap() {
		mVertexBuffer->Unlock();
	}
        
    void SoraDXVertexBuffer::active() {
		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		device->SetStreamSource(0, mVertexBuffer, 0, mFormat.totalSize());
	}
        
    SoraVertexFormat& SoraDXVertexBuffer::format() {
		return mFormat;
	}

    const SoraVertexFormat& SoraDXVertexBuffer::format() const {
		return mFormat;
	}
        
    uint32 SoraDXVertexBuffer::count() const {
		return mCount;
	}

	void SoraDXVertexBuffer::resize(uint32 desired_count) {
		if(mVertexBuffer)
			mVertexBuffer->Release();

		DWORD dflag;
		if(mFormat.checkFormat(VertexFormat::VertexColor))
			dflag = dflag | D3DFVF_DIFFUSE;
		if(mFormat.checkFormat(VertexFormat::VertexColor2))
			dflag = dflag | D3DFVF_SPECULAR;
		if(mFormat.checkFormat(VertexFormat::VertexNormal))
			dflag = dflag | D3DFVF_NORMAL;
		if(mFormat.checkFormat(VertexFormat::VertexXYZ))
			dflag = dflag | D3DFVF_XYZ;
		if(mFormat.checkFormat(VertexFormat::VertexUV))
			dflag = dflag | D3DFVF_TEX1;

		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		device->CreateVertexBuffer(desired_count*mFormat.totalSize(), D3DUSAGE_WRITEONLY, dflag, D3DPOOL_DEFAULT, &mVertexBuffer, NULL);
		sora_assert(mVertexBuffer);

		mCount = desired_count;
	}

	SoraDXIndexBuffer::SoraDXIndexBuffer(SoraRenderBuffer::Access access, 
                           SoraRenderBuffer::Usage usage, 
                           uint32 desired_count,
						   void* initData):
	SoraRenderBuffer(access, usage),
	mIndexBuffer(0) {
		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		sora_assert(device);

		DWORD dusage = 0;

		switch(usage) {
			case WriteOnly:
				dusage = D3DUSAGE_WRITEONLY;
				break;
			default:
				dusage = 0;
				break;
		}
		device->CreateIndexBuffer(desired_count*sizeof(uint32), dusage, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mIndexBuffer, 0);
		sora_assert(mIndexBuffer);

		mCount = desired_count;
	}
        
    SoraDXIndexBuffer::~SoraDXIndexBuffer() {
		if(mIndexBuffer)
			mIndexBuffer->Release();
	}
        
    void* SoraDXIndexBuffer::map() {
		void* data;
		mIndexBuffer->Lock(0, mCount*sizeof(uint32), &data, 0);
		return data;
	}
   
	void SoraDXIndexBuffer::unmap() {
		mIndexBuffer->Unlock();
	}
        
    void SoraDXIndexBuffer::active() {
		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		sora_assert(device);
		
		device->SetIndices(mIndexBuffer);
	}
        

	uint32 SoraDXIndexBuffer::count() const {
		return mCount;
	}
	
	void SoraDXIndexBuffer::resize(uint32 desired_count) {
		mIndexBuffer->Release();

		IDirect3DDevice9* device = (IDirect3DDevice9*)SoraCore::Ptr->getVideoDeviceHandle();
		sora_assert(device);

		device->CreateIndexBuffer(desired_count*sizeof(uint32), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mIndexBuffer, 0);
		sora_assert(mIndexBuffer);

		mCount = desired_count;
	}

} // namespace sora