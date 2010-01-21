#ifndef _FILE_LISTBOX_H_
#define _FILE_LISTBOX_H_

#include "scrollinglistbox.h"
#include "filelistboxdataitem.h"
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "gadgetstyle.h"
#include "woopsistring.h"

#ifdef ARM9
#include <fat.h>
#endif

namespace WoopsiUI {

	class FilePath;

	/**
	 * Class providing a listbox listing files.  Designed to allow users
	 * selection a file from the filesytem.  When a file is selected the
	 * requester will automatically close.
	 *
	 * To read the value of the selected option or options, you should listen
	 * for the value changed event.  This will fire when the user double-clicks
	 * an option or clicks the OK button.
	 *
	 * When using this class, ensure you call "fatInitDefault();" somewhere
	 * in your setup code.
	 */
	class FileListBox : public Gadget, public GadgetEventHandler  {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the listbox.
		 * @param y The y co-ordinate of the listbox.
		 * @param width The width of the listbox.
		 * @param height The height of the listbox.
		 * @param flags Standard flags.
		 * @param style Optional gadget style.
		 */
		FileListBox(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style = NULL);
	
		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event arguments.
		 */
		virtual void handleDoubleClickEvent(const GadgetEventArgs& e);

		/**
		 * Add a new option to the gadget using default colours.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 */
		virtual inline void addOption(const char* text, const u32 value) {
			_listbox->addOption(text, value);
		};

		/**
		 * Add a new option to the gadget.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 * @param normalTextColour Colour to draw the text with when not
		 * selected.
		 * @param normalBackColour Colour to draw the background with when not
		 * selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when
		 * selected.
		 */
		virtual inline void addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
			_listbox->addOption(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour);
		};

		/**
		 * Remove an option from the gadget by its index.
		 * @param index The index of the option to remove.
		 */
		virtual inline void removeOption(const s32 index) {
			_listbox->removeOption(index);
		};

		/**
		 * Remove all options from the gadget.
		 */
		virtual inline void removeAllOptions() {
			_listbox->removeAllOptions();
		};

		/**
		 * Get the selected index.  Returns -1 if nothing is selected.  If more than one
		 * option is selected, the index of the first selected option is returned.
		 * @return The selected index.
		 */
		virtual inline const s32 getSelectedIndex() const {
			return _listbox->getSelectedIndex();
		};

		/**
		 * Get the selected option.  Returns NULL if nothing is selected.
		 * @return The selected option.
		 */
		virtual inline const FileListBoxDataItem* getSelectedOption() const {
			return (const FileListBoxDataItem*)_listbox->getSelectedOption();
		};

		/**
		 * Sets whether multiple selections are possible or not.
		 * @param allowMultipleSelections True to allow multiple selections.
		 */
		virtual inline void setAllowMultipleSelections(const bool allowMultipleSelections) {
			_listbox->setAllowMultipleSelections(allowMultipleSelections);
		};

		/**
		 * Get the specified option.
		 * @return The specified option.
		 */
		virtual inline const FileListBoxDataItem* getOption(const s32 index) const {
			return (const FileListBoxDataItem*)_listbox->getOption(index);
		};

		/**
		 * Sort the options alphabetically by the text of the options.
		 */
		virtual inline void sort() {
			_listbox->sort();
		};

		/**
		 * Get the total number of options.
		 * @return The number of options.
		 */
		virtual inline const s32 getOptionCount() const {
			return _listbox->getOptionCount();
		};

		/**
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

		/**
		 * Set the displayed path.
		 * @param path The new path.
		 */
		virtual void setPath(const WoopsiString& path);

		/**
		 * Append a new path component to the current path.  Automatically
		 * inserts trailing slash.
		 */
		virtual void appendPath(const WoopsiString& path);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

	protected:
		ScrollingListBox* _listbox;			/**< Pointer to the list box */
		FilePath* _path;					/**< Path currently displayed */

		/**
		 * Destructor.
		 */
		virtual ~FileListBox();

		/**
		 * Populate list with directory data.
		 */
		virtual void readDirectory();
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline FileListBox(const FileListBox& fileListBox) : Gadget(fileListBox) { };
	};
}

#endif
