#ifndef _FILE_PATH_H_
#define _FILE_PATH_H_

namespace WoopsiUI {

	class FilePath {
	public:
		
		/**
		 * Constructor.
		 * @param path Initial path.
		 */
		FilePath(const char* path);

		/**
		 * Destructor.
		 */
		inline ~FilePath() {
			if (_path != NULL) {
				delete [] _path;
			}
		}

		/**
		 * Set the path.
		 * @param path The new path.
		 */
		void setPath(const char* path);

		/**
		 * Get the path.
		 * @return The path.
		 */
		inline const char* getPath() const { return _path; };

		/**
		 * Append a new path component to the current path.  Automatically
		 * inserts trailing slash.
		 * @param path Path to add to the end of the current path.
		 */
		void appendPath(const char* path);

	private:
		char* _path;
	};
}

#endif
