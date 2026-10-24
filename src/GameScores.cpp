#include "GameScores.h"

#include <HalStorage.h>
#include <Logging.h>
#include <Serialization.h>

namespace {
constexpr uint8_t SCORES_FILE_VERSION = 3;
constexpr char SCORES_DIR[] = "/.crosspoint";
constexpr char SCORES_FILE[] = "/.crosspoint/game_scores.bin";
}  // namespace

GameScores GameScores::instance;

void GameScores::ensureLoaded() {
  if (loaded) return;
  loaded = true;  // set first: a missing/corrupt file must not be retried on every game launch
  loadFromFile();
}

bool GameScores::saveToFile() const {
  Storage.mkdir(SCORES_DIR);
  HalFile file;
  if (!Storage.openFileForWrite("GSC", SCORES_FILE, file)) {
    LOG_ERR("GSC", "Failed to open game_scores.bin for write");
    return false;
  }
  const uint8_t version = SCORES_FILE_VERSION;
  serialization::writePod(file, version);
  serialization::writePod(file, best2048);
  file.close();
  return true;
}

bool GameScores::loadFromFile() {
  HalFile file;
  if (!Storage.openFileForRead("GSC", SCORES_FILE, file)) {
    return false;  // first use: file doesn't exist yet
  }
  uint8_t version = 0;
  serialization::readPod(file, version);
  if (version != SCORES_FILE_VERSION) {
    LOG_ERR("GSC", "Unknown game_scores.bin version %u", version);
    file.close();
    return false;
  }
  serialization::readPod(file, best2048);
  file.close();
  return true;
}
