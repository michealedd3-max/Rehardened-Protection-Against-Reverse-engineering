#pragma once
#include <string>

struct PolicyState {
    int build_channel;
    bool integrity_ok;
    bool license_ok;
    bool online_ok;
    bool debugger_present;
    bool module_ok;
};

class RustPolicy {
public:
    bool load(const std::string& dll_path);
    bool validate(const PolicyState& state) const;
    void unload();

private:
    void* handle_ = nullptr;
};
