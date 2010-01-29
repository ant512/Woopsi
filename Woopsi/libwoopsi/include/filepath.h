#ifndef _FILE_PATH_H_
#define _FILE_PATH_H_

#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * Class designed to simplify parsing path strings.  If told to append the
	 * current directory to its current path, the path will not change.  If told
	 * to append the parent directory, the current directory is stripped from
	 * the path.  The path string is therefore always as short as possible.
	 *
	 * Used by the FileListBox.
	 */
	class FilePath {
	public:
		
		/**
		 * Constructor.
		 * @param path Initial path.
		 */
		FilePath(const WoopsiString& path);

		/**
		 * Destructor.
		 */
		inline ~FilePath() { }

		/**
		 * Set the path.
		 * @param path The new path.
		 */
		void setPath(const WoopsiString& path);

		/**
		 * Get the path.
		 * @return The path.
		 */
		inline const WoopsiString& getPath() const { return _path; };

		/**
		 * Append a new path component to the current path.  Automatically
		 * inserts trailing slash.
		 * @param path Path to add to the end of the current path.
		 */
		void appendPath(const WoopsiString& path);

		/**
		 * Move from the current directory to its parent, if possible.
		 */
		void goToParent();

		/**
		 * Go to the root of the filesystem.
		 */
		void goToRoot();

	private:
		WoopsiString _path;			/** String containing the path. */
	};
}

#endif
