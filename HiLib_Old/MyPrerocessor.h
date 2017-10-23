#pragma once

#define NAMED_STRUCT_ARG_METHOD(cls,type,strct,prop) \
	cls& prop(type prop){strct.##prop=prop;return *this;}\
	type prop()const{return strct.##prop;}
#define NAMED_STRUCT_ARG_STRCPY_METHOD(cls,type,strct,prop) cls& prop(type prop){_tcscpy_s(strct.##prop,prop);return *this;}

