#include "GameInfo.h"

level::level() {
  move = 0;
  row = 0;
  col = 0;
}

level::level(int move, int row, int col) {
  move = move;
  row = row;
  col = col;
}

level::~level() {
}

level_md::level_md() {
  complete_sport_level = 0;

}

level_md::~level_md() {

}
