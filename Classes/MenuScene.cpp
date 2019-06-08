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

#include "MenuScene.h"
#include "PlayerScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	// Call Music
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/mainMainMusic.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/mainMainMusic.mp3", true);

	// Call Size screen and origin
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Instance Label For Title
	auto label_hector = Label::createWithTTF("Hector", "fonts/RussoOneRegular.ttf", 50);

	// position the label on the center of the screen
	label_hector->setPosition(origin.x + visibleSize.width / 2,
	origin.y + visibleSize.height - label_hector->getContentSize().height - 50);

	// add the label as a child to this layer
	this->addChild(label_hector, 1);

	auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::Start, this));
	menu_item_1->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 2)));

	auto *menu = Menu::create(menu_item_1, NULL);
	menu->setPosition(Point(0, 10));
	this->addChild(menu);

	return true;
}

void MenuScene::Start(cocos2d::Ref* pSender)
{
	CCLOG("Play");

	// Stop Music
	this->schedule(schedule_selector(MenuScene::StopMusic), 0.1);

	// Call Sound Effect
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/click.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click.wav");

	// Call PlayerScene
	auto playerScene = PlayerScene::create();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, playerScene, Color3B(68, 68, 68)));

	

}

void MenuScene::StopMusic(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void MenuScene::menuCloseCallback(Ref * pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
