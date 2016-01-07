#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
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

    
    int row = 4; // 2x2, 3x3, 4x4, 5x5;
    int col = row;
    const float margin = 10.0f;
    const float block_margin = 10.0f;
    const float default_block_size = 100.0f;


    float width = 750 - (margin * 2.0f);

    //const float each_side_size = width / 2.0f;

    float side_block_cnt = row / 2;

    float block_size = 100.0f;


    auto total_block_margin = ((row-1) * block_margin);
    block_size = (width - total_block_margin) / row;
    
    /*
    if(row % 2 == 0) {
      //block_size = (each_side_size - side_block_cnt * block_margin) / ((side_block_cnt));
    } else {
      //block_size = (each_side_size - side_block_cnt * block_margin) / ((side_block_cnt + 0.5f));
    }
    */

    CCLOG("each side block cnt: %f", block_size);


    auto last_y = visibleSize.height/4.0f;

    for(auto i=0; i<col; i++) {
      auto last_x = margin;
      
      for(auto j=0; j<row; j++) {
        auto block = ui::Button::create();
        block->setTouchEnabled(true);
        block->ignoreContentAdaptWithSize(false);
        block->setContentSize(Size(block_size, block_size));
        block->loadTextures("block.png", "block.png");

        auto x = last_x + block->getContentSize().width/2;
        last_x = x + block_margin + block->getContentSize().width/2;
    
        block->setPosition(Vec2(x, last_y));
        block->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
            if(type == ui::Widget::TouchEventType::BEGAN) {

            } else if(type == ui::Widget::TouchEventType::ENDED) {

            } else if(type == ui::Widget::TouchEventType::CANCELED) {

            }
          });
        this->addChild(block, 2);
      }

      last_y = last_y + block_size + block_margin;     
    }

    CCLOG("last y position: %f", last_y);

    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
