// The game selects its widescreen camera aspect in FUN_0015A960: the 16:9 branch loads the float at 0x289A18
// (1.7777778) and stores it in the view's aspect field; the 4:3 branch loads 1.3333334. Redirect that one load
// to our own 1.6 float. The wide UI flag and the 16:9 layout selection are unchanged: 16:10 reuses the wide UI
// and changes only the projection/render shape.
#include "aspect.h"

#include "common.h"

#include <cstring>

namespace {

constexpr uint32_t kSite = 0x0015A987;
// movss xmm0, dword ptr ds:[0x289A18]
const uint8_t kExpect[8] = {0xF3, 0x0F, 0x10, 0x05, 0x18, 0x9A, 0x28, 0x00};

float g_aspect = 16.0f / 10.0f;

} // namespace

bool install_aspect_patch()
{
    if (g_cfg.aspect != Aspect::Wide16x10) return true;
    if (memcmp((const void *)(uintptr_t)kSite, kExpect, sizeof kExpect) != 0) {
        logf("aspect: code at 0x%08x does not match this build; picture stays 16:9", kSite);
        return false;
    }
    uint32_t target = (uint32_t)(uintptr_t)&g_aspect;
    memcpy((void *)(uintptr_t)(kSite + 4), &target, 4); // the load's address operand
    logf("aspect: camera aspect 16:10 (1.6), wide UI layouts kept");
    return true;
}
