#ifndef __SINGLE_LOBBY_SCENE_H__
#define __SINGLE_LOBBY_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SingleLobbyScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    SingleLobbyScene() {
      CCLOG("ctor");
    }
    ~SingleLobbyScene() {
      CCLOG("dtor");
    }
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
   


    void update(float dt);
    
    void read_theme_json();
    void parsing_level_json(std::string read_data);

    void create_start_ui();

    Vec2 center;
  /*
    virtual bool onTouchBegan(Touch* touch, Event* unused_event);
    virtual void onTouchMoved(Touch* touch, Event* unused_event);
    virtual void onTouchCancelled(Touch* touch, Event* unused_event);
    virtual void onTouchEnded(Touch* touch, Event *unused_event);
  */


    CREATE_FUNC(SingleLobbyScene);
};

#endif // __HELLOWORLD_SCENE_H__
//http://stackoverflow.com/questions/32557414/how-do-you-load-a-scene-while-animating-a-sprite-in-cocos2d-x



/*
bool HelloWorld::check_end_game() {


}

void HelloWorld::read_theme_json() {
  std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("config/sports_theme.json");
  //auto bufferSize = 0; // android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  auto bufferSize = 0; 
#else 
  long bufferSize = 0; 
#endif

  //long bufferSize = 0; // linux
  unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "rb", &bufferSize );
  std::string read_data((const char*)json, bufferSize);
  parsing_json(read_data);
}

void HelloWorld::parsing_level_json(std::string read_data) {
  string err;
  auto json = Json::parse(read_data, err);

  if (!err.empty()) {
    CCLOG("[error] fail to parse singl_play.json");
  } else {
    for(auto &level : json["levels"].array_items()) {

    }
  }
}
*/
