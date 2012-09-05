/*
 *  GlobalValuesExporter.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "EnvVarExport.h"

namespace sora {

	void lua_export_env_val(LuaPlus::LuaState* state) {
		gge::LuaClass<SoraEnvValues>(state, "EnvVar", SoraEnvValues::Instance())
			.def("getBool",		&SoraEnvValues::getBool)
			.def("getInt",		&SoraEnvValues::getInt)
			.def("getFloat",	&SoraEnvValues::getFloat)
			.def("getString",	&SoraEnvValues::getString)
			.def("setBool",		&SoraEnvValues::setBool)
			.def("setInt",		&SoraEnvValues::setInt)
			.def("setFloat",	&SoraEnvValues::setFloat)
			.def("setString",	&SoraEnvValues::setString)
			.def("setUserData",	&SoraEnvValues::setData)
			.def("getUserData",	&SoraEnvValues::getData);
	}
	
}