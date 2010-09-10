#ifndef _WOOPSI_KEY_H_
#define _WOOPSI_KEY_H_

#include <nds.h>
#include "stickybutton.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Class providing a key for the WoopsiKeyboard class.
	 */
	class WoopsiKey : public StickyButton {
	public:

		/**
		 * Enum listing all possible types of key
		 */
		enum KeyType {
			KEY_NONE = 0,						/**< No key; for completeness only */
			KEY_ALPHA_NUMERIC_SYMBOL = 1,		/**< Character, number or symbol key */
			KEY_RETURN = 3,						/**< Return/enter key */
			KEY_CONTROL = 4,					/**< Control (Ctrl) key */
			KEY_SHIFT = 5,						/**< Shift key */
			KEY_CAPS_LOCK = 6,					/**< Caps lock key */
			KEY_BACKSPACE = 7,					/**< Backspace (ie delete) key */
			KEY_SPACE = 8						/**< Space bar */
		};

		/**
		 * Enum listing all possible modes a key can be in.  The modes reflect
		 * the keyboard state (ie. which modifiers are held down?), and each key
		 * has a separate string to display for each different mode.
		 * There is no mode for shift and caps lock nor for shift, caps lock and
		 * control, since in those situations the functionality of caps lock
		 * is already provided by the shift key.
		 */
		enum KeyMode {
			KEY_MODE_NORMAL = 0,				/**< No modifiers held */
			KEY_MODE_SHIFT = 1,					/**< Shift key held */
			KEY_MODE_CONTROL = 2,				/**< Control key held */
			KEY_MODE_SHIFT_CONTROL = 3,			/**< Shift and control held */
			KEY_MODE_CAPS_LOCK = 4,				/**< Caps lock held */
			KEY_MODE_CONTROL_CAPS_LOCK = 5		/**< Control and caps lock held */
		};

		/**
		 * Constructor for keys for the keyboard that display a string.
		 * The same text is used regardless of which modifier keys are held
		 * down.
		 * @param x The x co-ordinate of the button, relative to its parent.
		 * @param y The y co-ordinate of the button, relative to its parent.
		 * @param width The width of the key.
		 * @param height The height of the key.
		 * @param text The text for the key to display.
		 * @param keyType The type of key.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		WoopsiKey(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, const KeyType keyType = KEY_ALPHA_NUMERIC_SYMBOL, GadgetStyle* style = NULL);

		/**
		 * Constructor for keys for the keyboard that display a string.  A
		 * different string is supplied for each keyboard modifier situation.
		 * @param x The x co-ordinate of the button, relative to its parent.
		 * @param y The y co-ordinate of the button, relative to its parent.
		 * @param width The width of the key.
		 * @param height The height of the key.
		 * @param normalText The text for the key to display when no modifiers
		 * are held.
		 * @param shiftText The text for the key to display when shift is held.
		 * @param controlText The text for the key to display when control is
		 * held.
		 * @param shiftControlText The text for the key to display when shift
		 * and control are held.
		 * @param capsLockText The text for the key to display when caps lock is
		 * held.
		 * @param controlCapsLockText The text for the key to display when caps
		 * lock and control are held.
		 * @param keyType The type of key.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		WoopsiKey(s16 x, s16 y, u16 width, u16 height, const WoopsiString& normalText, const WoopsiString& shiftText, const WoopsiString& controlText, const WoopsiString& shiftControlText, const WoopsiString& capsLockText, const WoopsiString& controlCapsLockText, const KeyType keyType = KEY_ALPHA_NUMERIC_SYMBOL, GadgetStyle* style = NULL);

		/**
		 * Return the type of key this object represents.
		 * @return The key type.
		 */
		inline const KeyType getKeyType() const { return _keyType; };

		/**
		 * Set the type of this key.
		 * @param keyType The new keytype for this key.
		 */
		inline void setKeyType(KeyType keyType) { _keyType = keyType; };

		/**
		 * Set the mode of the key.  Should only be called by the keyboard
		 * itself.
		 * @param keyMode The new mode for this key.
		 */
		void setKeyMode(KeyMode keyMode);

		/**
		 * Get the value represented by this key.  This is the text that should
		 * (for example) be printed to the screen when the key is clicked.
		 * Modifier keys return the string terminator character.
		 * @return The value of the key
		 */
		const char getValue() const;

	protected:
		KeyType _keyType;					/**< Type of key represented by this object */
		WoopsiString _normalText;			/**< Text displayed when in normal mode */
		WoopsiString _shiftText;			/**< Text displayed when in shift mode */
		WoopsiString _controlText;			/**< Text displayed when in control mode */
		WoopsiString _shiftControlText;		/**< Text displayed when in shift/control mode */
		WoopsiString _capsLockText;			/**< Text displayed when in caps lock mode */
		WoopsiString _controlCapsLockText;	/**< Text displayed when in control/caps lock mode */
		KeyMode _keyMode;					/**< Current mode */

		/**
		 * Destructor.
		 */
		virtual inline ~WoopsiKey() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WoopsiKey(const WoopsiKey& key) : StickyButton(key) { };
	};
}

#endif
