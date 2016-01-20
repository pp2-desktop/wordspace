#include <vector>
#include <string>

#define ccsf2(...) CCString::createWithFormat(__VA_ARGS__)->getCString()

struct level {
  level();
  level(int move, int row, int col);
  ~level();
  int move;
  int row;
  int col;
  std::vector<std::string> words;
  std::vector<char> keys;
  //std::vector<std::string> keys;
};

class level_md {

  std::vector<level> sports_;

 public:
  static level_md& get() {
    static level_md obj;
    return obj;
  }

  level_md();
  ~level_md();

  std::vector<level>& get_sports() { return sports_; }
  int complete_sport_level;
};
