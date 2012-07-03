/*
 *  SoraZSorter.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/6/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraZSorter.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraRenderSystem.h"

namespace sora {
	
	typedef struct __Z_BUFFER_NODE {
		SoraVertex*			vertex;
		SoraTexture*		tex;
		SoraShaderContext*	shader;
		int32				blend;
		int32				size;
		RenderMode			drawMode;
        SoraMatrix4         viewMatrix;
		
		void release() {
			if(vertex) {
				delete []vertex;
				vertex = NULL;
				size = 0;
			}
			__Z_BUFFER_NODE* tnext = next;
			while(tnext != NULL) {
				__Z_BUFFER_NODE* ttnext = tnext;
				tnext = ttnext->next;
				
				delete [](ttnext->vertex);
				delete ttnext;
			}
			next = NULL;
		}
		
		__Z_BUFFER_NODE* next;
		__Z_BUFFER_NODE* tail;
		
		__Z_BUFFER_NODE(): vertex(NULL), shader(NULL), tex(NULL), blend(BLEND_DEFAULT), size(0), next(NULL), tail(NULL) {}
	} __Z_BUFFER_NODE;
	
	__Z_BUFFER_NODE __z_buffer_array[1000];
    	
	void __z_buffer_insert_node(const __Z_BUFFER_NODE& node, int32 z) {
		if(z >= 1000)
			z = 999;
		else if(z <= 0)
			z = 0;
		
		__Z_BUFFER_NODE* cnode = &__z_buffer_array[z];
		if(cnode->size != 0) {
			__Z_BUFFER_NODE* newNode = new __Z_BUFFER_NODE();
			memcpy(newNode, &node, sizeof(__Z_BUFFER_NODE));
			
			if(cnode->tail) {
				cnode->tail->next = newNode;
				cnode->tail = newNode;
			} else {
				cnode->tail = newNode;
				cnode->next = newNode;
			}
		} else {
			__z_buffer_array[z] = node;
		}
	}
	
	void SoraZSorter::renderQuad(SoraQuad& quad, SoraShaderContext* shader) {
		int32 z = static_cast<int32>(quad.v[0].z * 1000);
		__Z_BUFFER_NODE node;
		node.vertex = new SoraVertex[4];
		memcpy(node.vertex, &quad.v[0], sizeof(SoraVertex)*4);
		node.size = 4;
		node.blend = quad.blend;
		node.tex = quad.tex;
		node.shader = shader;
        node.viewMatrix = sora::SoraCore::Ptr->getRenderSystem()->getTransformMatrix();
		
		__z_buffer_insert_node(node, z);
	}
	
	void SoraZSorter::renderWithVertices(SoraTextureHandle tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode, SoraShaderContext* shader) {
		// use z 0
		int32 z = static_cast<int32>(vertices[0].z * 1000);
		__Z_BUFFER_NODE node;
		node.vertex = new SoraVertex[vsize];
		memcpy(node.vertex, vertices, sizeof(SoraVertex)*vsize);
		node.size = vsize;
		node.blend = blendMode;
		node.tex = (SoraTexture*)tex;
		node.shader = shader;
		node.drawMode = mode;
        node.viewMatrix = sora::SoraCore::Ptr->getRenderSystem()->getTransformMatrix();
		
		__z_buffer_insert_node(node, z);
	}
	
	void SoraZSorter::endSortAndRender() {
        SoraCore::Ptr->pushTransformMatrix();
        
		for(int i=999; i>=0; --i) {
			__Z_BUFFER_NODE* node = &__z_buffer_array[i];
			while(node != NULL) {
				if(node->size == 0)
					break;
				
				if(node->size == 4) {
					SoraQuad quad;
					memcpy(&quad.v[0], node->vertex, sizeof(SoraVertex)*4);
					quad.blend = node->blend;
					quad.tex = node->tex;
					
					if(node->shader)
						SoraCore::Ptr->attachShaderContext(node->shader);
                    SoraCore::Ptr->getRenderSystem()->setTransformMatrix(node->viewMatrix);
					SoraCore::Ptr->renderQuad(quad);
					SoraCore::Ptr->detachShaderContext();
				} else if(node->size != 0) {
					if(node->shader)
						SoraCore::Ptr->attachShaderContext(node->shader);
                    SoraCore::Ptr->getRenderSystem()->setTransformMatrix(node->viewMatrix);
					SoraCore::Ptr->renderWithVertices((SoraTextureHandle)node->tex, node->blend, node->vertex, node->size, node->drawMode);
					SoraCore::Ptr->detachShaderContext();
				}
				node = node->next;
			}
			__z_buffer_array[i].release();
		}
        
        SoraCore::Ptr->popTransformMatrix();
	}
}