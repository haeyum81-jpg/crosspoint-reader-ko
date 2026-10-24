#pragma once

#include "../Activity.h"
#include "util/ButtonNavigator.h"

// Simple list of the built-in games (Chess, Caro, Sudoku, Minesweeper, 2048), reached from
// Settings > System > Games. Rows are drawn with plain renderer primitives, so this screen has no
// dependency on the FreeInkUI list components.
class GamesActivity final : public Activity {
  static constexpr int GAME_COUNT = 5;

  ButtonNavigator buttonNavigator;
  int selectorIndex = 0;

  void launch(int index);

 public:
  explicit GamesActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Games", renderer, mappedInput) {}

  void onEnter() override;
  void loop() override;
  void render(RenderLock&&) override;
};
