from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def fnv1a64(data: bytes) -> int:
    h = 1469598103934665603
    for b in data:
        h ^= b
        h = (h * 1099511628211) & 0xFFFFFFFFFFFFFFFF
    return h

paths = [
    Path("launcher/main.cpp"),
    Path("launcher/integrity.cpp"),
    Path("launcher/rust_policy.cpp"),
    Path("policy/src/lib.rs"),
]

lines = []
for rel in paths:
    p = ROOT / rel
    if not p.exists():
        raise SystemExit(f"missing file: {rel}")
    h = fnv1a64(p.read_bytes())
    lines.append(f"{rel.as_posix()}|{h}|1")

out = ROOT / "assets" / "manifest.txt"
out.write_text("\n".join(lines) + "\n", encoding="utf-8")
print(f"wrote {out}")
