#pragma once

#include <cstddef>

// Auto page-turn choices in seconds per page. Index 0 disables; the EPUB and TXT readers and their
// menus share this table so every format offers the same options.
namespace autoturn {
constexpr int SECONDS[] = {0, 5, 10, 15, 20, 25, 30, 45, 60};
constexpr size_t COUNT = sizeof(SECONDS) / sizeof(SECONDS[0]);

constexpr unsigned long intervalMs(const size_t index) { return static_cast<unsigned long>(SECONDS[index]) * 1000UL; }
}  // namespace autoturn
