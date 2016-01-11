#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

struct block_info {
  Vec2 pos;
  Sprite* sprite;
  PhysicsBody* physcis;
  char key;
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

    int row_;
    int col_;

    //std::vector<std::tuple<int, int>> touched_blocks_;
    std::map<int, std::tuple<int, int>> touched_blocks_;
    std::deque<std::tuple<int, int>> ordered_touched_blocks_;
    bool is_touched_blocks(int row, int col);
    void start_touched_action(int row, int col);
    bool is_proper_place(int row, int col);
    bool is_touched;
    char get_rand_img_alphabet();

   

    Button* restart_button;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void clean_up();
};

#endif // __HELLOWORLD_SCENE_H__
//http://stackoverflow.com/questions/32557414/how-do-you-load-a-scene-while-animating-a-sprite-in-cocos2d-x
