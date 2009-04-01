#include <stdlib.h>
#include "calculator.h"
#include "amigawindow.h"
#include "amigascreen.h"
#include "textbox.h"
#include "button.h"

Calculator::Calculator(AmigaScreen* screen) {
	_screen = screen;
	_opCode = 0;
	_val1 = 0;
	_val2 = 0;
	_text = NULL;
	_wipeNeeded = false;

	initGUI();
}

void Calculator::initGUI() {
	_window = new AmigaWindow(0, 90, 60, 97, "Calc", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	_screen->addGadget(_window);

	Gadget::Rect rect;
	_window->getClientRect(rect);

	// Controls
	_output = new TextBox(rect.x, rect.y, 52, 16, "0");
	_output->setTextAlignmentHoriz(TextBox::TEXT_ALIGNMENT_HORIZ_RIGHT);
	_window->addGadget(_output);

	WoopsiArray<Button*> buttons;
	buttons.push_back(new Button(rect.x, rect.y + 16, 13, 16, "7"));
	buttons.push_back(new Button(rect.x + 13, rect.y + 16, 13, 16, "8"));
	buttons.push_back(new Button(rect.x + 26, rect.y + 16, 13, 16, "9"));
	buttons.push_back(new Button(rect.x + 39, rect.y + 16, 13, 16, "*"));

	buttons.push_back(new Button(rect.x, rect.y + 32, 13, 16, "4"));
	buttons.push_back(new Button(rect.x + 13, rect.y + 32, 13, 16, "5"));
	buttons.push_back(new Button(rect.x + 26, rect.y + 32, 13, 16, "6"));
	buttons.push_back(new Button(rect.x + 39, rect.y + 32, 13, 16, "-"));

	buttons.push_back(new Button(rect.x, rect.y + 48, 13, 16, "1"));
	buttons.push_back(new Button(rect.x + 13, rect.y + 48, 13, 16, "2"));
	buttons.push_back(new Button(rect.x + 26, rect.y + 48, 13, 16, "3"));
	buttons.push_back(new Button(rect.x + 39, rect.y + 48, 13, 16, "+"));

	buttons.push_back(new Button(rect.x, rect.y + 64, 13, 16, "0"));
	buttons.push_back(new Button(rect.x + 13, rect.y + 64, 13, 16, "C"));
	buttons.push_back(new Button(rect.x + 26, rect.y + 64, 13, 16, "="));
	buttons.push_back(new Button(rect.x + 39, rect.y + 64, 13, 16, "/"));

	// Wire up events
	for (u8 i = 0; i < buttons.size(); i++) {
		buttons[i]->addGadgetEventHandler(this);
		_window->addGadget(buttons[i]);
	}
}

void Calculator::appendText(const char* text) {
	if ((strcmp(_output->getText(), "0") == 0) || (_wipeNeeded)) {
		_wipeNeeded = false;
		_output->setText(text);
	} else {
		if (strlen(_output->getText()) < 5) {
			_output->appendText(text);
		}
	}

	_output->draw();
}

void Calculator::doAdd() {
	if (_opCode == 0) {
		_opCode = 1;
		_val1 = atoi(_output->getText());
		_output->setText("0");
	} else {
		_val2 = atoi(_output->getText());

		doFunction();
		_opCode = 1;

		updateDisplay();
	}
}

void Calculator::doSubtract() {
	if (_opCode == 0) {
		_opCode = 2;
		_val1 = atoi(_output->getText());
		_output->setText("0");
	} else {
		_val2 = atoi(_output->getText());

		doFunction();
		_opCode = 2;

		updateDisplay();
	}
}

void Calculator::doMultiply() {
	if (_opCode == 0) {
		_opCode = 3;
		_val1 = atoi(_output->getText());
		_output->setText("0");
	} else {
		_val2 = atoi(_output->getText());

		doFunction();
		_opCode = 3;

		updateDisplay();
	}
}

void Calculator::doDivide() {
	if (_opCode == 0) {
		_opCode = 4;
		_val1 = atoi(_output->getText());
		_output->setText("0");
	} else {
		_val2 = atoi(_output->getText());

		doFunction();
		_opCode = 4;

		updateDisplay();
	}
}

void Calculator::doEquals() {
	switch (_opCode) {
		case 0:
			break;
		case 1:
			doAdd();
			break;
		case 2:
			doSubtract();
			break;
		case 3:
			doMultiply();
			break;
		case 4:
			doDivide();
			break;
	}

	_opCode = 0;
}

void Calculator::updateDisplay() {

	// Delete old text
	if (_text != NULL) {
		delete[] _text;
	}

	// Allocate new text
	_text = new char[countDigits(_val1) + 1];

	// Get text
	itoa(_val1, _text);
	
	_output->setText(_text);
	_output->draw();
}

void Calculator::doFunction() {
	switch (_opCode) {
		case 1:
			_val1 += _val2;
			_val2 = 0;
			break;
		case 2:
			_val1 -= _val2;
			_val2 = 0;
			break;
		case 3:
			_val1 *= _val2;
			_val2 = 0;
			break;
		case 4:
			_val1 /= _val2;
			_val2 = 0;
			break;
		default:
			break;
	}
}

// Count the number of digits in an int
u8 Calculator::countDigits(s32 number) {
	u8 digits = 1;

	while (number > 9) {
		number /= 10;
		digits++;
	}

	return digits;
}

/* reverse:  reverse string s in place */
void Calculator::reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void Calculator::itoa(s32 n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

bool Calculator::handleEvent(const GadgetEventArgs& e) {
	switch (e.getType()) {
		case EVENT_CLICK:
			handleClick(e);
			return true;
		default:
			return false;
	}
}

void Calculator::handleClick(const GadgetEventArgs& e) {
	Button* button = (Button*)e.getSource();

	if (strcmp(button->getText(), "=") == 0) {
		doEquals();
		_wipeNeeded = true;
	} else if (strcmp(button->getText(), "C") == 0) {

		_output->setText("0");
		_val1 = 0;
		_val2 = 0;
		_opCode = 0;
		_output->draw();

	} else if (strcmp(button->getText(), "+") == 0) {
		doAdd();
		_wipeNeeded = true;
	} else if (strcmp(button->getText(), "-") == 0) {
		doSubtract();
		_wipeNeeded = true;
	} else if (strcmp(button->getText(), "*") == 0) {
		doMultiply();
		_wipeNeeded = true;
	} else if (strcmp(button->getText(), "/") == 0) {
		doDivide();
		_wipeNeeded = true;
	} else {
		appendText(button->getText());
	}
}
