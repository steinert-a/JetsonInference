#pragma once

#define DELETE_POINTER(p) {if(p){delete (p);(p)=nullptr;}}
#define DELETE_ARRAY(p) {if(p){delete[] (p);(p)=nullptr;}}

#define RELEASE_POINTER(p) {if(p){(p)->Release();(p)=nullptr;}}
#define DESTROY_POINTER(p) {if(p){(p)->destroy();(p)=nullptr;}}

#define FOR_EACH(it,stl) for(auto it = ((stl).begin()); it != ((stl).end()); it++)
#define FOR_EACH_REVERSE(it,stl) for(auto it = ((stl).rbegin()); it != ((stl).rend()); it++)

// support macros
#define VERSION_NAME(arg) #arg								// return string macro name
#define VERSION_VALUE(arg) VERSION_NAME(arg)				// return string macro value

// version [MAJOR].[MINOR].[REVISION]
#define VERSION_MAJOR		1								// compatibility
#define VERSION_MINOR		0								// feature / functional add on
#define VERSION_REVISION	0								// bugfix

#define VERSION_STRING		VERSION_VALUE(VERSION_MAJOR) "." VERSION_VALUE(VERSION_MINOR) "." VERSION_VALUE(VERSION_REVISION)

