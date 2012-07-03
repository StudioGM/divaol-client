/*
 *  SoraINIFile.h
 *  OpenGLES Study 1
 *
 *  Created by Robert Bu on 09��8��30��.
 *  Copyright 2009 Robert Bu. All rights reserved.
 *
 */
#ifndef _SoraINI_H_
#define _SoraINI_H_

#include <iostream>
#include <string>
#include <vector>

#include <cstdio>

#include "SoraPlatform.h"
#include "SoraException.h"
#include "io/SoraFileBase.h"

namespace sora {

using std::vector;
using std::ifstream;
using std::ofstream;
using std::endl;

struct INISectorValue {
	SoraString key;
	SoraString value;
	
	INISectorValue(const SoraString& _key, const SoraString& _value): key(_key), value(_value) {} 
};

struct INISector {
	vector<INISectorValue> value;
	SoraString name;
};

class SORA_API SoraINIFile: public SoraFileBase {
public:
	typedef vector<INISector>::iterator itSector;

	SoraINIFile();
	~SoraINIFile();
	
	int32 readFile(const SoraWString& path);
	int32 readFileMem(void* ptr, uint32 size);
	void closeFile();
	
	int32		getInt(const SoraString& section, const SoraString& name, int32 iDefault=0);
	SoraString  getString(const SoraString& section, const SoraString& name, const SoraString& sDefault);
	float		getFloat(const SoraString& section, const SoraString& name, float fDefault=0.f);
	bool		getBool(const SoraString& section, const SoraString& name, bool bDefault=false);
	
	void writeInt(const SoraString& section, const SoraString& name, int32 iValue);
	void writeString(const SoraString& section, const SoraString& name, const SoraString& sValue);
	void writeFloat(const SoraString& section, const SoraString& name, float fValue);
	
	vector<INISectorValue> getSector(const SoraString& section);
	
	void Commit();

private:	
	itSector GetSectorByName(const SoraString& section);
	
	SoraWString sFile;
	bool bFileWrite;
	bool bMemoryFile;
	
	vector<INISector> sec;
	INISector publicSec;
};

} //namespace sora

#endif
