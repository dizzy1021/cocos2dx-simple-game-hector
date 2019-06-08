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

#ifndef __PLAYER_SCENE_H__
#define __PLAYER_SCENE_H__

#include "cocos2d.h"

class PlayerScene : public cocos2d::Scene
{
public:
	virtual bool init() override;

	static cocos2d::Scene* scene();	

	// a selector callback
	void menuCloseCallback(Ref* sender);

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerScene);

	cocos2d::Sprite* player;
	cocos2d::Sprite* enemy;
	cocos2d::Label* label_score;
	cocos2d::Integer* score;

	cocos2d::Vector<cocos2d::SpriteFrame*> enemy_frames;
	cocos2d::Vector<cocos2d::SpriteFrame*> frames;

	void spawnEnemy(float dt);
	void spawnPlayer();
	bool onContactBegin(cocos2d::PhysicsContact& contact);

	bool keyPressed(cocos2d::EventKeyboard::KeyCode);
	double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);

	private:
		static std::map<cocos2d::EventKeyboard::KeyCode,
			std::chrono::high_resolution_clock::time_point> _keys;		

		cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);

	public:
		virtual void update(float delta) override;
};

#endif // __HELLOWORLD_SCENE_H__