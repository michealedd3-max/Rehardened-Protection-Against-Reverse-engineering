#include "integrity.hpp"
#include "rust_policy.hpp"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

static bool is_debugger_present() {
    return IsDebuggerPresent() != 0;
}

static bool module_whitelist_ok() {
    // Keep this tight. Add only modules you expect.
    // The policy DLL itself must be present.
    return GetModuleHandleA("rehardened_policy.dll") != nullptr;
}

int main() {
    std::vector<ManifestEntry> manifest;
    if (!load_manifest("assets/manifest.txt", manifest)) {
        std::cerr << "manifest load failed\n";
        return 1;
    }

    const bool integrity_ok = verify_manifest(manifest);
    if (!integrity_ok) {
        std::cerr << "integrity check failed\n";
        return 2;
    }

    RustPolicy policy;
    if (!policy.load("rehardened_policy.dll")) {
        std::cerr << "policy dll load failed\n";
        return 3;
    }

    PolicyState state{};
    state.build_channel = 0; // 0=release, 1=beta, 2=dev
    state.integrity_ok = integrity_ok;
    state.license_ok = true; // replace with real license verification
    state.online_ok = true;  // replace with real entitlement/server handshake
    state.debugger_present = is_debugger_present();
    state.module_ok = module_whitelist_ok();

    if (!policy.validate(state)) {
        std::cerr << "policy denied launch\n";
        policy.unload();
        return 4;
    }

    std::cout << "launch allowed\n";
    policy.unload();
    return 0;
}
