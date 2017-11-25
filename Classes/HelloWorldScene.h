#pragma once

#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include <ui\UITextField.h>

using namespace cocos2d;

class Bird : public Sprite {
	Bird::Bird() {
	}

	Bird::~Bird() {

	}

public:
	void initialize() {
		setScale(0.25f);
		setAnchorPoint(Vec2(0.5f, 0.5f));
		setPosition(Vec2(200, 150));

		PhysicsBody* bird_body = PhysicsBody::createCircle(getContentSize().width / 2.0f);
		bird_body->setDynamic(true);
		setPhysicsBody(bird_body);
	}
};

class HelloWorld : public cocos2d::Scene
{
public:
	CREATE_FUNC(HelloWorld);
    static cocos2d::Scene* createScene();
	virtual void onExit();
	virtual void onEnter();
    virtual bool init();
	void update(float deltaTime);
	void updateInputs();
	void updateMouseInputs();
	void updateKeyboardInputs();

	//Stuff
	void UpdatePositions(float dt);
	void UpdateRot(float dt);
	void CheckCollision();
	

	//bools
	//Has the bird entered the planets gravity field?
	//bird held?
	//whats the active bird?


    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void DrawWorld();
	
	float radius;
	float angle;


	//Remove or it will cause problems
	float rotfun;
	//
private:

	//Engine
	Director* director;

	//Event Listeners
	//birdheld???
	//birdlaunched??

	Bird* currentBird;
	Bird* birds[3];

	//Sprite* spritename;
	//Birds
	//Bird* bird1;

	//Slingshot
	Sprite* slingshotBack;
	Sprite* slingshotFront;

	//Pigs
	Sprite* pig1;
	
	//Background
	Sprite* background;

	//Level Elements
	Sprite* planetSmall;
	Sprite* planetBig;
	Sprite* planetG;

	//This scene's physics world
	//Reference to the physics world used within the scene. Prevents having to call: director->getRunningScene()->getPhysicsWorld() every time we want to do something
	//HAS to be static because the create function we set its value in is a static function. The compiler will complain if we try to use a non-static member in a static function
	static PhysicsWorld* physicsWorld;

	static Scene* sceneHandle;
};
