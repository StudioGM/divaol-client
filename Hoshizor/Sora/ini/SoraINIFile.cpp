/*
 *  SoraINIFile.cpp
 *  OpenGLES Study 1
 *
 *  Created by Robert Bu on 09��8��30��.
 *  Copyright 2009 Robert Bu. All rights reserved.
 *
 */

#include "SoraINIFile.h"

#include "SoraStringConv.h"
#include "SoraCore.h"


#include <fstream>

namespace sora {

SoraINIFile::SoraINIFile() {
	bFileWrite = false;
	publicSec.name = "public";
}

SoraINIFile::~SoraINIFile() {
	closeFile();
}

void SoraINIFile::closeFile() {
	if(bFileWrite) Commit();

	sec.clear();	
    setOpen(false);
}

int32 SoraINIFile::readFile(const SoraWString& path) {
	FILE* fp = fopen(ws2s(path).c_str(), "r");
	if(fp) {
		uint32 size = 0;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		
		void* pdata = malloc(size);
		if(pdata) {
			fread(pdata, size, 1, fp);
			
			sFile = path;
			bMemoryFile = false;
			
			int32 result = readFileMem(pdata, size);
			free(pdata);
			return result;
		}
	}
    setOpen(false);
	return false;
}
	
	inline SoraString readLn(const char* pstr, uint32& currPos, uint32 size) {
		SoraString buffer;
		while(pstr[currPos] != '\n' && pstr[currPos] != '\r' && currPos < size) {
			buffer += pstr[currPos];
			++currPos;
		}
		++currPos;
		return buffer;
	}
	
	int32 SoraINIFile::readFileMem(void* pData, uint32 size) {
		if(isOpen()) closeFile();
		
		sec.clear();
		
		const char* pstrData = (const char*)pData;
		uint32 currPos = 0;

		SoraString buffer;
		INISector section;
		while(currPos <= size) {
			buffer = readLn(pstrData, currPos, size);
			if(buffer.size() == 0)
				continue;
			
			if(buffer[0] != '[') {
				if(buffer.find('=') == SoraString::npos || buffer[0] == ';') {
					continue;
				}
				
				size_t equalPos = buffer.find('=');
				publicSec.value.push_back(INISectorValue(buffer.substr(0, equalPos), buffer.substr(equalPos+1, buffer.size()-equalPos-1)));
			} else {
				while(buffer[0] == '[') {
					if(buffer[0] == '[') {
						size_t right = buffer.find(']');
						if(right != SoraString::npos) {
							section.name = buffer.substr(1, right-1);
							
							buffer = readLn(pstrData, currPos, size);
							while(buffer[0] != '[' && currPos <= size) {
								if(buffer.find('=') == SoraString::npos || buffer[0] == ';') {
									buffer = readLn(pstrData, currPos, size);
									continue;
								}
								
								size_t equalPos = buffer.find('=');
								section.value.push_back(INISectorValue(buffer.substr(0, equalPos), buffer.substr(equalPos+1, buffer.size()-equalPos-1)));
								buffer = readLn(pstrData, currPos, size);
							}
							
							sec.push_back(section);
							section.value.clear();
							section.name.clear();
						} else {
							buffer = readLn(pstrData, currPos, size);
							while(buffer[0] != '[' && currPos < size)
								buffer = readLn(pstrData, currPos, size);
						}
					} 
				}
			}
		}
		
        setOpen(true);
		bMemoryFile = true;
		return true;
	}

    SoraString SoraINIFile::getString(const SoraString& section, const SoraString& name, const SoraString& sDefault) {
        vector<INISector>::iterator p = sec.begin();
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        return p2->value;
                    }
                }
            }
        }
        vector<INISectorValue>::iterator p2 = publicSec.value.begin();
        for(p2 = publicSec.value.begin(); p2 != publicSec.value.end(); ++p2) {
            if(p2->key.compare(name) == 0)
                return p2->value;
        }
        return sDefault;
    }
    
    float SoraINIFile::getFloat(const SoraString& section, const SoraString& name, float fDefault) {
        vector<INISector>::iterator p = sec.begin();
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        return (float)atof(p2->value.c_str());
                    }
                }
            }
        }
        vector<INISectorValue>::iterator p2 = publicSec.value.begin();
        for(p2 = publicSec.value.begin(); p2 != publicSec.value.end(); ++p2) {
            if(p2->key.compare(name) == 0)
                return (float)atof(p2->value.c_str());
        }
        return fDefault;
    }
    
    int32 SoraINIFile::getInt(const SoraString& section, const SoraString& name, int32 iDefault) {
        vector<INISector>::iterator p = sec.begin();
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        return atoi(p2->value.c_str());
                    }
                }
            }
        }
        vector<INISectorValue>::iterator p2 = publicSec.value.begin();
        for(p2 = publicSec.value.begin(); p2 != publicSec.value.end(); ++p2) {
            if(p2->key.compare(name) == 0)
                return atoi(p2->value.c_str());
        }
        return iDefault;
    }
    
    bool SoraINIFile::getBool(const SoraString& section, const SoraString& name, bool bDefault) {
        vector<INISector>::iterator p = sec.begin();
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        if(p2->value.compare("true") == 0 || atoi(p2->value.c_str()) != 0) {
                            return true;
                        }
                        else
                            return false;
                    }
                }
            }
        }
        vector<INISectorValue>::iterator p2 = publicSec.value.begin();
        for(p2 = publicSec.value.begin(); p2 != publicSec.value.end(); ++p2) {
            if(p2->key.compare(name) == 0)
                if(p2->value.compare("true") == 0 || atoi(p2->value.c_str()) != 0) {
                    return true;
                }
                else
                    return false;
        }
        return bDefault;
    }
    
    void SoraINIFile::writeInt(const SoraString& section, const SoraString& name, int32 iValue) {
        bFileWrite = true;
        if(section.compare("public") == 0)
            publicSec.value.push_back(INISectorValue(name, int_to_str(iValue)));
        
        vector<INISector>::iterator p = sec.begin();
        
        bool bFindSection = false;
        bool bFindValue = false;
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                bFindSection = true;
                
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        bFindValue = true;
                        
                        p2->value = int_to_str(iValue);
                        
                        //					Commit();
                        return;
                    }
                }
                
                if(!bFindValue) {
                    p->value.push_back(INISectorValue(name, int_to_str(iValue)));
                    
                    //				Commit();
                    return;
                }
            }
        }
        
        if(!bFindSection) {
            INISector tsec;
            
            tsec.name = section;
            tsec.value.push_back(INISectorValue(name, int_to_str(iValue)));
            sec.push_back(tsec);
            
            //		Commit();
            return;
        }		
    }
    
    void SoraINIFile::writeFloat(const SoraString& section, const SoraString& name, float fValue) {
        bFileWrite = true;
        if(section.compare("public") == 0)
            publicSec.value.push_back(INISectorValue(name, fp_to_str(fValue)));
        
        vector<INISector>::iterator p = sec.begin();
        
        bool bFindSection = false;
        bool bFindValue = false;
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                bFindSection = true;
                
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        bFindValue = true;
                        
                        p2->value = fp_to_str(fValue);
                        
                        //					Commit();
                        return;
                    }
                }
                
                if(!bFindValue) {
                    p->value.push_back(INISectorValue(name, fp_to_str(fValue)));
                    
                    //				Commit();
                    return;
                }
            }
        }
        
        if(!bFindSection) {
            INISector tsec;
            
            tsec.name = section;
            tsec.value.push_back(INISectorValue(name, fp_to_str(fValue)));
            sec.push_back(tsec);
            
            //		Commit();
            return;
        }	
    }
    
    void SoraINIFile::writeString(const SoraString& section, const SoraString& name, const SoraString& sValue) {
        bFileWrite = true;
        if(section.compare("public") == 0)
            publicSec.value.push_back(INISectorValue(name, sValue));
        
        vector<INISector>::iterator p = sec.begin();
        
        bool bFindSection = false;
        bool bFindValue = false;
        for(p = sec.begin(); p != sec.end(); ++p) {
            if(p->name.compare(section) == 0) {
                bFindSection = true;
                
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    if(p2->key.compare(name) == 0) {
                        bFindValue = true;
                        
                        p2->value = sValue;
                        
                        //				Commit();
                        return;
                    }
                }
                
                if(!bFindValue) {
                    p->value.push_back(INISectorValue(name, sValue));
                    
                    //			Commit();
                    return;
                }
            }
        }
        
        if(!bFindSection) {
            INISector tsec;
            
            tsec.name = section;
            tsec.value.push_back(INISectorValue(name, sValue));
            sec.push_back(tsec);
            
            //	Commit();
            return;
        }	
    }
    
    void SoraINIFile::Commit() {
        if(bMemoryFile) return;
        
        ofstream file(ws2s(sFile).c_str());
        if(file.is_open()) {
            vector<INISectorValue>::iterator ppb = publicSec.value.begin();
            for(; ppb != publicSec.value.end(); ++ppb)
                file<<ppb->key<<"="<<ppb->value<<endl;
            
            vector<INISector>::iterator p = sec.begin();
            for(p = sec.begin(); p != sec.end(); ++p) {
                file<<"["<<p->name<<"]"<<endl;
                
                vector<INISectorValue>::iterator p2 = p->value.begin();
                for(p2 = p->value.begin(); p2 != p->value.end(); ++p2) {
                    file<<p2->key<<"="<<p2->value<<endl;
                }
                
                file<<endl;
            }
            bFileWrite = false;
        }
        
        file.close();
    }
	
    vector<INISectorValue> SoraINIFile::getSector(const SoraString& section) {
        if(section.compare("public") == 0)
            return publicSec.value;
        
        itSector s = GetSectorByName(section);
        if(s != sec.end()) return s->value;
        vector<INISectorValue> ns;
        return ns;
    }
	
    vector<INISector>::iterator SoraINIFile::GetSectorByName(const SoraString& section) {
        itSector s = sec.begin();
        while( s != sec.end() ) {
            if(s->name.compare(section) == 0) return s;
            ++s;
        }
        return sec.end();
    }

} // namespace sora