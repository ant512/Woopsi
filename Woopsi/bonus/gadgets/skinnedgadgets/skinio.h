#ifndef _SKIN_IO_H_
#define _SKIN_IO_H_

#include "skin.h"

#ifdef ARM9
#include <fat.h>
#endif

namespace WoopsiUI {

	class Bitmap;

	class SkinIO {
	public:
		static void loadScreenSkin(ScreenSkin& skin, const char* path);
		static void loadWindowSkin(WindowSkin& skin, const char* path);
	private:
		static Bitmap* loadBMP(const char* fileName, const char* path);
		
		/**
		 * Resets all properties of a gadget skin to defaults.
		 * @param skin Skin to reset.
		 */
		static void resetGadgetSkin(GadgetSkin& skin);
	};
};

#endif

