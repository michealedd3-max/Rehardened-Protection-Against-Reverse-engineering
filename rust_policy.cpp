#include "rust_policy.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

struct PcPolicyInput {
    int build_channel;
    int integrity_ok;
    int license_ok;
    int online_ok;
    int debugger_present;
    int module_ok;
};

using validate_fn = bool(*)(const PcPolicyInput*);

bool RustPolicy::load(const std::string& dll_path) {
#ifdef _WIN32
    handle_ = LoadLibraryA(dll_path.c_str());
#else
    handle_ = dlopen(dll_path.c_str(), RTLD_NOW);
#endif
    return handle_ != nullptr;
}

bool RustPolicy::validate(const PolicyState& state) const {
    if (!handle_) return false;

#ifdef _WIN32
    auto fn = reinterpret_cast<validate_fn>(
        GetProcAddress((HMODULE)handle_, "rehardened_validate_pc_v2")
    );
#else
    auto fn = reinterpret_cast<validate_fn>(
        dlsym(handle_, "rehardened_validate_pc_v2")
    );
#endif

    if (!fn) return false;

    PcPolicyInput in{};
    in.build_channel = state.build_channel;
    in.integrity_ok = state.integrity_ok ? 1 : 0;
    in.license_ok = state.license_ok ? 1 : 0;
    in.online_ok = state.online_ok ? 1 : 0;
    in.debugger_present = state.debugger_present ? 1 : 0;
    in.module_ok = state.module_ok ? 1 : 0;

    return fn(&in);
}

void RustPolicy::unload() {
#ifdef _WIN32
    if (handle_) FreeLibrary((HMODULE)handle_);
#else
    if (handle_) dlclose(handle_);
#endif
    handle_ = nullptr;
}
