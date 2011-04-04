#include <nds.h>
#include "gadget.h"
#include "scrollingpanel.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Subclass of the ScrollingPanel that draws a rectangle and some text to the display
	 * and allows it to be dragged around.
	 */
	class TestPanel : public ScrollingPanel {
	public:

		/**
		 * Constructor.
		 * @param x X co-ord of the panel.
		 * @param y Y co-ord of the panel.
		 * @param width Width of the panel.
		 * @param height Height of the panel.
		 * @param font Font to use with the panel.
		 */
		TestPanel(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style = NULL);
		
	protected:
		
		/**
		 * Draw the test panel.
		 * @param port GraphicsPort to draw to.
		 */
		void drawContents(GraphicsPort* port);
	};
}
