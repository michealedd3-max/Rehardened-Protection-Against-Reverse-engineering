#include "integrity.hpp"
#include <fstream>
#include <sstream>

static uint64_t fnv1a64_bytes(const std::string& data) {
    uint64_t hash = 1469598103934665603ULL;
    for (unsigned char c : data) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    return hash;
}

uint64_t hash_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return 0;

    std::ostringstream ss;
    ss << in.rdbuf();
    return fnv1a64_bytes(ss.str());
}

static bool parse_line(const std::string& line, ManifestEntry& entry) {
    // format: path|hash|required
    // example: launcher/main.cpp|123456789|1
    size_t p1 = line.find('|');
    size_t p2 = line.find('|', p1 == std::string::npos ? std::string::npos : p1 + 1);

    if (p1 == std::string::npos || p2 == std::string::npos) {
        return false;
    }

    entry.path = line.substr(0, p1);
    entry.expected_hash = std::stoull(line.substr(p1 + 1, p2 - p1 - 1));
    entry.required = line.substr(p2 + 1) == "1";
    return !entry.path.empty();
}

bool load_manifest(const std::string& path, std::vector<ManifestEntry>& out) {
    std::ifstream in(path);
    if (!in) return false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        ManifestEntry e{};
        if (parse_line(line, e)) {
            out.push_back(e);
        }
    }
    return !out.empty();
}

bool verify_manifest(const std::vector<ManifestEntry>& entries) {
    for (const auto& e : entries) {
        const uint64_t h = hash_file(e.path);
        if (h == 0 && e.required) {
            return false;
        }
        if (h != e.expected_hash) {
            return false;
        }
    }
    return true;
}
