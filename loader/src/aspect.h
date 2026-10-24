// Native 16:10: the game's widescreen projection uses a 16:9 camera aspect. For 16:10 the loader redirects
// that one load to a 1.6 float, so the picture gains height instead of being stretched.
#pragma once

// After the XBE is mapped. Returns false when 16:10 was requested but the game code did not match this build;
// the caller then falls back to 16:9. Returns true for 4:3 and 16:9.
bool install_aspect_patch();
