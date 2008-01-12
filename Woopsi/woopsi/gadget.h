#ifndef _GADGET_H_
#define _GADGET_H_

#include <nds.h>
#include <vector>
#include "eventhandler.h"
#include "debug.h"
#include "glyphs.h"
#include "defines.h"
#include "fontbase.h"

using namespace std;

class GraphicsPort;

class Gadget {
public:
	enum OutlineType {
		OUTLINE_CLICK_DEPENDENT = 0,
		OUTLINE_OUT = 1,
		OUTLINE_IN = 2
	};

	enum CloseType {
		CLOSE_TYPE_CLOSE = 0,
		CLOSE_TYPE_HIDE = 1
	};

	enum GadgetFlagType {
		GADGET_BORDERLESS = 0x0001,
		GADGET_CLOSEABLE = 0x0002,
		GADGET_DRAGGABLE = 0x0004,
		GADGET_PERMEABLE = 0x0008
	};

	typedef struct {
		s16 x;
		s16 y;
		s32 width;
		s32 height;
	} Rect;

	typedef struct {
		u8 clicked : 1;
		u8 active : 1;
		u8 dragging : 1;
		u8 deleted : 1;
		u8 borderless : 1;
		u8 draggable : 1;
		u8 visible : 1;
		u8 closeable : 1;
		u8 enabled : 1;
		u8 decoration : 1;
		u8 permeable : 1;
	} Flags;

	Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font = NULL);
	virtual ~Gadget();

	s16 getX();
	s16 getY();
	u32 getRefcon();
	CloseType getCloseType();
	bool isActive();
	bool isDeleted();
	bool isVisible();
	bool isDecoration();
	bool isPermeable();
	bool isBorderless();
	bool isClicked();
	u16 getWidth();
	u16 getHeight();
	Gadget* getParent();
	Gadget* getActiveGadget();
	u8 getPhysicalScreenNumber();
	virtual void getClientRect(Rect& rect);
	void getRectClippedToHierarchy(Rect& rect);
	virtual GraphicsPort* newGraphicsPort();
	virtual GraphicsPort* newGraphicsPort(Rect clipRect);
	vector<Rect>* getVisibleRectCache();
	FontBase* getFont();

	const u16 getBackColour() const;
	const u16 getShineColour() const;
	const u16 getHighlightColour() const;
	const u16 getShadowColour() const;
	const u16 getFillColour() const;
	const OutlineType getOutlineType() const;

	u32 setRefcon(u32 refcon);
	void setBorderless(bool isBorderless);
	void setDraggable(bool isDraggable);
	void setPermeable(bool isPermeable);
	void setOutlineType(OutlineType outline);
	void setEventHandler(EventHandler* eventHandler);
	void setVisible(bool visible);
	void setBackColour(u16 colour);
	void setShineColour(u16 colour);
	void setHighlightColour(u16 colour);
	void setShadowColour(u16 colour);
	void setFillColour(u16 colour);
	void setCloseType(CloseType closeType);
	virtual void setFont(FontBase* font);

	virtual void draw();
	virtual void erase();

	virtual bool enable();
	virtual bool disable();

	virtual void close();
	virtual bool hide();
	virtual bool show();

	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool vbl();
	virtual bool keyPress(KeyCode keyCode);
	virtual bool keyRelease(KeyCode keyCode);
	virtual void lidClosed();
	virtual void lidOpened();
	virtual bool focus();
	virtual bool blur();

	virtual bool moveTo(s16 x, s16 y);
	virtual bool resize(u16 width, u16 height);
	virtual bool raiseToTop();
	virtual bool lowerToBottom();
	virtual bool raiseGadgetToTop(Gadget* gadget);
	virtual bool lowerGadgetToBottom(Gadget* gadget);

	// These should be limited to friends
	void moveChildToDeleteQueue(Gadget* gadget);
	bool moveChildToHiddenList(Gadget* gadget);
	bool moveHiddenToChildList(Gadget* gadget);
	virtual void setActiveGadget(Gadget* gadget);
	virtual void setClickedGadget(Gadget* gadget);
	bool checkCollision(s16 x, s16 y);
	bool checkCollision(s16 x, s16 y, u16 width, u16 height);
	bool checkCollision(Gadget* gadget);
	void invalidateLowerGadgetsVisibleRectCache(Gadget* gadget);

	void unregisterChildrenFromVBL();

	void addGadget(Gadget* gadget);
	void insertGadget(Gadget* gadget);
	void setParent(Gadget* parent);

	void setDragging(u16 x, u16 y);

	void cacheVisibleRects();
	void invalidateVisibleRectCache();

	virtual void draw(Rect clipRect);
	virtual void eraseGadget(Gadget* gadget);
	virtual void redrawDirty(vector<Rect>* invalidRects, Gadget* sender);
	virtual void removeOverlappedRects(vector<Rect>* visibleRects, vector<Rect>* invisibleRects, Gadget* gadget);
	virtual void splitRectangles(vector<Rect>* invalidRectangles, vector<Rect>* validRects, Gadget* sender);
	void clipRectToHierarchy(Rect& rect, Gadget* sender);
	virtual bool swapGadgetDepth(Gadget* gadget);
	bool swapDepth();


protected:
	s16 _x;
	s16 _y;
	u16 _width;
	u16 _height;
	u32 _refcon;

	// Dragging variables
	s16 _grabPointX;
	s16 _grabPointY;
	s16 _newX;
	s16 _newY;

	// Colour definitions
	u16 _backColour;
	u16 _shineColour;
	u16 _highlightColour;
	u16 _shadowColour;
	u16 _fillColour;

	// Status
	Flags _flags;

	// Event handling
	EventHandler* _eventHandler;

	// Hierarchy control
	Gadget* _parent;
	Gadget* _activeGadget;
	Gadget* _clickedGadget;
	vector<Gadget*> _gadgets;
	vector<Gadget*> _hiddenGadgets;

	// Decorations
	u8 _decorationCount;

	// Visible regions
	vector<Rect> _visibleRegionCache;
	bool _visibleRegionCacheInvalid;

	OutlineType _outline;
	CloseType _closeType;

	FontBase* _font;

	// Initialisation
	void init();

	s16 calculatePhysicalScreenY(s16 y);
	u8 calculatePhysicalScreenNumber(s16 y);

	// Drawing tools
	void clear(Rect clipRect);
	void clear();
	void drawChildren();

	s16 getGadgetIndex(Gadget* gadget);

	virtual void setActive(bool active);

	virtual void closeChild(Gadget* gadget);
	virtual void hideChild(Gadget* gadget);

	virtual void redrawDirtyChildren(vector<Rect>* invalidRects, Gadget* sender);

	virtual GraphicsPort* newInternalGraphicsPort();
	virtual GraphicsPort* newInternalGraphicsPort(Rect clipRect);

	// Events
	void raiseClickEvent(s16 x, s16 y);
	void raiseReleaseEvent(s16 x, s16 y);
	void raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY);
	void raiseVBLEvent();
	void raiseKeyPressEvent(KeyCode keyCode);
	void raiseKeyReleaseEvent(KeyCode keyCode);
	void raiseLidClosedEvent();
	void raiseLidOpenedEvent();
	void raiseFocusEvent();
	void raiseBlurEvent();
	void raiseCloseEvent();
	void raiseHideEvent();
	void raiseShowEvent();
	void raiseEnableEvent();
	void raiseDisableEvent();
	void raiseValueChangeEvent();
	void raiseResizeEvent(u16 width, u16 height);
	void raiseMoveEvent(s16 x, s16 y);
};

#endif
