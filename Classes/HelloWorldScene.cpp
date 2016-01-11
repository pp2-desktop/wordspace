#include "HelloWorldScene.h"
#include <ctime>
#include <chrono>
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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
    //restart_button->ignoreContentAdaptWithSize(false);
    restart_button->setContentSize(Size(231, 94));
    restart_button->loadTextures("restart.png", "restart.png");
    restart_button->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height - 100));

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

    
    std::srand(std::time(0)); // use current time as seed for random generator
    row_ = (std::rand() % 6) + 2; // 2x2, 3x3, 4x4, 5x5;
    CCLOG("row: %d", row_);
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

        char key = get_rand_img_alphabet();
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
        // physics 
        auto physicsBody = PhysicsBody::createBox(Size(block_size, block_size), PhysicsMaterial(0.0f, 0.5f, 0.5f));

        physicsBody->setGravityEnable(true);
	physicsBody->setDynamic(true);
        physicsBody->setRotationEnable(false);
        physicsBody->setVelocity(Vec2(0.0f, -400.0f));
        //physicsBody->setMass(50000000.5f);
        sprite->setPhysicsBody(physicsBody);

        //sprite->setTag(tag++);
	//sprite->retain();gxs
        this->addChild(sprite, 1);

        block_info_ptr->pos = sprite->getPosition();
        block_info_ptr->sprite = sprite;
        block_info_ptr->physcis = physicsBody;
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
    auto physicsBody2 = PhysicsBody::createBox(Size(bottom->getContentSize().width, bottom->getContentSize().height), PhysicsMaterial(0.0f, 0.5f, 0.5f));

    physicsBody2->setDynamic(false);
    bottom->setPhysicsBody(physicsBody2);

    this->addChild(bottom, 0 );
    
    this->scheduleUpdate();


    auto listener = EventListenerTouchOneByOne::create(); 
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
  //CCLOG("touch begin");
  if(is_touched) {
    return false;
  }

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
  CCLOG("touch canceled");
}
 
void HelloWorld::onTouchEnded(Touch* touch, Event* unused_event) {
  CCLOG("------------------------------------");
  
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

  is_touched = false;
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
  return index / 5;
}

int HelloWorld::get_col_by_index(int index) {
  if(index == 0) {
    return 0;
  }
  return index % 5;
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
  auto tintBy = TintBy::create(1.0f, 255.0f, 196.0f, 0.0f);
  block->sprite->runAction(tintBy);
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
