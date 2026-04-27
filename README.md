# Rehardened PC

A small Windows-focused launcher hardening template for PC games or PC apps.

## What this does

- Verifies a manifest of files before launch
- Loads a Rust policy DLL
- Applies a fail-closed launch decision
- Separates policy logic from the C++ launcher

## What this does not do

This does not make reverse engineering impossible. Client-side code can always be inspected eventually. The purpose here is to raise the cost of tampering and patching.

## Layout

```text
rehardened-pc/
  launcher/
  policy/
  assets/
  tools/
```

## Build requirements

- CMake 3.16+
- A C++17 compiler
- Rust stable
- Windows for the launcher example

## Build the Rust DLL

```bash
cd policy
cargo build --release
```

Copy the resulting DLL next to the launcher binary and name it:

- `rehardened_policy.dll`

## Build the C++ launcher

```bash
cmake -S . -B build
cmake --build build --config Release
```

## Generate the manifest

The launcher checks `assets/manifest.txt`.

Run the helper script after you decide which files you want to protect:

```bash
python tools/gen_manifest.py
```

That script uses the same FNV-1a 64-bit hash as the launcher.

## Notes for PC games

Good targets for checks:

- launcher binary
- policy DLL
- asset pack files
- config files
- plugin whitelist

## Notes for PC apps

Good targets for checks:

- launcher binary
- signed config
- plugin DLLs
- licensing files
- update manifests

## Suggested release workflow

1. Build release binaries.
2. Copy them into a clean output folder.
3. Generate the manifest for the shipped files.
4. Sign the manifest or config in a real release pipeline.
5. Keep sensitive logic on the server when possible.

## Important

If any required file hash changes, the launcher blocks startup.
