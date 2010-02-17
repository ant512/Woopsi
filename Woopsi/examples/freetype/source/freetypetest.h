#ifndef _FREETYPE_TEST_H_
#define _FREETYPE_TEST_H_

#include "woopsi.h"
#include "freetypefont.h"
#include "freetypefacemanager.h"
#include "freetypecache.h"

using namespace WoopsiUI;

class FreeTypeTest : public Woopsi {
public:
	void startup();
	void shutdown();
	
private:
	FreeTypeFaceManager* _faceManager; 
	FreeTypeCache* _faceCache;
};

#endif
