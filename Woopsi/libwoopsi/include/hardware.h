#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <nds.h>
#include <graphics.h>

#include "framebuffer.h"
#include "pad.h"
#include "stylus.h"

namespace WoopsiUI {
	/**
	 * Provides an interface to the DS' hardware.
	 */
	class Hardware {
	public:

		/**
		 * Initialise the hardware.
		 */
		static void init();

		/**
		 * Shutdown the hardware.
		 */
		static void shutdown();

		/**
		 * Get a pointer to a Graphics object that can draw to the top screen.
		 * @return A pointer to a Graphics object that can draw to the top screen.
		 */
		static inline WoopsiUI::Graphics* getTopGfx() {
			return _topGfx;
		};

		/**
		 * Get a pointer to a Graphics object that can draw to the bottom screen.
		 * @return A pointer to a Graphics object that can draw to the bottom
		 * screen.
		 */
		static inline WoopsiUI::Graphics* getBottomGfx() {
			return _bottomGfx;
		};

		/**
		 * Get a reference to an object representing the state of the DS' buttons.
		 *@return A reference to the current pad state.
		 */
		static inline const Pad& getPad() {
			return _pad;
		};

		/**
		 * Get a reference to an object representing the state of the DS' stylus.
		 *@return A reference to the current stylus state.
		 */
		static inline const Stylus& getStylus() {
			return _stylus;
		};

		/**
		 * Waits for the next VBlank.  Also updates the pad/stylus states.
		 */
		static void waitForVBlank();

		/**
		 * Get a pointer to the FrameBuffer object that wraps around the top
		 * frame buffer VRAM.
		 * @return A pointer to a FrameBuffer object that wraps around the top
		 * frame buffer VRAM.
		 */
		static inline FrameBuffer* getTopBuffer() { return _topBuffer; };

		/**
		 * Get a pointer to the FrameBuffer object that wraps around the bottom
		 * frame buffer VRAM.
		 * @return A pointer to a FrameBuffer object that wraps around the bottom
		 * frame buffer VRAM.
		 */
		static inline FrameBuffer* getBottomBuffer() { return _bottomBuffer; };

	private:
		static Pad _pad;						/**< State of the DS' pad. */
		static Stylus _stylus;					/**< State of the DS' stylus. */
		static FrameBuffer* _topBuffer;         /**< Top frame buffer. */
		static FrameBuffer* _bottomBuffer;      /**< Bottom frame buffer. */
		static Graphics* _topGfx;				/**< Top display graphics object. */
		static Graphics* _bottomGfx;			/**< Bottom display graphics object. */

#ifdef USING_SDL

		static SDL_Window* _window;
		static SDL_Renderer* _renderer;
		static SDL_Texture* _texture;
		static u16* _topBitmap;
		static u16* _bottomBitmap;

#endif

		/**
		 * Constructor.
		 */
		Hardware() { };
		
		/**
		 * Destructor.
		 */
		~Hardware() { };
	};
}

#endif
