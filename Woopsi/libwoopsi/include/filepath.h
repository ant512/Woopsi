#ifndef _FILE_PATH_H_
#define _FILE_PATH_H_

#include "woopsistring.h"

namespace WoopsiUI {

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

	private:
		WoopsiString _path;
	};
}

#endif
