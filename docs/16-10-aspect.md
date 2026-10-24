# Native 16:10

BLACK's own widescreen mode is 16:9. The loader can now also run it at 16:10: the picture keeps the same
horizontal field of view and gains vertical field of view instead of being stretched.

## How it works

- The Xbox reports a widescreen TV through EEPROM `XC_VIDEO` bit 16. The loader keeps reporting it for 16:10,
  so the game still selects its `_wide` UI layouts and its widescreen camera branch.
- The game's widescreen camera branch in `FUN_0015A960` loads `1.7777778f` from `0x289A18` and stores it as the
  view aspect. For 16:10 the loader redirects that single load (at `0x0015A987`, `movss xmm0, [0x289A18]`) to a
  `1.6f` value in the DLL. The 4:3 branch is untouched.
- The view setup (`FUN_000D2F40`) reads that aspect from `[0x2D67F4] + 0xD4C4` and divides the horizontal half
  width by it. With the horizontal FOV unchanged, the vertical FOV grows from about 43 degrees (16:9) to about
  46.4 degrees (16:10), which is the extra height a 16:10 display can show.
- The loader widens its host render targets by `1.2` instead of `4/3` and opens a 16:10 window. `--scale 3`
  therefore renders `2304x1440`; `--resolution 1920x1200` (or `--resolution 1200`) renders at the display's
  native 16:10 size.

## Usage

```
black --aspect 16:10 --resolution 1600x1000
```

or in `black.ini`:

```
widescreen = 1
aspect = 16:10
```

The default remains 16:9, and `--normal-aspect` (or `aspect = 4:3`) still selects 4:3. The in-game
PC SETTINGS > DISPLAY > ASPECT RATIO row offers all three; aspect changes take effect on the next start because
the game reads the video mode at boot.

## Limits

The game's `_wide` UI layouts were authored for 16:9. At 16:10 they are reused unchanged, so UI elements are
slightly narrower than on a 16:9 display. The 3D picture itself is correct: horizontal FOV is unchanged and the
additional vertical view is real geometry. Videos keep the same presentation path as before; check them on the
target display and add movie letterboxing later if needed.
