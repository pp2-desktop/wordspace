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
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
   
    block_info** blocks;
    //block_info* blocks[5][5];
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

    std::vector<std::tuple<int, int>> touched_blocks_;
    bool is_touched_blocks(int row, int col);

    bool is_touched;

    Button* restart_button;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
