/*
 *  SoraCoreTransform.cpp
 *  loliEngine Rewrite
 *
 *  Created by Robert Bu on 7/19/10.
 *  Copyright 2010 GM&Symphonie&ring0dev. All rights reserved.
 *  Modified by Griffin Bu for Sora 14/10/10
 *
 */

#ifndef SoraCoreTransform_H
#define SoraCoreTransform_H

#include "SoraPlatform.h"

namespace sora {
	enum {
		TRANSFORM_1F = 1,
		TRANSFORM_2F = 2,
		TRANSFORM_3F = 3,
		TRANSFORM_4F = 4,
	};

	/* from loliEngine rewrite */
	class SORA_API SoraCoreTransform {
	public:
		SoraCoreTransform(real _f);
		SoraCoreTransform(real _f1, real _f2);
		SoraCoreTransform(real _f1, real _f2, real _f3);
		SoraCoreTransform(real _f1, real _f2, real _f3, real _f4);
	
		SoraCoreTransform();
			
		void Set(real _f);
		void Set(real _f1, real _f2);
		void Set(real _f1, real _f2, real _f3);
		void Set(real _f1, real _f2, real _f3, real _f4);
	
		int GetKind() const { return iKind; }
	
		real Get1st() const;
		real Get2nd() const;
		real Get3rd() const;
		real Get4th() const;
	
		SoraCoreTransform GetDist(const SoraCoreTransform& t2, int divider);
	
		SoraCoreTransform operator / (real divider) const;
		SoraCoreTransform operator * (real m) const;
        SoraCoreTransform operator * (const SoraCoreTransform& rhs) const;
		SoraCoreTransform operator - (const SoraCoreTransform& rhs) const;
		SoraCoreTransform operator + (const SoraCoreTransform& rhs) const;
		void operator = (const SoraCoreTransform& rhs);
	
		SoraCoreTransform& operator += (const SoraCoreTransform& rhs);
		SoraCoreTransform& operator -= (const SoraCoreTransform& rhs);
		
		bool operator == (const SoraCoreTransform& rhs) const;
			
	private:
		real f1, f2, f3, f4;
		bool bClean;	
		int iKind;
	};
	
} // namespace sora

#endif