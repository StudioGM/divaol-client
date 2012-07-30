/*
 *  DivaHDPSystem.h
 *
 *  Created by Hyf042 on 3/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_HD_PSYSTEM
#define DIVA_HD_PSYSTEM

#include "Core/DivaCommon.h"
#include "Core/DivaMapInfo.h"
#include "Core/DivaEventManager.h"
#include "SoraColor.h"
#include "SoraSprite.h"
#include "Core/DivaParticleSystem.h"

namespace divacore
{
	int UNIT_COLOR[][3]={
		{246,  127,  167},	//UNIT_CIRCLE
		{240,  135,  230},	//UNIT_RECT
		{98,  191,  225},	//UNIT_CROSS
		{234, 254,  183},	//UNIT_TRIANGLE
		{240,  135,  230},	//UNIT_RIGHT
		{246,  127,  167},	//UNIT_LEFT
		{234, 254,  183},	//UNIT_DOWN
		{98,  191,  225},	//UNIT_UP
	};

	class ParticleBase
	{
	protected:
		float	_age;
		float	_x,_y,_size;
		sora::SoraSprite *_drawTex;
		std::string tag;

	public:
		short	_type, _state;

		ParticleBase(float x,float y,sora::SoraSprite *drawTex):_age(0),_x(x),_y(y),_drawTex(drawTex){}
		virtual void Draw() = 0;
		virtual bool Update(float) = 0;	// return false if die
		virtual bool Die() = 0;

		void setTag(const std::string &tag) {this->tag=tag;}
		std::string getTag() {return tag;}
	};

	class ParticleTail : public ParticleBase
	{
	private:
		int _a,_r,_g,_b;
	public:
		static Rect srcRect;
		static Point center;

		ParticleTail(int x,int y,int a,int *rgb,sora::SoraSprite* drawTex,int size)
			:ParticleBase(x,y,drawTex),_a(a),_r(rgb[0]),_g(rgb[1]),_b(rgb[2]){
				_size = size;
		}

		void Draw()
		{
			_drawTex->setTextureRect(srcRect.x,srcRect.y,srcRect.w,srcRect.h);
			_drawTex->setColor(CARGB(_a,_r,_g,_b));
			_drawTex->setScale(_size/_drawTex->getSpriteWidth(),_size/_drawTex->getSpriteHeight());
			_drawTex->setCenter(center.x,center.y);
			_drawTex->render(_x,_y);
		}
		bool Update(float dwTimeMilliSecond)	// return false if die
		{
			_age += dwTimeMilliSecond;
			_a -= (_age>145?4:2);
			if(_a<0)_a=0;
			return (_a!=0);
		}
		bool Die()
		{
			return (!_a);
		}
	};

	class ParticleComet : public ParticleBase
	{
	private:
		//state:0~6->normal, 1->normal
		int		_a,_r,_g,_b;
		float	dx,dy;

	public:
		static Rect srcRect;
		static Point center;
		ParticleComet(int x,int y,int a,int *rgb,sora::SoraSprite* drawTex, float _dx, float _dy, float size, int type, short state)
			:ParticleBase(x,y,drawTex),_a(a),_r(rgb[0]),_g(rgb[1]),_b(rgb[2]),dx(_dx),dy(_dy){
				_type = type;
				_state = state;
				_size = size;
		}

		void Draw()
		{
			int32 tmp = _drawTex->getBlendMode();
			_drawTex->setBlendMode(BLEND_NOZWRITE);
			_drawTex->setTextureRect(srcRect.x,srcRect.y,srcRect.w,srcRect.h);
			_drawTex->setColor(CARGB(_a,_r,_g,_b));
			_drawTex->setScale(4*_size/_drawTex->getSpriteWidth(),4*_size/_drawTex->getSpriteHeight());
			_drawTex->setCenter(center.x,center.y);
			_drawTex->setPosition(_x,_y);
			Core::Ptr->render(_drawTex,getTag());
			_drawTex->setBlendMode(tmp);
		}
		bool Update(float dwTimeMilliSecond)	// return false if die
		{
			// increment age count
			_age += dwTimeMilliSecond;

			// update color
			if( _state & 1 )
			{	// change color every 2 frames
				int *unit_color = UNIT_COLOR[_type];
				if( _state & 0x0008 ){
					int R=rand()&0xff,G=rand()&0xff,B=rand()&0xff;
					int maxv = max(R,G);
					maxv = max(maxv,B);
					float r = (float)256/(maxv+1);
					_r = ((int)((R+1)*r)+unit_color[0])>>1;
					_g = ((int)((G+1)*r)+unit_color[1])>>1;
					_b = ((int)((B+1)*r)+unit_color[2])>>1;
				}else{
					_r = (rand()&0xff+unit_color[0])>>1;
					_g = (rand()&0xff+unit_color[1])>>1;
					_b = (rand()&0xff+unit_color[2])>>1;
				}
			}
			if( _state & 2 )
				_a = (rand()&0xff);
			if( _state & 4 )
				_size = (float)rand()*(8.0f/RAND_MAX);

			// update motion
			float mul = ((_state&0x4000)?1500.0f:36.0f)/(_age+20);
			_x += dx*mul;
			_y += dy*mul;

			// update fading
			_a -= (_state&0x8000)?14:2;
			if(_a<0)
				_a=0;
			return (_a!=0);
		}
		bool Die()		// return true if die
		{
			return (!_a);
		}
	};
	Rect ParticleComet::srcRect = Rect();
	Point ParticleComet::center = Point();


	class PSystem
	{

		int particleType;
        
        typedef std::vector<ParticleBase*> ParticleBaseVector;
        typedef std::map<int, ParticleBaseVector> ParticleMap;
		ParticleMap m_particle_groups;

	public:
		PSystem():particleType(4){}
		~PSystem(){Clear();}

		void Draw()
		{
			//drawSP();
			//return;

			if(particleType>0){
                for(ParticleMap::iterator it = m_particle_groups.begin(),
                    end = m_particle_groups.end();
                    it != end;
                    ++it) {
                    for(ParticleBaseVector::iterator it2 = it->second.begin(),
                        end2 = it->second.end();
                        it2 != end2;
                        ++it2) {
                        (*it2)->Draw();
					}
				}
			}
		}
		void Update(float dwTimeMilliSecond)
		{
			for (ParticleMap::iterator group_it = m_particle_groups.begin();
                 group_it!=m_particle_groups.end();){
				ParticleBaseVector &group = group_it->second;
				for ( int n=0, N=group.size(); n<N; n++ ){
					if(!group[n]->Update(dwTimeMilliSecond)){
						delete group[n];
						group[n] = group[--N];
						group.resize(N);
						n--;
					}
				}
				if(group.empty())
				{
                    //
					m_particle_groups.erase(group_it++);
					continue;
				}
				++group_it;
			}
		}

		void AddParticle(ParticleBase* p, int group)
		{
			ParticleBaseVector &particle_group = m_particle_groups[group];
			if(particle_group.empty()) particle_group.reserve(256);
			particle_group.push_back(p);
		}

		void Clear()
		{
			if(particleType>0){
                for(ParticleMap::iterator it = m_particle_groups.begin(),
                    end = m_particle_groups.end();
                    it != end;
                    ++it) {
                    for(ParticleBaseVector::iterator it2 = it->second.begin(),
                        end2 = it->second.end();
                        it2 != end2;
                        ++it2) {
                        delete *it2;
					}
				}
			}
			m_particle_groups.clear();
		}
		void Clear(int group)
		{
			ParticleBaseVector &particle_group = m_particle_groups[group];
			for(ParticleBaseVector::iterator it2 = particle_group.begin(),
                end2 = particle_group.end();
                it2 != end2;
                ++it2) {
                delete *it2;
            }
			m_particle_groups.erase(group);
		}

		void SetState(int state_mask)
		{
			for(ParticleMap::iterator it = m_particle_groups.begin(),
                end = m_particle_groups.end();
                it != end;
                ++it) {
                for(ParticleBaseVector::iterator it2 = it->second.begin(),
                    end2 = it->second.end();
                    it2 != end2;
                    ++it2) {
                    (*it2)->_state |= state_mask;
                }
            }
		}
		void SetState(int state_mask, int group)
		{
			ParticleBaseVector &particle_group = m_particle_groups[group];
			for(ParticleBaseVector::iterator it2 = particle_group.begin(),
                end2 = particle_group.end();
                it2 != end2;
                ++it2) {
                (*it2)->_state |= state_mask;
            }
		}

		void AddCometParticle(float x, float y, float dx, float dy, NotePtr note)
		{
			static SoraSprite *particle_sphere = NULL;
			if(!particle_sphere){
				particle_sphere = sora::SoraCore::Ptr->createSprite("pic/effect/particle_sphere.png");
				Rect rect = Rect(0,0,particle_sphere->getSpriteWidth(),particle_sphere->getSpriteHeight());
				ParticleComet::srcRect = rect;
				ParticleComet::center = Point(rect.w*0.5f,rect.h*0.5f);
			}

			// Random color interpolated with Unit color
			int rgb[3]={rand()&0xff,rand()&0xff,rand()&0xff}, *unit_color = UNIT_COLOR[note->getReceivePoint().type];
			//if(serial&1){
			if(0){
				int maxv = max(rgb[0],rgb[1]);
				maxv = max(maxv,rgb[2]);
				float r = (float)256/(maxv+1);
				rgb[0] = (rgb[0]+1)*r;
				rgb[1] = (rgb[1]+1)*r;
				rgb[2] = (rgb[2]+1)*r;
			}
			rgb[0] = (rgb[0]+unit_color[0])>>1;
			rgb[1] = (rgb[1]+unit_color[1])>>1;
			rgb[2] = (rgb[2]+unit_color[2])>>1;

			// Adjust disperse density
			Point p = Point(dx,dy);
			p = p.unit()*sqrt((float)rand()/RAND_MAX);
			dx = p.x*4, dy = p.y*4;
			//SetLength(dx,dy,(float)rand()/RAND_MAX);

			// Add particle
			//if(serial&1)
			ParticleBase *particle = (new ParticleComet(x,y,255,rgb,particle_sphere,dx,dy,
				(float)rand()*(5.0f/RAND_MAX),note->getReceivePoint().type,0));
			particle->setTag(note->getTailTag());
			AddParticle((ParticleBase*)particle
				,note->getID());
		}

		void AddStarParticle(float x, float y, float size, NotePtr note){
			static sora::SoraSprite *particle_star = NULL;
			if(!particle_star){
				particle_star = sora::SoraCore::Ptr->createSprite("pic/effect/particle_star.png");
				Rect rect = Rect(0,0,particle_star->getSpriteWidth(),particle_star->getSpriteHeight());
				ParticleComet::srcRect = rect;
				ParticleComet::center = Point(rect.w*0.5f,rect.h*0.5f);
			}

			// Random color interpolated with Unit color
			int rgb[3]={rand()&0xff,rand()&0xff,rand()&0xff}, *unit_color = UNIT_COLOR[note->getReceivePoint().type];
			rgb[0] = (rgb[0]+unit_color[0])>>1;
			rgb[1] = (rgb[1]+unit_color[1])>>1;
			rgb[2] = (rgb[2]+unit_color[2])>>1;

			// Add particle
			ParticleBase *particle = new ParticleComet(x,y,255,rgb,particle_star,0,0,size,note->getReceivePoint().type,2);
			particle->setTag(note->getTailTag());
			AddParticle(
				particle,
				note->getID()
				);
		}
	};

}

#endif