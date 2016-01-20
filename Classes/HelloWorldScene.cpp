#include "HelloWorldScene.h"
#include <ctime>
#include <chrono>
#include "GameInfo.h"
#include "SimpleAudioEngine.h"
#include <algorithm>

using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,-1500.0f));
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    //layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::clean_up() {
  
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {

      auto block_ptr = blocks[i][j];
      if(block_ptr != nullptr) {
	delete block_ptr;
      }
    }
  }

  CCLOG("clean 1"); 
  
  for(auto i=0; i<row_; i++) {
    delete [] *(blocks+i);
  }

  CCLOG("clean 2"); 

  delete [] blocks;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    restart_button = ui::Button::create();
    restart_button->setContentSize(Size(231, 94));
    restart_button->loadTextures("restart.png", "restart.png");
    restart_button->setPosition(Vec2(visibleSize.width + 1500.0f, visibleSize.height - 100.0f));

    restart_button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if(type == ui::Widget::TouchEventType::BEGAN) {
          auto scaleTo = ScaleTo::create(0.2f, 1.3f);
          restart_button->runAction(scaleTo);

        } else if(type == ui::Widget::TouchEventType::ENDED) {
          auto scaleTo2 = ScaleTo::create(0.2f, 1.0f);
          restart_button->runAction(scaleTo2);

	  clean_up();

	  auto s = HelloWorld::createScene();
	  Director::getInstance()->replaceScene(s);
	  //Director::getInstance()->replaceScene(TransitionFade::create(0.0f, _scene, Color3B(0,255,255)));

        } else if(type == ui::Widget::TouchEventType::CANCELED) {
          auto scaleTo2 = ScaleTo::create(0.2f, 1.0f);
          restart_button->runAction(scaleTo2);
        }
      });

    this->addChild(restart_button, 0);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);

    
    //std::srand(std::time(0)); // use current time as seed for random generator
    //row_ = (std::rand() % 6) + 2; // 2x2, 3x3, 4x4, 5x5;

    std::vector<level>& lvs = level_md::get().get_sports();

    auto i = level_md::get().complete_sport_level;
    move_ = lvs[i].move;
    words_ = lvs[i].words;
    //keys_ = lvs[i].keys;
    row_ = lvs[i].row;
    col_ = row_;

    blocks = new block_info_ptr*[row_];
    for(auto i=0; i<row_; i++) {
      *(blocks+i) = new block_info_ptr[col_];
    }

    const float margin = 10.0f;
    const float block_margin = 0.0f;
    const float block_img_size = 100.0f;


    float width = 750 - (margin * 2.0f);


    float block_size = 100.0f;


    auto total_block_margin = ((row_-1) * block_margin);
    block_size = (width - total_block_margin) / row_;
 
    //LOG("each side block cnt: %f", block_size);

    auto last_y = visibleSize.height - 100.0f;
    auto tag = 0;
    CCLOG("--------------------------------------------------------------");
    for(auto i=0; i<col_; i++) {
      auto last_x = margin;
      for(auto j=0; j<row_; j++) {

        auto block_info_ptr = new block_info;
	block_info_ptrs.push_back(block_info_ptr);

        auto x = last_x + block_size/2;
        last_x = x + block_margin + block_size/2;

        //char key = get_rand_img_alphabet();
        char key = get_img_alphabet(get_index(i,j));
        CCLOG("%c", key);
        std::string img_path = "aaa/" + std::string(&key, 1) + std::string(".png");
        auto sprite = Sprite::create(img_path);

        sprite->setPosition(Vec2(x, last_y));

        float scale_factor = block_img_size - block_size;

        if(scale_factor == 0.0f) {

        }        
        else if(scale_factor < 0.0f) {
          // 이미지보다 블럭이 더 큰 상황
          auto add_factor = std::abs(scale_factor) * 0.01f;
          scale_factor = 1.00 + add_factor; 
          scale_factor *= 0.85;
	  // 대각선 테스트 하기 위해서 주석처리
          sprite->setScale(scale_factor);
        } else {
          auto sub_factor = std::abs(scale_factor) * 0.01f;
          scale_factor = 1.00 - sub_factor; 
          scale_factor *= 0.85;
	  // 대각선 테스트 하기 위해서 주석처리
          sprite->setScale(scale_factor);
        }

	CCLOG("x: %f, y: %f", sprite->getPosition().x, sprite->getPosition().y);

        
        // 블럭 단어 추가
        std::string tmp = "A";
        auto label = Label::createWithTTF(tmp.c_str(), "fonts/nanumb.ttf", 35);
        label->setAnchorPoint(Vec2(0.5f, 0.5f));
        auto front_size = sprite->getContentSize();
        label->setPosition(Vec2(
                              front_size.width/2.0f,
                              front_size.height/2.0f
                              ));
        label->setTextColor(Color4B::RED);
        sprite->addChild(label);
        

        // physics 
        auto physicsBody = PhysicsBody::createBox(Size(block_size, block_size), PhysicsMaterial(1.0f, 0.0f, 0.0f));

        physicsBody->setGravityEnable(true);
        
	physicsBody->setDynamic(true);
        physicsBody->setRotationEnable(false);
        //physicsBody->setMass(1000.0f);
        //physicsBody->setAngularDamping(0);
        //physicsBody->setLinearDamping(0.0f);
        //physicsBody->setVelocity(Vec2(0.0f, -400.0f));
        sprite->setPhysicsBody(physicsBody);
        //physicsBody->applyImpulse(Vec2(0.0f, -800.0f));

        //sprite->setTag(tag++);
	//sprite->retain();gxs
        this->addChild(sprite, 1);

        block_info_ptr->pos = sprite->getPosition();
        block_info_ptr->sprite = sprite;
        //block_info_ptr->physcis = physicsBody;
        block_info_ptr->key = key;

        blocks[i][j] = block_info_ptr;

	//std::chrono::milliseconds duration( 10 ); 
	//std::this_thread::sleep_for( duration ); // Sleep for 2 seconds.
      }

      last_y = last_y - (block_size + block_margin);
    }


    CCLOG("size: %d", block_info_ptrs.size());
    //CLOG("last y position: %f", last_y);

    auto bottom = Sprite::create("bottom.png");
    bottom->setPosition(Vec2(visibleSize.width/2.0f, 50.0f));
    auto physicsBody2 = PhysicsBody::createBox(Size(bottom->getContentSize().width, bottom->getContentSize().height), PhysicsMaterial(0.0f, 0.0f, 0.0f));

    physicsBody2->setDynamic(false);
    bottom->setPhysicsBody(physicsBody2);

    this->addChild(bottom, 0);


    move_font = Label::createWithTTF(ccsf2("%d", move_) , "fonts/nanumb.ttf", 40);
    move_font->setPosition(Vec2(visibleSize.width/2.0f, visibleSize.height - 100));
    move_font->setColor( Color3B( 255, 255, 255) );
    this->addChild(move_font, 0);

    
    this->scheduleUpdate();


    auto listener = EventListenerTouchOneByOne::create(); 
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    is_touched = false;
    return true;
}

void HelloWorld::update(float dt) {

}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event) {
  CCLOG("------------------------------------");
  if(is_touched) {
    return false;
  }

  CCLOG("터치 시작");
  is_touched = true;
  touched_blocks_.clear();
  ordered_touched_blocks_.clear();
  
  return true; 
}
 
void HelloWorld::onTouchMoved(Touch* touch, Event* unused_event) {

  Point touchEnd = touch->getLocation();


  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {

      if(!is_touched_blocks(i, j)) {
        /*
        if(!check_proper_move(i, j)) {
          return;
        }
        */
        auto block_ptr = blocks[i][j];
        if(block_ptr != nullptr) {
          Rect boundingBox = block_ptr->sprite->boundingBox();

          Size size = boundingBox.size;
          Size size2 = size * 0.65f;
          auto moveX = (size.width - size2.width) * 0.5f; // 10
          auto moveY = (size.height- size2.height) * 0.5f; // 10

          Rect boundingBox2(boundingBox.getMinX() + moveX, boundingBox.getMinY() + moveY, size2.width, size2.height);
          
          if (boundingBox2.containsPoint(touchEnd)) {

            if(is_proper_place(i, j)) {
              std::tuple<int, int> tmp = std::make_tuple(i, j);
              auto index = (i*row_) + j;
              touched_blocks_[index] = tmp;
              ordered_touched_blocks_.push_back(tmp);
              start_touched_action(i, j);
            }
          }
        }
      }
    }
  }
}
 
void HelloWorld::onTouchCancelled(Touch* touch, Event* unused_event) {
  CCLOG("xxxxxxxxxxxxxxx CANCELD xxxxxxxxxxxxxxx");
}
 
void HelloWorld::onTouchEnded(Touch* touch, Event* unused_event) {

  if(ordered_touched_blocks_.size() <= 0) {
    is_touched = false;
    return;
  }
   
  move_font->setString(ccsf2("%d", --move_));

  std::string touched_word;
  for(auto& it : ordered_touched_blocks_) {
    auto i = std::get<0>(it);
    auto j = std::get<1>(it);
    auto block = blocks[i][j];
    touched_word.push_back(block->key);
  }

  //CCLOG("%s", touched_word.c_str());

  bool is_find = false;
  auto index = 0;
  for(auto& word : words_) {
    if(touched_word == word) {
      if(words_.size() > 1) {
        auto audio = SimpleAudioEngine::getInstance();
        audio->playEffect("sound/yeah.wav", false, 1.0f, 1.0f, 1.0f);
      }
      is_find = true;
      complete_words_.push_back(touched_word);
      break;
    }
    index++;
  }


  if(is_find) {
    words_.erase(words_.begin()+index);
  }
  for(auto& word : words_) {
    CCLOG("남은 단어들: %s", word.c_str());
  }



  if(!is_find) {
    action_incorrect();
    if(move_ < 1) {
      clean_up();
      auto audio = SimpleAudioEngine::getInstance();
      audio->playEffect("sound/YouFailed.wav", false, 1.0f, 1.0f, 1.0f);
      this->scheduleOnce(SEL_SCHEDULE(&HelloWorld::replace_level_scene), 0.0f);
    } else {
      is_touched = false;
    }
    return;
  }

    
  for(auto& it : touched_blocks_) {

    auto tmp = it.second;
    
    auto i = std::get<0>(tmp);
    auto j = std::get<1>(tmp);

    auto block_ptr = blocks[i][j];

    if(block_ptr != nullptr) {
      replace_blocks(i, j);
      this->removeChild(block_ptr->sprite, true);
      delete block_ptr;
    }
    
  }

  if(check_complete_game()) {
    auto audio = SimpleAudioEngine::getInstance();
    audio->playEffect("sound/YouWin.wav", false, 1.0f, 1.0f, 1.0f);
    level_md::get().complete_sport_level++;
    this->scheduleOnce(SEL_SCHEDULE(&HelloWorld::replace_level_scene), 0.5f);
    CCLOG("터치 종료");
    return;
  } 


  if(check_end_game()) {
    auto audio = SimpleAudioEngine::getInstance();
    audio->playEffect("sound/YouFailed.wav", false, 1.0f, 1.0f, 1.0f);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    restart_button->setPosition(Vec2(visibleSize.width - 150.0f, visibleSize.height - 100.0f));

  }

   is_touched = false;
   CCLOG("터치 종료");
  
}

block_info* HelloWorld::get_block_info_by_index(int index) {

  if(index == 0){
    return blocks[0][0];
  }
  
  int row = index / 5;
  int col = index % 5;

  CCLOG("row: %d", row);
  CCLOG("col: %d", col);
  
  return blocks[row][col];
}

int HelloWorld::get_row_by_index(int index) {
  if(index == 0) {
    return 0;
  }
  //return index / 5;
  return index / row_;
}

int HelloWorld::get_col_by_index(int index) {
  if(index == 0) {
    return 0;
  }
  //return index % 5;
  return index / col_;
}

void HelloWorld::replace_blocks(int row, int col) {
  
  if(row == 0) {
    auto block_ptr = blocks[0][col];
    blocks[0][col] = nullptr;
    return;
  }

  for(auto i=row; i>0; i--) {
    if(blocks[i-1][col]) {
      CCLOG("i: %d", i);
      CCLOG("col: %d", col);
      //auto block_ptr = blocks[i][col];
      blocks[i][col] = blocks[i-1][col];
      blocks[i-1][col] = nullptr;
    } else {
      //auto block_ptr = blocks[i][col];
      blocks[i][col] = nullptr;
    }
  }
}

bool HelloWorld::is_touched_blocks(int row, int col) {

  auto index = (row * row_) + col;
  auto it = touched_blocks_.find(index);
  if(it != touched_blocks_.end()) {
    return true;
  }

  /*
  for(auto& tb : touched_blocks_) {
    if(std::get<0>(tb) == row && std::get<1>(tb) == col) {
      return true;
    }
  }
  */

  return false;
}

void HelloWorld::start_touched_action(int row, int col) {
  auto block = blocks[row][col];
  auto tintTo = TintTo::create(0.0f, 124.0f, 124.0f, 0.0f);
  block->sprite->runAction(tintTo);
}

void HelloWorld::end_touched_action(int row, int col) {
  auto block = blocks[row][col];
  if(block != nullptr) {
    auto tintTo = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);
    block->sprite->runAction(tintTo);
  }
}

bool HelloWorld::is_proper_place(int row, int col) {

  if(ordered_touched_blocks_.empty()) {
    return true;
  }

  std::tuple<int, int> touched_block = ordered_touched_blocks_.back();
  if(std::abs(std::get<0>(touched_block) - row) > 1 || 
     std::abs(std::get<1>(touched_block) - col) > 1) {
    CCLOG("not proper place");
    return false;
  }

  CCLOG("proper place");
  return true;
}

char HelloWorld::get_rand_img_alphabet() {
  CCLOG("rand_img_aphabet");

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
  std::srand((time_t)ts.tv_nsec);

  //std::srand(std::time(0));
  int r = (std::rand() % 26) + alphabet::a;
  return static_cast<char>(r);
}

char HelloWorld::get_img_alphabet(int index) {
  auto i = level_md::get().complete_sport_level;
  return level_md::get().get_sports()[i].keys[index];
}

bool HelloWorld::check_complete_game() {
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      auto block_ptr = blocks[i][j];
      if(block_ptr != nullptr) {
        return false;
      }
    }
  }
  return true;
}

bool HelloWorld::check_end_game() {

  for(auto& word : words_) {
    if(check_possible_word2(word)) {
      return false;
    }
  }
  return true;
}

bool HelloWorld::check_possible_word2(std::string word) {

  std::vector<std::tuple<int, int>> indexs;
  const char key = word[0];
  
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      if(blocks[i][j]) {
        if(blocks[i][j]->key == key) {
          indexs.push_back(std::tuple<int, int>(i, j));
        }
      }
    }
  }

  CCLOG("*******************************************************");

  for(auto& index : indexs) {
    path.clear();
    CCLOG("찾을 단어: %s", word.c_str());
    CCLOG("시작 위치: %d, %d", std::get<0>(index), std::get<1>(index));
    auto r = find_word(index, 0, word);
    if(r == 100) {
      CCLOG("(O) 단어 찾음: %s", word.c_str());
      return true;
    } else {
      CCLOG("(X) 단어 못찾음: %s", word.c_str());
    }
  }
  
  return false;
}

int HelloWorld::find_word(std::tuple<int, int> index, int depth, const std::string& word) {

  if(depth == word.size()-1) {

    for(auto& i : path) {
      auto row = std::get<0>(i);
      auto col = std::get<1>(i);
      CCLOG("%c", blocks[row][col]->key);
    }

    return 100; // found
  }

  int row = std::get<0>(index);
  int col = std::get<1>(index);
  
  CCLOG("현재 key: %c", word[depth]);
  //CCLOG("다음 key: %c", word[depth+1]);

  auto next_key = word[depth+1];
  CCLOG("찾아야할 key: %c", word[depth+1]);
  //8 way search
  if(go_top_left(index, next_key)) {
    CCLOG("왼쪽위 진행");
    //CCLOG("왼쪽위 키값: %c", blocks[row-1][col-1]->key);
    auto r = find_word(std::tuple<int, int>(row-1, col-1), depth+1, word);
    if(r == 100) {
      return 100;
    }
    
    path.pop_back();
  }

  if(go_top(index, next_key)) {
    CCLOG("위 진행");
    auto r = find_word(std::tuple<int, int>(row-1, col), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  } 

  if(go_top_right(index, next_key)) {
    CCLOG("오른쪽위 진행");
    auto r = find_word(std::tuple<int, int>(row-1, col+1), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }

  if(go_right(index, next_key)) {
    CCLOG("오른쪽 진행");
    auto r = find_word(std::tuple<int, int>(row, col+1), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }

  if(go_bottom_right(index, next_key)) {
    CCLOG("오른쪽 아래 진행");
    auto r = find_word(std::tuple<int, int>(row+1, col+1), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }

  if(go_bottom(index, next_key)) {
    CCLOG("아래 진행");
    auto r = find_word(std::tuple<int, int>(row+1, col), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }

  if(go_bottom_left(index, next_key)) {
    CCLOG("왼쪽 아래 진행");
    auto r = find_word(std::tuple<int, int>(row+1, col-1), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }

  if(go_left(index, next_key)) {
    CCLOG("왼쪽 진행");
    auto r = find_word(std::tuple<int, int>(row, col-1), depth+1, word);
    if(r == 100) {
      return 100;
    }

    path.pop_back();
  }


  return -1;
}

bool HelloWorld::is_path(std::tuple<int, int> index) {
  for(auto& tmp : path) {
    if(tmp == index) {
      CCLOG("지나온 위치: %d, %d", std::get<0>(index), std::get<1>(index));
      return true;
    }
  }
  return false;
}

bool HelloWorld::go_top_left(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(row - 1 < 0 || col -1 < 0) {
    return false;
  } 

  if(blocks[row-1][col-1] == nullptr) {
    return false;
  }

  if(blocks[row-1][col-1]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row-1, col-1);

  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true; 
}

bool HelloWorld::go_top(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);
  if(row - 1 < 0) {
    return false;
  }

  if(blocks[row-1][col] == nullptr) {
    return false;
  }

  if(blocks[row-1][col]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row-1, col);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);
 
  return true;
}

bool HelloWorld::go_top_right(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(row - 1 < 0 || col + 1 >= col_) {
    return false;
  }

  if(blocks[row-1][col+1] == nullptr) {
    return false;
  }

  if(blocks[row-1][col+1]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row-1, col+1);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true;
}

bool HelloWorld::go_right(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(col + 1 >= col_) {
    return false;
  }

  if(blocks[row][col+1] == nullptr) {
    return false;
  }

  if(blocks[row][col+1]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row, col+1);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true;
}

bool HelloWorld::go_bottom_right(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(row + 1 >= row_ || col + 1 >= col_) {
    return false;
  }

  if(blocks[row+1][col+1] == nullptr) {
    return false;
  }

  if(blocks[row+1][col+1]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row+1, col+1);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true;
}

bool HelloWorld::go_bottom(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(row + 1 >= row_) {
    return false;
  }

  if(blocks[row+1][col] == nullptr) {
    return false;
  }

  if(blocks[row+1][col]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row+1, col);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true;
}

bool HelloWorld::go_bottom_left(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(row + 1 >= row_ || col - 1 < 0) {
    return false;
  }

  if(blocks[row+1][col-1] == nullptr) {
    return false;
  }
  
  if(blocks[row+1][col-1]->key != key) {
    return false;
  }

 auto tmp = std::tuple<int, int>(row+1, col-1);
  if(is_path(tmp)) {
    return false;
  }

 path.push_back(tmp);

  return true;
}

bool HelloWorld::go_left(std::tuple<int, int> index, char key) {
  auto row = std::get<0>(index);
  auto col = std::get<1>(index);

  if(col - 1 < 0) {
    return false;
  }

  if(blocks[row][col-1] == nullptr) {
    return false;
  }

  if(blocks[row][col-1]->key != key) {
    return false;
  }

  auto tmp = std::tuple<int, int>(row, col-1);
  if(is_path(tmp)) {
    return false;
  }

  path.push_back(tmp);

  return true;
}

bool HelloWorld::is_key_next(int row, int col, char next_key) {
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      if(blocks[i][j] != nullptr) {
        if(blocks[i][j]->key == next_key) {
          if(check_possible_move(std::make_tuple(row, col), std::make_tuple(i, j))) return true;
        }
      }
    }
  }
  return false;
}

std::vector<std::tuple<int, int>> HelloWorld::get_keys(const char c) {

  std::vector<std::tuple<int, int>> v;
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      if(blocks[i][j] != nullptr) {
        if(blocks[i][j]->key == c) {
          v.push_back(std::make_tuple(i, j));
        }
      }
    }
  }
  return v;
}

/*
std::vector<std::tuple<int, int>> HelloWorld::find_next_index(char key) {
  std::vector<std::tuple<int, int>> v;

  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      auto block_ptr = blocks[i][j];
      if(block_ptr != nullptr) {
        if(block_ptr->key == key) {
          v.push_back(std::make_tuple(i, j));
        }
      }
    }
  }
  return v;
}
*/

bool HelloWorld::check_possible_move(std::tuple<int, int> index0, std::tuple<int, int> index1) {                         
                               
  if(std::abs(std::get<0>(index0) - std::get<0>(index1)) > 1 || 
     std::abs(std::get<1>(index0) - std::get<1>(index1)) > 1) {
    return false;
  }

  return true;
}

int HelloWorld::get_index(int row, int col) {
  return (row * row_) + col;
}

void HelloWorld::action_incorrect() {
  CCLOG("색 다시 밝게해주는 작업");
  auto audio = SimpleAudioEngine::getInstance();
  audio->playEffect("sound/oh_no.wav", false, 1.0f, 1.0f, 1.0f);
  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {
      end_touched_action(i, j);
    }
  }
}

void HelloWorld::replace_level_scene() {
   auto s = HelloWorld::createScene();
  Director::getInstance()->replaceScene(s); 
}

/*
bool HelloWorld::check_possible_word(std::string word) {
  
  // static assert
  if(word.size() <= 1) {
    CCLOG("단어 사이즈가 이상함");
    return false;
  }

  std::tuple<int, int> last_index = std::make_tuple(-1, -1);

  for(auto i=0; i<word.size()-1; i++) {
    std::vector<std::tuple<int, int>> tmp = get_keys(word[i]);

    const char next_key = word[i+1];

    bool is_possible = false;
    for(auto j=0; j<tmp.size(); j++) {
      if(is_key_next(std::get<0>(tmp[j]), std::get<1>(tmp[j]), next_key)) {
        CCLOG("@curr key: %c", word[i]);
        CCLOG("@next key: %c", word[i+1]);
        
        if(std::get<0>(last_index) == -1 && std::get<1>(last_index) == -1) {
          is_possible = true;
          CCLOG("계속 진행할수 있음");
        } else {
          if(check_possible_move(last_index, tmp[j])) {
            is_possible = true;
            CCLOG("계속 진행할수 있음");
          } else {
            CCLOG("한번젼으로 갈수가 없어서 진행할수 없음");
          }
        }

        // row, col 기준으로 전에 키가 존재해야함 무조건!
        last_index = tmp[j];
      }
    }

    if(!is_possible) {
      CCLOG("계속 진행할수 없음");
      return false;
    }
  }

  return true;
}
*/
