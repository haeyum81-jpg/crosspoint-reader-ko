#include "GamesActivity.h"

#include <GfxRenderer.h>
#include <I18n.h>
#include <Memory.h>

#include "CaroActivity.h"
#include "ChessActivity.h"
#include "MinesweeperActivity.h"
#include "SudokuActivity.h"
#include "TwentyFortyEightActivity.h"
#include "components/UITheme.h"
#include "fontIds.h"

namespace {
// Flash-resident label table; order matches GamesActivity::launch().
constexpr StrId GAME_LABELS[] = {StrId::STR_CHESS, StrId::STR_CARO, StrId::STR_SUDOKU, StrId::STR_MINESWEEPER,
                                 StrId::STR_2048};
}  // namespace

void GamesActivity::launch(const int index) {
  // makeUniqueNoThrow: bare `new` aborts on OOM under -fno-exceptions (see AGENTS.md rule 9).
  std::unique_ptr<Activity> game;
  switch (index) {
    case 0:
      game = makeUniqueNoThrow<ChessActivity>(renderer, mappedInput);
      break;
    case 1:
      game = makeUniqueNoThrow<CaroActivity>(renderer, mappedInput);
      break;
    case 2:
      game = makeUniqueNoThrow<SudokuActivity>(renderer, mappedInput);
      break;
    case 3:
      game = makeUniqueNoThrow<MinesweeperActivity>(renderer, mappedInput);
      break;
    case 4:
      game = makeUniqueNoThrow<TwentyFortyEightActivity>(renderer, mappedInput);
      break;
    default:
      return;
  }
  if (!game) {
    LOG_ERR("GAMES", "OOM: game activity %d", index);
    return;
  }
  activityManager.pushActivity(std::move(game));
}

void GamesActivity::onEnter() {
  Activity::onEnter();
  selectorIndex = 0;
  requestUpdate();
}

void GamesActivity::loop() {
  buttonNavigator.onNext([this] {
    selectorIndex = ButtonNavigator::nextIndex(selectorIndex, GAME_COUNT);
    requestUpdate();
  });

  buttonNavigator.onPrevious([this] {
    selectorIndex = ButtonNavigator::previousIndex(selectorIndex, GAME_COUNT);
    requestUpdate();
  });

  if (mappedInput.wasReleased(MappedInputManager::Button::Confirm)) {
    launch(selectorIndex);
    return;
  }

  if (mappedInput.wasReleased(MappedInputManager::Button::Back)) {
    finish();
  }
}

void GamesActivity::render(RenderLock&&) {
  const auto& metrics = UITheme::getInstance().getMetrics();
  const int pageWidth = renderer.getScreenWidth();

  renderer.clearScreen();

  GUI.drawHeader(renderer, Rect{0, metrics.topPadding, pageWidth, metrics.headerHeight}, tr(STR_GAMES));

  const int rowHeight = metrics.menuRowHeight;
  const int listTop = metrics.topPadding + metrics.headerHeight + metrics.verticalSpacing;
  const int textHeight = renderer.getLineHeight(UI_10_FONT_ID);

  for (int i = 0; i < GAME_COUNT; i++) {
    const int y = listTop + i * rowHeight;
    const bool selected = (i == selectorIndex);
    if (selected) {
      renderer.fillRect(metrics.contentSidePadding, y, pageWidth - 2 * metrics.contentSidePadding, rowHeight - 2);
    }
    // Inverted text on the selected (black) row.
    renderer.drawText(UI_10_FONT_ID, metrics.contentSidePadding + 12, y + (rowHeight - textHeight) / 2,
                      I18N.get(GAME_LABELS[i]), !selected);
  }

  const auto labels = mappedInput.mapLabels(tr(STR_BACK), tr(STR_SELECT), tr(STR_DIR_UP), tr(STR_DIR_DOWN));
  GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);

  renderer.displayBuffer();
}
