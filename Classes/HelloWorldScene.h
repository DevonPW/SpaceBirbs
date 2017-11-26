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
	enum Type {
		RED,
		YELLOW,
		BLUE
	};
	Type type;
	enum State {
		WAITING,
		LOADED,
		GRABBED,
		LAUNCHED,
		DEAD
	};
	State state;

	void initialize() {
		setScale(0.25f);
		setAnchorPoint(Vec2(0.5f, 0.5f));

		PhysicsBody* bird_body = PhysicsBody::createCircle(getContentSize().width / 2.0f);
		bird_body->setDynamic(false);
		setPhysicsBody(bird_body);

		state = WAITING;
	}

	void orbit(Sprite* orbitBody, int modAngle = 0.0f) {
		if (orbitBody != NULL) {
			Vec2 birdDisplacement = (getPosition() - orbitBody->getPosition());
			float birdAngle = -MATH_RAD_TO_DEG(birdDisplacement.angle(birdDisplacement, Vec2(1, 0))) + 90;
			if (getPosition().y < orbitBody->getPosition().y) {
				birdAngle = -180 - birdAngle;
			}
			birdAngle += modAngle;
			setRotation(birdAngle);
		}
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

	//Birds
	Bird* currentBird;
	Bird* birds[3];

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

	Label* printAngle;

	//This scene's physics world
	//Reference to the physics world used within the scene. Prevents having to call: director->getRunningScene()->getPhysicsWorld() every time we want to do something
	//HAS to be static because the create function we set its value in is a static function. The compiler will complain if we try to use a non-static member in a static function
	static PhysicsWorld* physicsWorld;

	static Scene* sceneHandle;
};
