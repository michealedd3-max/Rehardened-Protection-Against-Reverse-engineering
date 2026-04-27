#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct ManifestEntry {
    std::string path;
    uint64_t expected_hash;
    bool required;
};

bool load_manifest(const std::string& path, std::vector<ManifestEntry>& out);
uint64_t hash_file(const std::string& path);
bool verify_manifest(const std::vector<ManifestEntry>& entries);
