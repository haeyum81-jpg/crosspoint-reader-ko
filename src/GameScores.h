#pragma once
#include <cstdint>

// Persists high scores for the built-in games to /.crosspoint/game_scores.bin.
// Loaded lazily on first use (ensureLoaded) so boot does not touch the SD card for it.
// Each game saves on improvement.
class GameScores {
  static GameScores instance;
  bool loaded = false;

 public:
  uint32_t best2048 = 0;  // 2048 best score

  static GameScores& getInstance() { return instance; }

  // Reads the file once per boot; a missing file leaves the defaults in place.
  void ensureLoaded();
  bool loadFromFile();
  bool saveToFile() const;
};

#define GAME_SCORES GameScores::getInstance()
