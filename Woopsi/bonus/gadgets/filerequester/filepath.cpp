#include <string.h>
#include "filepath.h"

using namespace WoopsiUI;

FilePath::FilePath(const char* path) {
	_path = NULL;
	setPath(path);
}

void FilePath::setPath(const char* path) {

	// Delete existing path memory
	if (_path != NULL) {
		delete [] _path;
	}

	// Set new path
	_path = new char[strlen(path) + 1];

	strcpy(_path, path);
}

void FilePath::appendPath(const char* path) {

	// Ensure path exists
	if (_path == NULL) {
		setPath(path);
		return;
	}

	// Abort if path is current directory
	if (strcmp(path, ".") == 0) return;

	// Handle parent directory
	if (strcmp(path, "..") == 0) {

		// Abort if we're at the root directory already
		if (strlen(_path) == 1) return;

		// Locate start of the previous directory in the path string
		// by moving backwards along the string hunting slashes
		char* lastSlash = _path + strlen(_path) - 2;
		while (*lastSlash != '/') {
			lastSlash--;
		}

		// Append terminator after previous slash to truncate string
		lastSlash[1] = '\0';

		return;
	}

	// Append valid path to existing path

	// Create new path memory
	char* newPath = new char[strlen(path) + strlen(_path) + 2];

	// Concat strings
	strcpy(newPath, _path);
	strcat(newPath, path);
	strcat(newPath, "/");

	// Delete old path
	delete [] _path;

	// Update pointer
	_path = newPath;
}
