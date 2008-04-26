#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include "woopsi.h"

using namespace std;

class HelloWorld : public Woopsi {
public:
	void startup();
	void shutdown();
};

#endif
