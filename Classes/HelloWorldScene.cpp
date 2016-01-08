#include "HelloWorldScene.h"
#include <ctime>

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
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
    row_ = (std::rand() % 10) + 2; // 2x2, 3x3, 4x4, 5x5;
    CCLOG("row: %d", row_);
    col_ = row_;

   

    const float margin = 10.0f;
    const float block_margin = 10.0f;
    const float block_img_size = 100.0f;


    float width = 750 - (margin * 2.0f);


    float block_size = 100.0f;


    auto total_block_margin = ((row_-1) * block_margin);
    block_size = (width - total_block_margin) / row_;
 
    //LOG("each side block cnt: %f", block_size);

    auto last_y = visibleSize.height - 200.0f;
    auto tag = 0;

    for(auto i=0; i<col_; i++) {
      auto last_x = margin;
      for(auto j=0; j<row_; j++) {

        auto block_info_ptr = new block_info;

        auto x = last_x + block_size/2;
        last_x = x + block_margin + block_size/2;

        auto sprite = Sprite::create("block.png");
        sprite->setPosition(Vec2(x, last_y));

        float scale_factor = block_img_size - block_size;

        if(scale_factor == 0.0f) {

        }        
        else if(scale_factor < 0.0f) {
          // 이미지보다 블럭이 더 큰 상황
          auto add_factor = std::abs(scale_factor) * 0.01f;
          scale_factor = 1.00 + add_factor; 
          //sprite->setScale(scale_factor);
          //CCLOG("scale_factor1: %f", scale_factor);
        } else {
          auto sub_factor = std::abs(scale_factor) * 0.01f;
          scale_factor = 1.00 - sub_factor; 
          //sprite->setScale(scale_factor);
          //CCLOG("scale_factor2: %f", scale_factor);
        }


        // physics 
        auto physicsBody = PhysicsBody::createBox(Size(block_size, block_size+10.0f), PhysicsMaterial(0.0f, 0.5f, 0.5f));

        physicsBody->setGravityEnable(true);
        physicsBody->setRotationEnable(false);
        sprite->setPhysicsBody(physicsBody);

        sprite->setTag(tag++);

        this->addChild(sprite, 0);

        block_info_ptr->pos = sprite->getPosition();
        block_info_ptr->sprite = sprite;
        block_info_ptr->physcis = physicsBody;

        blocks[i][j] = block_info_ptr;
      }

      last_y = last_y - (block_size + block_margin);
    }

    CCLOG("last y position: %f", last_y);

    auto bottom = Sprite::create("bottom.png");
    bottom->setPosition(Vec2(visibleSize.width/2.0f, last_y - 10.0f));
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
    return true;
  }

  is_touched = true;
  touched_blocks_.clear();
  
  return true; 
}
 
void HelloWorld::onTouchMoved(Touch* touch, Event* unused_event) {

  Point touchEnd = touch->getLocation();


  for(auto i=0; i<row_; i++) {
    for(auto j=0; j<col_; j++) {

      if(!is_touched_blocks(i, j)) {
        auto block_ptr = blocks[i][j];
        if(block_ptr != nullptr) {

          Rect boundingBox = block_ptr->sprite->boundingBox();
          //CCLOG("boundingbox width: %f", boundingBox.getMaxX() - boundingBox.getMinX());
          if (boundingBox.containsPoint(touchEnd)) {
            std::tuple<int, int> tmp = std::make_tuple(i, j);
            touched_blocks_.push_back(tmp);
          }
        }
      }
    }
  }

  /*
  int tag_end = row_ * col_;
  for(auto i=0; i<tag_end; i++) {

    if(getChildByTag(i)) {
      Rect boundingBox = getChildByTag(i)->boundingBox();
      if (boundingBox.containsPoint(touchEnd)) {

        auto it = std::find(tags_.begin(), tags_.end(), i);
        if(it == tags_.end()) {
          tags_.push_back(i);
        }

      }
    }
  }
  */

}
 
void HelloWorld::onTouchCancelled(Touch* touch, Event* unused_event) {
  CCLOG("touch canceled");
}
 
void HelloWorld::onTouchEnded(Touch* touch, Event* unused_event) {

  for(auto& tb : touched_blocks_) {
    CCLOG("row: %d, col: %d", std::get<0>(tb), std::get<1>(tb));
  }

  CCLOG("--------------------------------------");
  
  for(auto& tb : touched_blocks_) {

    auto i = std::get<0>(tb);
    auto j = std::get<1>(tb);

    auto block_ptr = blocks[i][j];

    if(block_ptr != nullptr) {
      replace_blocks(i, j);
      this->removeChild(block_ptr->sprite, true);
    }
  }

  is_touched = false;
   //Rect boundingBox = getChildByTag(i)->boundingBox();
   //if (boundingBox.containsPoint(touchEnd))
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
    blocks[0][col] = nullptr;
    return;
  }  

  for(int i=row; i>0; i--) {
    if(blocks[i-1][col]) {
      CCLOG("i: %d", i);
      CCLOG("col: %d", col);
      blocks[i][col] = blocks[i-1][col];
      blocks[i-1][col] = nullptr;
    } else {
      blocks[i][col] = nullptr;
    }
  }
}

bool HelloWorld::is_touched_blocks(int row, int col) {

  for(auto& tb : touched_blocks_) {
    if(std::get<0>(tb) == row && std::get<1>(tb) == col) {
      return true;
    }
  }

  return false;
}
