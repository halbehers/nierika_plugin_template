# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

**Nierika Plugin Template** is a cookie-cutter starting point for JUCE audio plugins (Standalone /
AU / AUv3 / VST3) maintained by Nierika. It carries no domain-specific business logic — what it
provides is a working, fully-wired scaffold: CI/build/test/packaging infrastructure, native
standalone window chrome, an i18n mechanism, and a minimal themed `AppLayout`/`SettingsWindow`
shell. Use it as the base for a new plugin rather than starting from an empty JUCE project.

## Build / Tests

Same CMake presets, CPM dependency fetching, Catch2 + `pluginval` CTest wiring, and GitHub Actions
workflow as any project built on this template — see `README.md` for the exact commands
(`cmake --workflow --preset default`, `ctest --test-dir build`, etc.). Nothing about the build
pipeline itself needs to change to start a new plugin from this template; only the identity/content
changes described below.

## Turning this into a new plugin

Renaming checklist:

- The six identity variables at the top of `CMakeLists.txt`: `PROJECT_NAME`, `PRODUCT_NAME`,
  `COMPANY_NAME`, `BUNDLE_ID`, `MANUFACTURER_CODE`, `PLUGIN_CODE`. `COMPANY_NAME` is threaded
  through as a compile definition (`target_compile_definitions(... COMPANY_NAME="${COMPANY_NAME}")`)
  and read directly by `AppLogger.cpp` and `AppSettings::getAppSupportDirectory()` — changing the
  one CMake variable is enough, there's nothing else to edit by hand for those two call sites.
- The `InterProcessLock` id in `AppSettings.h` (`"nierika-plugin-template-settings"`).
- `Packaging/icon.png` — replace, then regenerate `Packaging/icon.iconset/*` + `icon.icns` via
  `Scripts/generate_iconset.sh Packaging/icon.png Packaging`.
- `Packaging/*/resources/EULA` and `Packaging/macos/resources/README` — placeholder text
  referencing "this plugin" / "Nierika Plugin Template", not tied to any variable.
- `README.md`.

Where to add real functionality:

- `PluginProcessor::processBlock`/`prepareToPlay` for actual DSP — both are currently no-ops
  (`processBlock` just clears any channel without a corresponding input).
- `Parameters.h`/`.cpp` for new parameters, following the existing `PLUGIN_ENABLED_ID` pattern.
- `Code/Include/Component/VisualSettings` (theme toggle) and
  `Code/Include/Component/LanguageSettings` (language picker) are working, compiled reference
  components — they exist but aren't instantiated by `SettingsWindow`. Copy their pattern (and
  `SettingsWindow`'s own `_layout.addComponent(...)` calls, now reduced to just the title) when
  wiring real settings content back in.
- `Assets/Languages/*.lang` for new translated strings/languages. Never delete the
  `settings_title`, `visual_settings_title`/`_theme_label`/`_dark_theme`/`_light_theme`, or
  `language_settings_title` keys without also removing the components that reference them via
  `juce::translate(...)`.

## What's deliberately absent and why

No audio-capture dependency, no process table/filtering/categorization, no domain-specific
business logic of any kind — this template is intentionally an empty themed shell with working
infra, ready for a new plugin's actual feature set to be built on top.
