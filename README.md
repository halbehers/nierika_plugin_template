# Nierika Plugin Template

**Nierika Plugin Template** is a cookie-cutter starting point for macOS & Windows JUCE audio
plugins by Nierika. It has no business logic of its own — instead it ships a working, tested,
CI-wired skeleton: a themed editor shell with a settings button, native standalone window styling,
an internationalization mechanism, and the full build/test/packaging pipeline already wired up, so
a new plugin starts from "everything compiles, builds, tests, and ships an installer" rather than
an empty JUCE project.

Available as Standalone, AU, AUv3, and VST3.

## Features

- **Themed `AppLayout` shell** with a settings button that opens a settings window (title + close
  button only — see [Where to add real functionality](#where-to-add-real-functionality) below for
  wiring actual content into it).
- **Native standalone window styling** — a custom title bar with proper traffic-light/caption-bar
  theming on both macOS and Windows.
- **Internationalization (i18n)** mechanism (English, French, Spanish, German, Italian, Portuguese)
  with English as the always-available fallback.
- **Light/dark theme**, backed by persisted user preferences shared across all plugin instances
  and hosts.
- Full CI/build/test/packaging pipeline: CMake presets, CPM-fetched dependencies, Catch2 unit
  tests, `pluginval` end-to-end validation, GitHub Actions build matrix, and macOS/Windows
  installer packaging.

## Requirements

- macOS ≥ 14.5 or Windows ≥ 10 (2020)
- CMake ≥ 3.22, [Ninja](https://ninja-build.org/)
- A C++20 compiler (Xcode command line tools)

## Building

Dependencies — [JUCE](https://github.com/juce-framework/JUCE) 8.0.14,
[Catch2](https://github.com/catchorg/Catch2), and Nierika's `nierika_dsp` module — are fetched
automatically via [CPM](https://github.com/cpm-cmake/CPM.cmake) on first configure.

```sh
cmake --workflow --preset default  # configure (first run/whenever CMakeLists.txt changes) + build
```

`--workflow` always does the right thing whether `build/` already exists or not (a fresh clone, or
after deleting it) - if you'd rather configure and build as separate steps (e.g. to build
repeatedly without reconfiguring), that still works too, as long as `build/` already exists:

```sh
cmake --preset default          # configure (Debug, Ninja) - only needed once, or after CMakeLists.txt changes
cmake --build --preset default  # build
```

Built plugin bundles land in `build/NierikaPluginTemplate_artefacts/Debug/{Standalone,AU,VST3}`.
For a Release build (also produces an installer - see below):

```sh
cmake --workflow --preset release
```

Xcode and Visual Studio project generation is available via the `Xcode`/`vs` presets.

### Installers

A Release build also packages an installer automatically:
`release-build/Packaging/Nierika Plugin Template-<version>-macOS.pkg` (AU + VST3, ad-hoc signed
unless real Developer ID credentials are configured in the environment) on macOS, or
`release-build\Packaging\Nierika Plugin Template-<version>-Windows.exe` (VST3, requires
[Inno Setup](https://jrsoftware.org/isdl.php)'s `iscc` on `PATH`) on Windows.

## Testing

```sh
ctest --test-dir build
```

Runs the Catch2 unit test suite plus an end-to-end [pluginval](https://github.com/Tracktion/pluginval)
validation pass against the built AU (parameter automation, state save/restore, thread safety,
repeated editor open/close). See `build/Tests/NierikaPluginTemplate_Tests --help` for running a
subset of tests by name or tag.

## Continuous integration

`.github/workflows/build_and_test.yml` runs on GitHub Actions:

- **When it runs**: on every push to any branch, on every pull request (a same-repo PR is skipped
  since the matching push event already covers it — only PRs from forks trigger separately), and
  on demand via the Actions tab's "Run workflow" button (`workflow_dispatch`).
- **What it does**: builds a matrix of macOS (universal `arm64`+`x86_64`) and Windows, each of
  which configures and builds a Release, runs `ctest` (the Catch2 suite plus the `pluginval`
  validation pass), then uploads the resulting installer as a workflow artifact, downloadable from
  that run's summary page.
- **Artifact generation/signing**: the installer itself is produced by the *same* Release build
  described in [Installers](#installers) above (`CMake/packaging.cmake`) — CI doesn't package
  anything separately or differently from a local `cmake --workflow --preset release`. Signing is
  the only thing that differs by environment: it's ad-hoc/unsigned unless the corresponding
  repository secrets are configured, so the workflow always produces something installable even
  without a paid signing setup:
  - macOS: imports a Developer ID Application/Installer certificate from the `DEV_ID_APP_CERT` /
    `DEV_ID_APP_PASSWORD` / `DEV_ID_INSTALLER_CERT` / `DEV_ID_INSTALLER_PASSWORD` secrets, then
    notarizes and staples the `.pkg` if `NOTARIZATION_USERNAME` / `NOTARIZATION_PASSWORD` /
    `TEAM_ID` are also set.
  - Windows: signs the `.exe` via Azure Artifact Signing if `AZURE_TENANT_ID` (and the other
    `AZURE_*` secrets) are set.
- **Releases**: pushing a tag matching `v*` (e.g. `v1.2.0`) additionally runs the `release` job,
  which downloads both installers from that run and attaches them to a new pre-release GitHub
  Release — no separate manual publish step needed.

## Where to add real functionality

See `CLAUDE.md` for a full checklist, but in short:

- **Renaming**: the identity variables at the top of `CMakeLists.txt`
  (`PROJECT_NAME`/`PRODUCT_NAME`/`COMPANY_NAME`/`BUNDLE_ID`/`MANUFACTURER_CODE`/`PLUGIN_CODE`),
  `Packaging/icon.png` (regenerate the iconset via `Scripts/generate_iconset.sh`), and this file.
- **DSP**: `PluginProcessor::processBlock`/`prepareToPlay`.
- **Parameters**: `Parameters.h`/`.cpp`.
- **Settings content**: `Code/Component/VisualSettings` (theme toggle) and
  `Code/Component/LanguageSettings` (language picker) are working reference components that
  aren't currently wired into `SettingsWindow` — copy their pattern when adding real settings
  content back into `SettingsWindow`'s internal `_layout`.
- **Translations**: `Assets/Languages/*.lang`.

## Project layout

- `Code/Include`, `Code/Source` — plugin source, mirrored header/implementation layout.
- `Assets/Languages` — localization strings (`.lang` files).
- `Tests` — Catch2 unit tests.
- `CMake` — build configuration helpers (dependency fetching, compiler warnings, `pluginval`
  integration).
- `Libs` — CPM-fetched dependencies (JUCE, and Nierika's own `nierika_dsp` module unless a local
  checkout is configured — see `CMakeLists.txt`).

---

## Developers

Nierika (`halbehers`).
