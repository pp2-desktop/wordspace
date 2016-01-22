#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

struct block_info {
  block_info();
  ~block_info();
  Vec2 pos;
  Sprite* sprite;
  Label* label;
  PhysicsBody* physics;
  std::string key;
};
typedef block_info* block_info_ptr;

enum alphabet {a=97,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,end};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    HelloWorld() {
      CCLOG("ctor");
    }
    ~HelloWorld() {
      CCLOG("dtor");
    }
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
   
    //block_info* blocks[20][20];

    block_info_ptr** blocks;
    std::vector<block_info*> block_info_ptrs;

    void update(float dt);

    virtual bool onTouchBegan(Touch* touch, Event* unused_event);
    virtual void onTouchMoved(Touch* touch, Event* unused_event);
    virtual void onTouchCancelled(Touch* touch, Event* unused_event);
    virtual void onTouchEnded(Touch* touch, Event *unused_event);

    block_info* get_block_info_by_index(int index);
    int get_row_by_index(int index);
    int get_col_by_index(int index);
    void replace_blocks(int row, int col);

    //std::vector<std::tuple<int, int>> touched_blocks_;
    std::map<int, std::tuple<int, int>> touched_blocks_;
    std::deque<std::tuple<int, int>> ordered_touched_blocks_;
    bool is_touched_blocks(int row, int col);
    void start_touched_action(int row, int col);
    void end_touched_action(int row, int col);
    bool is_proper_place(int row, int col);
    bool is_touched;

    std::string get_img_alphabet(int index);
    int get_index(int row, int col);
    void action_incorrect();
    void replace_level_scene();

    bool check_complete_game();
    bool check_end_game();
    //std::vector<std::tuple<int, int>> find_next_index(char key);
    bool check_incomplete(std::vector<std::vector<std::tuple<int, int>>>& route_word);
    bool check_possible_word2(std::string word);
    int find_index(const std::string c);
    std::vector<std::tuple<int, int>> get_keys(const std::string c);
    bool is_key_next(int row, int col, std::string next_key);
    
    bool check_possible_move(std::tuple<int, int> index0, std::tuple<int, int> index1);

    void push_down_blocks();

    Button* restart_button;

    Label* move_font;


    int find_word(std::tuple<int, int> index, int depth, const std::string& word);
    bool is_path(std::tuple<int, int> index);
    bool go_top_left(std::tuple<int, int> index, std::string key);
    bool go_top(std::tuple<int, int> index, std::string key);
    bool go_top_right(std::tuple<int, int> index, std::string key);
    bool go_right(std::tuple<int, int> index, std::string key);
    bool go_bottom_right(std::tuple<int, int> index, std::string key);
    bool go_bottom(std::tuple<int, int> index, std::string key);
    bool go_bottom_left(std::tuple<int, int> index, std::string key);
    bool go_left(std::tuple<int, int> index, std::string key);

    std::deque<std::tuple<int, int>> path;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void clean_up();

    int char_size;
    int row_;
    int col_;
    int move_;
    std::vector<std::string> words_;
    std::vector<std::string> complete_words_;
    std::vector<std::string> keys_;

};

#endif // __HELLOWORLD_SCENE_H__
//http://stackoverflow.com/questions/32557414/how-do-you-load-a-scene-while-animating-a-sprite-in-cocos2d-x
