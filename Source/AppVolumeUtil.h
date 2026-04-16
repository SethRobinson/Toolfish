#pragma once

#include <tchar.h>

// Set (or adjust) the per-app volume for the current foreground window's process.
// volume:     0-100 for absolute mode, -100 to +100 for relative mode
// relative:   if true, 'volume' is added to the current level
// appNameOut: if non-NULL, receives the process name (e.g. "firefox"). Buffer must be >= appNameSize TCHARs.
// appNameSize: size of appNameOut buffer in TCHARs
// Returns the resulting volume percentage (0-100), or -1 on failure.
int SetActiveWindowVolume(int volume, bool relative, TCHAR* appNameOut = NULL, int appNameSize = 0);
