#ifndef _ALERT_TEST_H_
#define _ALERT_TEST_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "gadgeteventargs.h"
#include "button.h"
#include "alert.h"

using namespace WoopsiUI;

class AlertTest : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();
	
	void handleClickEvent(const GadgetEventArgs& e);
	void handleDragEvent(const GadgetEventArgs& e);
	void handleReleaseEvent(const GadgetEventArgs& e);
	void handleReleaseOutsideEvent(const GadgetEventArgs& e);
	void handleKeyPressEvent(const GadgetEventArgs& e);
	void handleKeyReleaseEvent(const GadgetEventArgs& e);
	void handleLidOpenEvent(const GadgetEventArgs& e);
	void handleLidCloseEvent(const GadgetEventArgs& e);
	void handleFocusEvent(const GadgetEventArgs& e);
	void handleBlurEvent(const GadgetEventArgs& e);
	void handleCloseEvent(const GadgetEventArgs& e);
	void handleHideEvent(const GadgetEventArgs& e);
	void handleShowEvent(const GadgetEventArgs& e);
	void handleEnableEvent(const GadgetEventArgs& e);
	void handleDisableEvent(const GadgetEventArgs& e);
	void handleValueChangeEvent(const GadgetEventArgs& e);
	void handleResizeEvent(const GadgetEventArgs& e);
	void handleMoveEvent(const GadgetEventArgs& e);
	void handleScrollEvent(const GadgetEventArgs& e);
	void handleShiftClickEvent(const GadgetEventArgs& e);
	void handleContextMenuSelectionEvent(const ContextMenuEventArgs& e);
	void handleDoubleClickEvent(const GadgetEventArgs& e);
	void handleShelveEvent(const GadgetEventArgs& e);
	void handleUnshelveEvent(const GadgetEventArgs& e);
	void handleActionEvent(const GadgetEventArgs& e);
	void handleMoveForwardEvent(const GadgetEventArgs& e);
	void handleMoveBackwardEvent(const GadgetEventArgs& e);
	
private:
	Button* _shelve;
	Button* _unshelve;
	Button* _hide;
	Button* _show;
	Button* _move;
	Button* _resize;
	Button* _autosize;
	Button* _enable;
	Button* _disable;
	Button* _dimensions;
	Alert* _alert;
};

#endif
