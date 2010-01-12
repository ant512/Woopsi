#include "filepath.h"
#include "stringiterator.h"

using namespace WoopsiUI;

FilePath::FilePath(const WoopsiString& path) {
	setPath(path);
}

void FilePath::setPath(const WoopsiString& path) {
	_path = path;
}

void FilePath::appendPath(const WoopsiString& path) {

	if (_path.getLength() == 0) setPath(path);

	// Abort if path is current directory
	if ((path.getCharAt(0) == '.') && (path.getLength() == 1)) return;

	// Handle parent directory
	if ((path.getCharAt(0) == '.') && (path.getCharAt(1) == '.') && (path.getLength() == 2)) {

		// Abort if we're at the root directory already
		if (_path.getLength() == 1) return;

		// Locate start of the previous directory in the path string
		// by moving backwards along the string hunting slashes
		StringIterator* iterator = _path.newStringIterator();
		iterator->moveTo(_path.getLength() - 2);

		do {
			if (iterator->getCodePoint() == '/') {

				// Found a slash; remove everything after it
				_path.remove(iterator->getIndex() + 1);
			}
		} while (iterator->moveToPrevious());

		delete iterator;

		return;
	}

	// Append valid path to existing path
	_path.append(path);
	_path.append("/");
}
