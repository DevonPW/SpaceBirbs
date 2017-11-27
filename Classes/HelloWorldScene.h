#pragma once

#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include <ui\UITextField.h>
#include <vector>

using std::vector;
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
		BLUE,
		PIG//pigs are birds
	};
	Type type;
	enum State {
		WAITING,
		LOADED,
		GRABBED,
		LAUNCHED,
		ORBITING,
		SPINNING,
		HIT,
		DEAD
	};
	State state;
	State lastState;

	Vec2 startPos = Vec2(0,0);

	void initialize() {
		setScale(0.35f);
		setAnchorPoint(Vec2(0.5f, 0.5f));
		setPosition(startPos);

		PhysicsBody* bird_body = PhysicsBody::createCircle(getContentSize().width / 2.0f);
		
		bird_body->setGravityEnable(false);
		if (type == PIG) {
			bird_body->setMass(100);
			bird_body->setDynamic(true);
			bird_body->setEnabled(false);
		}
		else {
			bird_body->setMass(100);
			bird_body->setDynamic(true);
			bird_body->setEnabled(false);
		}
		setPhysicsBody(bird_body);

		state = WAITING;
	}

	void update(float deltatime) {
		Sprite::update(deltatime);
	}

	Sprite* orbitBody1;

	void orbit(Sprite* orbitBody, float modAngle = 0.0f, float radius = 1000.0f) {
		if (state != ORBITING){
			orbitBody1 = orbitBody;
		}
		if (orbitBody1 != NULL) {
			Vec2 birdDisplacement = (getPosition() - orbitBody1->getPosition());
			float birdAngle = -MATH_RAD_TO_DEG(birdDisplacement.angle(birdDisplacement, Vec2(1, 0))) + 90;
			if (getPosition().y < orbitBody1->getPosition().y) {
				birdAngle = -180 - birdAngle;
			}
			birdAngle += modAngle;
			float targetRotation = birdAngle;

			float rotationVelocity = 2.0f;

			if (state == LAUNCHED || state == ORBITING || state == SPINNING || state == HIT) {
				if (getRotation() < targetRotation) {
					float rot = getRotation() + rotationVelocity;
					if (rot > targetRotation) {
						rot = targetRotation;
					}
					setRotation(rot);
				}
				else if (getRotation() > targetRotation) {
					float rot = getRotation() - rotationVelocity;
					if (rot < targetRotation) {
						rot = targetRotation;
					}
					setRotation(rot);
				}
				else {
					setRotation(targetRotation);
				}
			}
			else {
				setRotation(targetRotation);
			}

			if (state == SPINNING) {
				setRotation(getRotation() + 15);
			}

			Vec2 forceVector = birdDisplacement.getNormalized() * -40000.0f;

			/*if (state == LAUNCHED || state == HIT) {
				forceVector *= (birdDisplacement.getLength()) / radius;
			}*/

			forceVector = forceVector.rotateByAngle(birdDisplacement, CC_DEGREES_TO_RADIANS(getRotation()));

			getPhysicsBody()->applyForce(forceVector);
			
		}
	}

	int deathTime = -1;

	static Bird* create(const std::string& filename)
	{
		Bird *sprite = new (std::nothrow) Bird();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	bool checkCollision(Sprite* object) {
		if (getPosition().distance(object->getPosition()) <= (object->getContentSize().width / 2.0f * object->getScale()) + (getContentSize().width / 2.0f * object->getScale())) {
			return true;
		}
		else {
			return false;
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

private:

	//Engine
	Director* director;

	Sprite* cursor;

	ParticleSystem* explosion;

	int birdCount;
	int pigCount;

	//Birds
	Bird* currentBird = 0;
	//Bird* birds[3];
	vector<Bird*> birds;

	//Slingshot
	Sprite* slingshotBack;
	Sprite* slingshotFront;

	//Pigs
	//Bird* pigs[4];
	vector<Bird*> pigs;
	
	//Background
	Sprite* background;

	//Level Elements
	Sprite* planetSmall;
	Sprite* planetBig;
	Sprite* planetG;

	Label* gameOver;

	//This scene's physics world
	//Reference to the physics world used within the scene. Prevents having to call: director->getRunningScene()->getPhysicsWorld() every time we want to do something
	//HAS to be static because the create function we set its value in is a static function. The compiler will complain if we try to use a non-static member in a static function
	static PhysicsWorld* physicsWorld;

	static Scene* sceneHandle;
};
