#ifndef _DEMO_H_
#define _DEMO_H_

#include "woopsi.h"

using namespace std;

class PacMan;
class Calculator;
class Pong;
class Alert;

class Demo : public Woopsi {
public:
	void startup();
	void preLoop();
	void shutdown();

private:
	PacMan* _pacMan;
	Calculator* _calculator;
	Pong* _pong;
	Alert* _alert;
};

#endif
