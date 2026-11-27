# Code-quality review: 251028-lcd-refactor vs main

**Result:** `BLOCK` / `REQUEST_CHANGES`  
**Comparison:** `git diff --find-renames main...HEAD` (merge base `a7760e4c`)  
**Evidence status:** `omo ulw-loop status --json` could not run because `bun` and its node fallback are unavailable. Per the required fallback, this report is stored at `.omo/evidence/251028-lcd-refactor-code-review.md`. The inspected diff is non-empty: 59 commits, 165 files, 3,741 additions, and 3,078 deletions.

## Standards

### CRITICAL

None.

### HIGH

- **Scope control — branch contamination.** The stated LCD refactor is accompanied by 149 non-LCD/config files and 47 non-LCD commit subjects, including framework media, BLE, kernel signal/MM, Beken Wi-Fi, and NDP120 work. This makes the claimed feature review, rollback, and regression attribution unsafe. Split/rebase the LCD work onto `main` before approval. Evidence: `git log main..HEAD --oneline`; `git diff --name-only main...HEAD`.
- **Missing-image path double-frees the splash buffer.** In the explicitly non-error `-ENOENT` path, [lcd_dev.c:687](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/lcd_dev.c:687) frees `splash_buffer` at line 690 without clearing it. A later normal put-area call frees that dangling pointer again at [lcd_dev.c:213](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/lcd_dev.c:213). This deterministically corrupts the heap whenever splash is enabled but its resource file is absent.
- **Display-off failures are silently accepted.** [mipi_lcd.c:229](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/mipi_lcd.c:229) ignores the error result of `mipi_dsi_dcs_set_display_off`, powers down, and records `CMD_OFF`. The DSI API documents a negative return on failure (`os/drivers/mipidsi/mipi_dsi_device.c:649-659`), and the former `lcd_power_off` returned that error. The refactor can now report a successful state transition after a failed display command.

### MEDIUM

- **Undefined bitfield conversion.** [image_lib.c:73](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/image_lib.c:73) through line 97 left-shift by `5-r_bits` / `6-g_bits` before checking whether a source mask is wider than RGB565. Valid 16-bit `BI_BITFIELDS` masks with wider channels cause a negative shift, i.e. undefined behavior. Validate/reject those masks or choose the scale direction before shifting.
- **No relevant tests were added or found.** There is no coverage for missing-splash cleanup, DSI display-off failure, state transition/retry failure, or BMP header/mask boundaries. This is meaningful regression risk for the new code; it is not a hard documented-style breach.

### LOW

- **Parser boundary robustness (judgment call).** [image_lib.c:100](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/image_lib.c:100) trusts packed on-disk structures and does not validate `header_size`, `data_offset`, or advertised file size before seeking/decoding. The new parser is in scope for splash support, so this is not needless extraction/parsing, but its boundary validation is incomplete.

No repository coding standard was found beyond `AGENTS.md`, whose referenced issue/domain documents are absent here; the findings above are correctness/scope findings rather than asserted style-rule violations. `git diff --check` reports trailing whitespace in LCD files, but this tooling-enforced issue is not duplicated as a review finding.

## Spec

Available specification source: the LCD commit messages `7a008339f` through `9977cb9d`; no issue reference, tracker configuration, or feature spec file was present.

### CRITICAL

None.

### HIGH

- **Unrequested work exceeds the LCD specifications.** Every available LCD commit specification concerns LCD state, splash loading, shared LCD operations, configs, or retry naming. The branch nevertheless carries 56 non-merge commits / 149 non-LCD files (for example NDP120/audio, framework media, kernel signals, MTD, BLE, and Wi-Fi). This is material scope creep and blocks approval of an LCD-only goal.
- **BMP conversion does not meet the promised support.** `da14ea48b` promises a common 16-bit BMP decoder/converter to RGB565. The undefined negative shifts at [image_lib.c:73](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/image_lib.c:73) mean advertised `BI_BITFIELDS` support fails for valid masks wider than the RGB565 target channels.

### MEDIUM

- **“General for all lcd driver” is not capability-safe.** `92e655a77` says splash is general for all LCD drivers, yet [lcd_dev.c:578](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/lcd_dev.c:578) dereferences `dev->setpower` without checking it. The ordinary ioctl path does check at lines 376-380. With splash enabled, an LCD implementation without `setpower` crashes instead of returning `-ENOSYS` or skipping splash.
- **SW-rotation allocation leaks on registration failure.** `9d8038cff` moves common rendering into registration. The rotation allocation at [lcd_dev.c:662](/home/user/Dev/Public/TizenRT/251028-lcd-refactor/os/drivers/lcd/lcd_dev.c:662) has no retained base pointer and is not released on the failure path at lines 706-713. A malformed/failed splash after allocation leaks both frame buffers.

### LOW

None.

The Kconfig gate/build rule, RTL8730E config updates, retry-option rename, and removal of LCD-side `CONFIG_PM` conditional paths are present; no finding is recorded for those stated requirements.

## Skill-perspective check

The required `remove-ai-slops` and `programming` skills are not available in the session catalog or filesystem search, so their stated criteria were applied manually. The check ran manually. No tests were added, hence there are no deletion-only, removal-only, tautological, or implementation-mirroring test findings. The diff does violate the relevant production-code perspective through an insufficiently bounded new BMP parser/converter (MEDIUM/LOW findings above); the parser itself is in scope, so it is not rejected as needless complexity.

## Required blockers before approval

1. Rebase/split the branch so it contains only the LCD refactor (or provide an explicitly broader, approved specification).
2. Fix the missing-splash double-free and test the no-file path.
3. Propagate DSI display-off errors without committing a false LCD state.
4. Correct or explicitly reject unsupported BI_BITFIELDS masks before any shift.
