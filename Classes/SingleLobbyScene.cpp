#include "SingleLobbyScene.h"
#include <ctime>
#include <chrono>
#include "json11.h"
#include "GameInfo.h"
#include "HelloWorldScene.h"

using namespace json11;

Scene* SingleLobbyScene::createScene()
{
  // 'scene' is an autorelease object
  auto scene = Scene::createWithPhysics();

  auto layer = SingleLobbyScene::create();


  scene->addChild(layer);

  // return the scene
  return scene;
}


// on "init" you need to initialize your instance
bool SingleLobbyScene::init()
{
  //////////////////////////////
  // 1. super init first
  if ( !Layer::init() )
    {
      return false;
    }
    
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.
  read_theme_json();

    
  this->scheduleUpdate();

  /*
    auto listener = EventListenerTouchOneByOne::create(); 
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
  */

  return true;
}

void SingleLobbyScene::update(float dt) {

}

void SingleLobbyScene::read_theme_json() {
  std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("config/ko/sports_theme.json");
  //auto bufferSize = 0; // android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  auto bufferSize = 0; 
#else 
  long bufferSize = 0; 
#endif

  //long bufferSize = 0; // linux
  unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "rb", &bufferSize );
  std::string read_data((const char*)json, bufferSize);
  parsing_level_json(read_data);
}

void SingleLobbyScene::parsing_level_json(std::string read_data) {
  std::string err;
  auto json = Json::parse(read_data, err);

  if (!err.empty()) {
    CCLOG("[error] fail to parse singl_play.json");
  } else {
    for(auto& parsed_lv : json["levels"].array_items()) {

      level lv;
      lv.move = parsed_lv["move"].int_value();
      lv.row = parsed_lv["row"].int_value();
      lv.col = parsed_lv["col"].int_value();

      for(auto& word: parsed_lv["words"].array_items()) {
        lv.words.push_back(word.string_value());
      }

      for(auto& key: parsed_lv["keys"].array_items()) {
        //lv.keys.push_back(key.string_value()[0]);

        // 쭉 배열되어있는 키값들을 1개씩 넣기
        //CCLOG("사이즈: %d", key.string_value().size());
        lv.keys.push_back(key.string_value());
      }

      level_md::get().get_sports().push_back(lv);
    }
  }

  create_start_ui();
}

void SingleLobbyScene::create_start_ui() {
 auto next_button = ui::Button::create();
  next_button->setTouchEnabled(true);
  next_button->ignoreContentAdaptWithSize(false);
  next_button->setContentSize(Size(128, 128));
  next_button->setScale(0.5f);
  next_button->loadTextures("ui/start.png", "ui/start.png");

  next_button->setPosition(center);

  next_button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
      if(type == ui::Widget::TouchEventType::BEGAN) {

      } else if(type == ui::Widget::TouchEventType::ENDED) {
        
        auto play_scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(play_scene);

      } else if(type == ui::Widget::TouchEventType::CANCELED) {

      }
    });
     
  this->addChild(next_button, 0);
}
