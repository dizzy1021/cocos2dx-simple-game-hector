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

#include "PlayerScene.h"
#include "SimpleAudioEngine.h"
#include <time.h>
#include "AgainScene.h"

USING_NS_CC;


Scene* PlayerScene::scene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	return scene;
}

// on "init" you need to initialize your instance
bool PlayerScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	// Load frame 
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hector.plist");

	// Instance Background Music	 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/dimension.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/dimension.mp3", true);

	// Get size screen
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// Instance Label For Title
	score = 0;
	label_score = Label::createWithTTF(StringUtils::format("Score :%d", score), "fonts/RussoOneRegular.ttf", 20);

	// position the label on the center of the screen
	label_score->setPosition(650, 460);

	// add the label as a child to this layer
	this->addChild(label_score, 1);


	// creating a keyboard event listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event * event) {

		if (_keys.find(keyCode) == _keys.end())
		{
			_keys[keyCode] = std::chrono::high_resolution_clock::now();

		}
	};
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event * event) {
		_keys.erase(keyCode);
		
	};

	// Dispatch listener keyboard
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);	

	// Spawn Player
	PlayerScene::spawnPlayer();
	//PlayerScene::spawnEnemy(1.50f);

	srand((unsigned int)time(nullptr));
	this->schedule(schedule_selector(PlayerScene::spawnEnemy), 0.5);

	 //adds contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayerScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	// Auto update
	this->scheduleUpdate();

	return true;
}

// Get condition keyboard is pressed or not
bool PlayerScene::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (_keys.find(keyCode) != _keys.end())
	{	
		return true;
	}		

	return false;
}

// Get keypress duration
double PlayerScene::keyPressedDuration(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (!keyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || !keyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || 
		!keyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) || !keyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) )
	{
		return 0;
	}

	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - _keys[keyCode]).count();
}

// function update
void PlayerScene::update(float dt)
{	
	Node::update(dt);

	Vec2 loc = player->getPosition();
	auto velocity = 1;

	if (keyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW))
	{			
		if(loc.x >= 20)
		{			
			player->setFlipX(true);
			player->setPosition(--loc.x - velocity, loc.y);		
			
		}
	}
	else if (keyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
	{			
		
		if(loc.x <= 700)
		{	
			player->setFlipX(false);
			player->setPosition(++loc.x + velocity, loc.y);
			
		}
	}
	else if (keyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW))
	{	
		if(loc.y <= 460)
		{
			player->setFlippedY(false);
			player->setPosition(loc.x, ++loc.y + velocity);
		}
	}
	else if (keyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW))
	{	
		if(loc.y >= 20)
		{
			player->setFlippedY(true);
			player->setPosition(loc.x, --loc.y - velocity);
		}
	}
}

void PlayerScene::spawnPlayer()
{

	// Get size screen
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// Instance sprite frame
	frames = getAnimation("player_walk/right_%d.png", 2);
	player = Sprite::createWithSpriteFrame(frames.front());

	// Set scale,physic body and position sprite player
	player->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 2)));
	player->setScale(0.5f);		

	// Instance Animation player sprite
	auto animation = Animation::createWithSpriteFrames(frames, 1.0f / 2);
	auto player_walks = Animate::create(animation);
	player->runAction(RepeatForever::create(player_walks));

	// PhysicBody for player
	auto playerSize = player->getContentSize();
	auto physicsBody = PhysicsBody::createBox(Size(playerSize.width, playerSize.height),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	player->setTag(10);
	physicsBody->setCollisionBitmask(1);
	physicsBody->setContactTestBitmask(true);
	player->addComponent(physicsBody);	

	// Insert sprite to scene
	this->addChild(player);

}

void PlayerScene::spawnEnemy(float dt)
{
	// Get size screen
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// Instance sprite frame
	enemy_frames = getAnimation("enemy_walk/swim_%d.png", 2);
	enemy = Sprite::createWithSpriteFrame(frames.front());
	auto s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	auto enemySize = enemy->getContentSize();
	Vec2 player_loc = player->getPosition();

	// Set scale and position sprite enemy
	enemy->setPosition(Vec2(cocos2d::random(-40, 0),
		cocos2d::random(-40, 0)));
	enemy->setScale(0.5f);

	// Instance Animation enemy sprite
	auto animation = Animation::createWithSpriteFrames(enemy_frames, 1.0f / 2);
	auto enemy_walks_animation = Animate::create(animation);
	auto enemy_move = MoveTo::create(cocos2d::random(2,5), Vec2(player_loc.x * 2, player_loc.y * 2));
	auto enemy_walks = RepeatForever::create(enemy_walks_animation);
	enemy->runAction(enemy_walks);
	enemy->runAction(Sequence::create(enemy_move, RemoveSelf::create(), nullptr));

	score = score + 1;
	label_score->setString(StringUtils::format("Score :%d", score));

	// PhysicBody for enemy	
	auto physicsBody = PhysicsBody::createBox(Size(60.0f,45.0f),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCollisionBitmask(2);
	physicsBody->setContactTestBitmask(true);
	enemy->addComponent(physicsBody);

	this->addChild(enemy, 0);
}

std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> PlayerScene::_keys;


Vector<SpriteFrame*> PlayerScene::getAnimation(const char* format, int count)
{
	SpriteFrameCache* spritecache = SpriteFrameCache::getInstance();	

	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}

bool PlayerScene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	// check if the bodies have collided
	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();

		// Instance Sound Effect
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/Collide.wav");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/Collide.wav", true);

		label_score = Label::createWithTTF(StringUtils::format("Your Score :%d", score), "fonts/RussoOneRegular.ttf", 40);

		// position the label on the center of the screen
		label_score->setPosition(360, 340);

		auto menu_again = AgainScene::create();
		menu_again->addChild(label_score);

		Director::getInstance()->pushScene(menu_again);
	}

	return true;
}

void PlayerScene::menuCloseCallback(Ref * sender)
{
	Director::getInstance()->end();
}
