#include "SoraCoreTransform.h"

namespace sora {
    
    SoraCoreTransform::SoraCoreTransform(real _f): 
    f1(_f), 
    bClean(false), 
    iKind(TRANSFORM_1F) {
    }
    
    SoraCoreTransform::SoraCoreTransform(real _f1, real _f2): 
    f1(_f1), 
    f2(_f2), 
    bClean(false), 
    iKind(TRANSFORM_2F) {
    }
    
    SoraCoreTransform::SoraCoreTransform(real _f1, real _f2, real _f3): 
    f1(_f1), 
    f2(_f2), 
    f3(_f3), 
    bClean(false), 
    iKind(TRANSFORM_3F) {
    }
    
    SoraCoreTransform::SoraCoreTransform(real _f1, real _f2, real _f3, real _f4):
    f1(_f1), 
    f2(_f2), 
    f3(_f3), 
    f4(_f4), 
    bClean(false), 
    iKind(TRANSFORM_4F) {
    }
	
    SoraCoreTransform::SoraCoreTransform():
    bClean(true), 
    iKind(0) {
    }
    
    void SoraCoreTransform::Set(real _f) { 
        f1 = _f; 
        bClean = false;
        iKind = TRANSFORM_1F;
    }
    
    void SoraCoreTransform::Set(real _f1, real _f2) {
        f1 = _f1; 
        f2 = _f2;
        bClean = false;
        iKind = TRANSFORM_2F;
    }
    
    void SoraCoreTransform::Set(real _f1, real _f2, real _f3) { 
        f1 = _f1;
        f2 = _f2; 
        f3 = _f3;
        bClean = false; 
        iKind = TRANSFORM_3F;
    }
    
    void SoraCoreTransform::Set(real _f1, real _f2, real _f3, real _f4) {
        f1 = _f1;
        f2 = _f2;
        f3 = _f3;
        f4 = _f4;
        bClean = false;
        iKind = TRANSFORM_4F; 
    }
	
	SoraCoreTransform SoraCoreTransform::GetDist(const SoraCoreTransform& t2, int divider) {
		if(divider == 0)
			 return *this;
	
		if(bClean) return *this;
		
		if(iKind != t2.GetKind()) {
			//THROW_SORA_EXCEPTION("SoraCoreTransform::GetDist: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform((f1-t2.f1)/divider); break;
			case TRANSFORM_2F: return SoraCoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider); break;
			case TRANSFORM_3F: return SoraCoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider, (f3-t2.f3)/divider); break;
			case TRANSFORM_4F: return SoraCoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider, (f3-t2.f3)/divider, (f4-t2.f4)/divider); break;
		}
		return *this;
	}

	SoraCoreTransform SoraCoreTransform::operator / (real divider) const {
		if(bClean) return *this;
	
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform(f1/divider); break;
			case TRANSFORM_2F: return SoraCoreTransform(f1/divider, f2/divider); break;
			case TRANSFORM_3F: return SoraCoreTransform(f1/divider, f2/divider, f3/divider); break;
			case TRANSFORM_4F: return SoraCoreTransform(f1/divider, f2/divider, f3/divider, f4/divider); break;
		}
		return *this;
	}

	SoraCoreTransform SoraCoreTransform::operator + (const SoraCoreTransform& rhs) const {
		if(bClean) return rhs;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("SoraCoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform(f1+rhs.f1); break;
			case TRANSFORM_2F: return SoraCoreTransform(f1+rhs.f1, f2+rhs.f2); break;
			case TRANSFORM_3F: return SoraCoreTransform(f1+rhs.f1, f2+rhs.f2, f3+rhs.f3); break;
			case TRANSFORM_4F: return SoraCoreTransform(f1+rhs.f1, f2+rhs.f2, f3+rhs.f3, f4+rhs.f4); break;
		}
		return *this;
	}

	
	SoraCoreTransform SoraCoreTransform::operator - (const SoraCoreTransform& rhs) const {
		if(bClean) return rhs;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("SoraCoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform(f1-rhs.f1); break;
			case TRANSFORM_2F: return SoraCoreTransform(f1-rhs.f1, f2-rhs.f2); break;
			case TRANSFORM_3F: return SoraCoreTransform(f1-rhs.f1, f2-rhs.f2, f3-rhs.f3); break;
			case TRANSFORM_4F: return SoraCoreTransform(f1-rhs.f1, f2-rhs.f2, f3-rhs.f3, f4-rhs.f4); break;
		}
		return *this;
	}	

	SoraCoreTransform SoraCoreTransform::operator * (real m) const {
		if(bClean) return *this;
	
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform(f1*m); break;
			case TRANSFORM_2F: return SoraCoreTransform(f1*m, f2*m); break;
			case TRANSFORM_3F: return SoraCoreTransform(f1*m, f2*m, f3*m); break;
			case TRANSFORM_4F: return SoraCoreTransform(f1*m, f2*m, f3*m, f4*m); break;
		}
		return *this;
	}
    
    SoraCoreTransform SoraCoreTransform::operator * (const SoraCoreTransform& rhs) const {
        if(bClean) return *this;
        
		switch (iKind) {
			case TRANSFORM_1F: return SoraCoreTransform(f1*rhs.f1); break;
			case TRANSFORM_2F: {
                real x = f1 * rhs.f1;
                real y = f2 * rhs.f2;
                return SoraCoreTransform(x, y);
                break;
            }
                
			case TRANSFORM_3F: {
                real x = f1 * rhs.f1;
                real y = f2 * rhs.f2;
                real z = f3 * rhs.f3;
                return SoraCoreTransform(x, y, z);
                break;
            }
			case TRANSFORM_4F: return SoraCoreTransform(f1*rhs.f1, f2*rhs.f2, f3*rhs.f3, f4*rhs.f4); break;
		}
		return *this;
    }

	void SoraCoreTransform::operator = (const SoraCoreTransform& rhs) {
		iKind = rhs.iKind;
		bClean = rhs.bClean;
	
		switch (iKind) {
			case TRANSFORM_1F: f1 = rhs.f1; break;
			case TRANSFORM_2F: f1 = rhs.f1; f2 = rhs.f2; break;
			case TRANSFORM_3F: f1 = rhs.f1; f2 = rhs.f2; f3 = rhs.f3; break;
			case TRANSFORM_4F: f1 = rhs.f1; f2 = rhs.f2; f3 = rhs.f3; f4 = rhs.f4; break;
		}
	}

	SoraCoreTransform& SoraCoreTransform::operator += (const SoraCoreTransform& rhs) {
		if(bClean) return *this;
	
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("SoraCoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: f1 += rhs.f1; break;
			case TRANSFORM_2F: f1 += rhs.f1; f2 += rhs.f2; break;
			case TRANSFORM_3F: f1 += rhs.f1; f2 += rhs.f2; f3 += rhs.f3; break;
			case TRANSFORM_4F: f1 += rhs.f1; f2 += rhs.f2; f3 += rhs.f3; f4 += rhs.f4; break;
		}
		return *this;
	}
	
	SoraCoreTransform& SoraCoreTransform::operator -= (const SoraCoreTransform& rhs) {
		if(bClean) return *this;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("SoraCoreTransform::Operator +: Different Transform Type");
			return *this;
		}
		
		switch (iKind) {
			case TRANSFORM_1F: f1 -= rhs.f1; break;
			case TRANSFORM_2F: f1 -= rhs.f1; f2 -= rhs.f2; break;
			case TRANSFORM_3F: f1 -= rhs.f1; f2 -= rhs.f2; f3 -= rhs.f3; break;
			case TRANSFORM_4F: f1 -= rhs.f1; f2 -= rhs.f2; f3 -= rhs.f3; f4 -= rhs.f4; break;
		}
		return *this;
	}

	real SoraCoreTransform::Get1st() const {
		if(bClean) return 0.f;
		
		return f1;
	}

	real SoraCoreTransform::Get2nd() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_2F) return f2;
		return 0.f;
	}

	real SoraCoreTransform::Get3rd() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_3F) return f3;
		return 0.f;
	}

	real SoraCoreTransform::Get4th() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_4F) return f4;
		return 0.f;
	}

	bool SoraCoreTransform::operator == (const SoraCoreTransform& rhs) const {
		if(bClean) return false;;
	
		if(iKind != rhs.GetKind()) return false;
	
		switch (iKind) {
			case TRANSFORM_1F: if(f1 != rhs.f1) return false; break;
			case TRANSFORM_2F: if(f1 != rhs.f1 || f2 != rhs.f2) return false; break;
			case TRANSFORM_3F: if(f1 != rhs.f1 || f2 != rhs.f2 || f3 != rhs.f3) return false; break;
			case TRANSFORM_4F: if(f1 != rhs.f1 || f2 != rhs.f2 || f3 != rhs.f3 || f4 != rhs.f4) return false; break;
		}
		return true;
	}

} // namespace sora