#ifndef _DEMO_H_
#define _DEMO_H_

#include <alert.h>
#include <woopsi.h>

#define DEMO_VERSION "Woopsi Demo V1.3"

using namespace WoopsiUI;

class PacMan;
class Calculator;
class Pong;

class Demo : public Woopsi {
public:
	void startup();
	void shutdown();

private:
	PacMan* _pacMan;
	Calculator* _calculator;
	Pong* _pong;
	Alert* _alert;
};

#endif
