/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	// Call Size screen and origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();    

	// Call Tint SplashScreen
	splashScreen("HelloWorld.png", "audio/misc_sound.wav");
	this->schedule(schedule_selector(HelloWorld::menuScene), 6);

    return true;
}

void HelloWorld::menuScene(float dt)
{
	auto menu_scene = MenuScene::create();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, menu_scene, Color3B(68, 68, 68)));
}

void HelloWorld::splashScreen(char image[50], char sound[50])
{
	// Call Sound Effect
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(sound);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sound);

	// Call size screen and origin
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add "HelloWorld" splash screen"
	auto splash_screen = Sprite::create(image);
	if (splash_screen == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
	 	// position the sprite on the center of the screen
		splash_screen->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add interval to hide splash screen
		auto sequence_splash = Sequence::create(TintTo::create(5, -255, 0, 0), RemoveSelf::create(), nullptr);
		splash_screen->runAction(sequence_splash);

		// add the sprite as a child to this layer
		this->addChild(splash_screen, 0);		
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
