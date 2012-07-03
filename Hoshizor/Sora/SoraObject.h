#ifndef _SORA_OBJECT_H
#define _SORA_OBJECT_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"
#include "SoraAutoPtr.h"
#include "SoraMovable.h"
#include "SoraFunction.h"

#include <algorithm>
#include <list>


namespace sora {
    
    class SoraObjectHandle;
    class SoraAbstractModifierAdapter;
    class SoraPhysicBody;
    class SoraMessageEvent;
    struct SoraPhysicFixtureDef;
    struct SoraPhysicBodyDef;

	class SORA_API SoraObject: public SoraEventHandler, public SoraMovable {
	public:
        friend class SoraObjectHandle;
        
		SoraObject();
		virtual ~SoraObject();

		virtual	int32	update(float dt);
		virtual void	render();
        
        // inherited from SoraEventHandler
        virtual void    onUpdate(float dt);
        
        virtual void    onPositionChange(float x, float y, float z);
        virtual void    onParentChange(SoraObject* parent);
        virtual void    on3DEnabled(bool flag);
        virtual void    onMessage(SoraMessageEvent* message);
        
		virtual void	add(SoraObject* pobj);
		virtual void	del(SoraObject* pobj);
        virtual void    delAll();
        
    public:
        
        /**
         * position
         **/
    	
        void setPosition(float x, float y, float z=0.f);
        void setPosition(const SoraVector3& pos);
        SoraVector3 getPosition() const;
        
        float getPositionX() const;
        float getPositionY() const;
        float getPositionZ() const;
        
        float getAbsolutePositionX() const;
        float getAbsolutePositionY() const;
        
    public:
        
        /**
         * 3d transform
         **/
        
        void setTransform(const SoraTransform& transform);
        
        SoraTransform&       getTransform();
        const SoraTransform& getTransform() const;

        void enable3D(bool flag);
        bool is3DEnabled() const;
        
    private:
        bool m3DEnabled;

    public:
        
        /** 
         * child objects
         **/
        
        void    setParent(SoraObject* obj);
        
		SoraObject*  getObjList() const;
		SoraObject*  getParent() const;
        
        int32        getObjSize() const;
        SoraObject*  getNext() const;
				
		SoraObject* getObjByName(const SoraString& n);
        SoraObject* getObjByName(SoraStringId sid);
		
    public:
        
        /**
         * type and name
         **/
        
		uint32  getType() const;
		void    setType(uint32 t);
        
        SoraHandle          getHandleId() const;
        SoraUniqueId        getUniqueId() const;
        SoraObjectHandle    getHandle();
        
        void setName(const SoraString& name);
        void setName(SoraStringId n);
		SoraString getName() const;
        
    public:
        
        /**
         * modifiers
         **/
        
        void moveTo(float x, float y, float t);
        
        typedef SoraFunction<void(SoraObject*)> NotificationFunc;
        void moveToAndNotify(float x, float y, float t, const NotificationFunc& onFinish);
        
    public:
        
        /**
         * physics
         **/
        
        /**
         * If a physic body is binded, the object would update itself and move itself to
         * the body's position everyframe
         **/
        void            attachPhysicBody(SoraPhysicBody* body, bool autoRelease=false);
        void            createPhysicBody(const SoraPhysicBodyDef& def, const SoraPhysicFixtureDef& fixtureDef, float mass, float centerX, float centerY);
        void            detachPhysicBody();
        SoraPhysicBody* getPhysicBody() const;
        
    public:
        
        void addModifierAdapter(SoraAbstractModifierAdapter* adapter);
        void removeModifierAdapter(SoraAbstractModifierAdapter* adapter);
        bool hasModifierAdapter() const;
        void clearModifierAdapters();
        
    private:
        void  setNext(SoraObject* obj);

        typedef std::list<SoraAbstractModifierAdapter*> ModifierAdapterList;
        ModifierAdapterList mModifierAdapters;
		
	protected:
		
        SoraObject* operator[](const SoraString& name);
            
	protected:
       
		SoraObject* mParent;
        SoraObject* mSubObjects;
        SoraObject* mNext;
        
		uint32 mType;
        
        int32 mSubObjectSize;
        SoraString mName;
        
        bool mAutoReleasePhysicBody;
        SoraPhysicBody* mPhysicBody;
        
		SoraObject(const SoraObject& rhs);
        SoraObject& operator =(const SoraObject& rhs);
        
    private:
        /**
         *  handleId and UniqueId for the object cannot be copied
         **/        
        SoraHandle mHandleId;
        SoraUniqueId mUniqueId;
    };

} // namespace sora

#endif
