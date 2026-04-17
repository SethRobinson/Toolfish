---
name: update-version
description: Bump the Toolfish version number and update README.md history. Use when the user says "update the version", "bump the version", "version to V2.X", or asks to release/version a new build of Toolfish.
---

# Update Toolfish Version

Use this skill whenever the user asks to update, bump, or release a new Toolfish version (e.g. "let's version to V2.36"). It covers the two files that must change in lockstep and the format expected for the README history entry.

## Files to update

1. `Source/CGlobals.h` ? contains the version constant:

   ```cpp
   const float C_F_VERSION = 2.36f;
   ```

   Replace `2.36f` with the new version using the `X.YYf` float literal format (always two decimal places, lowercase `f` suffix).

2. `README.md` ? has two sections that may need updates:
   - **History section** (required): add a new entry at the top, just under `## History`, above the previous version's entry.
   - **Actions table** (only if a new Action was added): insert a new row in the `### Actions` table under `## Triggers & Actions Reference`.

## History entry format

Use this exact format. The date should be today's date in `MMM Dth, YYYY` form (e.g. `Apr 16th, 2026`, `Feb 4th, 2026`). Use ordinal suffixes (`st`, `nd`, `rd`, `th`).

```markdown
**V2.36 Apr 16th, 2026**
* Short bullet describing change one
* Short bullet describing change two
```

Leave a blank line between the new entry and the previous version's entry.

## Workflow

1. Confirm the new version number and what changed (the user usually states this in the request).
2. Edit `Source/CGlobals.h` to bump `C_F_VERSION`.
3. Edit `README.md`:
   - Add the new `**VX.YY <date>**` history block directly under `## History`.
   - If new Actions or Triggers were added, also add corresponding rows to the tables under `### Actions` / `### Triggers`.
4. Show the user the diff/summary of edits. Do NOT commit unless explicitly asked.

## Notes

- Do not modify `unicode_Source.rc` `VS_VERSION_INFO` ? it is currently kept at `1,0,0,1` and is not part of the public version scheme.
- The version is a `float`, so only two decimal digits are meaningful. Treat `2.36` and `V2.36` as equivalent.
- Bullets in the history entry should mirror the style of the existing entries: terse, sentence-case, no trailing period required (match neighboring entries if the user has a preference).
