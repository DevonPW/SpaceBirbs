#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "InputHandler.h"
#include "DisplayHandler.h"
#include <array>
#include <ctime>

USING_NS_CC;

int currentTime() {
	return clock() / (double)CLOCKS_PER_SEC * 1000;
}

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	Scene* scene = Scene::createWithPhysics();
	HelloWorld* layer = HelloWorld::create();

	scene->addChild(layer);

	sceneHandle = scene;

	Vec2 winSize = Director::getInstance()->getWinSizeInPixels();
	//Get the physics world from the scene so that we can work with it later
	//If we didn't do this, we would have to call director->getRunningScene()->getPhysicsWorld() every time we wanted to do something to the physics world
	physicsWorld = scene->getPhysicsWorld();
	return scene;
}

void HelloWorld::onExit()
{
	Scene::onExit();
}

void HelloWorld::onEnter()
{
	//DISPLAY->createDebugConsole(true);
	std::cout << "THE CONSOLE IS NOW OPEN" << std::endl;
	Scene::onEnter();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	//printf("Error while loading: %s\n", filename);
	//printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// super init first
	if (!Scene::init())
	{
		return false;
	}
	//Get the director from cocos so we can use it when needed
	director = Director::getInstance();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Draw objects like background,planets,slingshot, etc 
	DrawWorld();

	cursor = Sprite::create("UA/Birds/spr_Bird.png");
	cursor->setScale(0.1f);
	cursor->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(cursor);

	Bird* bird = Bird::create("UA/Birds/spr_Bird.png");
	birds.push_back(bird);
	birds[0]->startPos = planetSmall->getPosition() + Vec2(66, 66);
	birds[0]->type = Bird::RED;
	birds[0]->initialize();
	this->addChild(birds[0]);

	bird = Bird::create("UA/Birds/spr_BirdYellow.png");
	birds.push_back(bird);
	birds[1]->startPos = planetSmall->getPosition() + Vec2(-66, 66);
	birds[1]->type = Bird::YELLOW;
	birds[1]->initialize();
	this->addChild(birds[1]);

	bird = Bird::create("UA/Birds/spr_BirdBlue.png");
	birds.push_back(bird);
	birds[2]->startPos = planetSmall->getPosition() + Vec2(-95, 0);
	birds[2]->type = Bird::BLUE;
	birds[2]->initialize();
	this->addChild(birds[2]);

	//Create the Pigs

	Bird* pig = Bird::create("UA/Enemies/spr_Pig.png");
	pigs.push_back(pig);
	pigs[0]->startPos = (planetBig->getPosition() + Vec2(148, 148));
	pigs[0]->type = Bird::PIG;
	pigs[0]->initialize();
	this->addChild(pigs[0]);

	pig = Bird::create("UA/Enemies/spr_Pig.png");
	pigs.push_back(pig);
	pigs[1]->startPos = (planetBig->getPosition() + Vec2(-148, 148));
	pigs[1]->type = Bird::PIG;
	pigs[1]->initialize();
	this->addChild(pigs[1]);

	pig = Bird::create("UA/Enemies/spr_Pig.png");
	pigs.push_back(pig);
	pigs[2]->startPos = (planetBig->getPosition() + Vec2(-148, -148));
	pigs[2]->type = Bird::PIG;
	pigs[2]->initialize();
	this->addChild(pigs[2]);

	pig = Bird::create("UA/Enemies/spr_Pig.png");
	pigs.push_back(pig);
	pigs[3]->startPos = (planetBig->getPosition() + Vec2(148, -148));
	pigs[3]->type = Bird::PIG;
	pigs[3]->initialize();
	this->addChild(pigs[3]);

	/*printAngle = Label::createWithTTF(std::to_string(birdAngle), "Fonts/arial.ttf", 20.0f);
	printAngle->setAnchorPoint(Vec2(0.0f, 1.0f));
	printAngle->setPosition(0.0f, DISPLAY->getWindowSize().height);
	this->addChild(printAngle);*/

	//Allow for the update() function to be called by cocos
	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float deltaTime)
{
	cursor->setPosition(INPUTS->getMousePosition());

	for (int i = 0; i < birds.size(); i++) {
		if (birds[i]->state == Bird::WAITING) {
			birds[i]->orbit(planetSmall);
		}
		if (birds[i]->state == Bird::HIT) {
			if (birds[i]->deathTime < 0) {
				birds[i]->deathTime = currentTime() + 5000;
			}
			if (birds[i]->deathTime > 0 && currentTime() >= birds[i]->deathTime) {
				birds[i]->state = Bird::DEAD;
			}
		}
		if (birds[i]->state == Bird::DEAD) {
			if (birds[i] == currentBird) {
				currentBird = NULL;
			}
			this->removeChild(birds[i]);
			birds.erase(birds.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < pigs.size(); i++) {
		if (pigs[i]->state == Bird::HIT) {
			if (pigs[i]->deathTime < 0) {
				pigs[i]->deathTime = currentTime() + 5000;
			}
			if (pigs[i]->deathTime > 0 && currentTime() >= pigs[i]->deathTime) {
				pigs[i]->state = Bird::DEAD;
			}
		}
		if (pigs[i]->state == Bird::DEAD) {
			this->removeChild(pigs[i]);
			pigs.erase(pigs.begin() + i);
			i--;
		}
	}
	
	physicsWorld->setGravity(Vec2(0.0f, 0.0f));

	CheckCollision();

	if (currentBird != NULL) {
		if (currentBird->state == Bird::LOADED) {
			currentBird->setRotation(0);
			currentBird->setPosition(slingshotBack->getPosition());
		}
		else if (currentBird->state == Bird::GRABBED) {
			currentBird->orbit(slingshotBack, 90);
		}
		else if (currentBird->state == Bird::ORBITING) {
			currentBird->orbit(cursor);
		}
		else if (currentBird->state == Bird::SPINNING) {
			if (currentBird->orbitBody1 == NULL) {
				currentBird->setRotation(currentBird->getRotation() + 15);
			}
		}
	}


	//Check for Keyboard & Mouse Input DO NOT REMOVE
	updateInputs();
	//THIS NEEDS TO BE CALLED DO NOT REMOVE
	INPUTS->clearForNextFrame();
}

void HelloWorld::updateInputs()
{
	updateKeyboardInputs();
	updateMouseInputs();
}

void HelloWorld::updateMouseInputs()
{
	if (INPUTS->getMouseButtonPress(MouseButton::BUTTON_LEFT))
	{
	}
	//If mouse is held
	if (INPUTS->getMouseButton(MouseButton::BUTTON_LEFT))
	{
		radius = 200.0f;
		if (currentBird != NULL && currentBird->state == Bird::LOADED) {
			if (INPUTS->getMousePosition().distance(currentBird->getPosition()) <= currentBird->getContentSize().width / 2.0f * currentBird->getScale()) {
				currentBird->state = Bird::GRABBED;
				currentBird->getPhysicsBody()->setCollisionBitmask(0x01);
			}
		}
		else if (currentBird != NULL && currentBird->state == Bird::GRABBED) {
			currentBird->getPhysicsBody()->setEnabled(true);
			currentBird->getPhysicsBody()->setVelocity(Vec2(0, 0));

			if (INPUTS->getMousePosition().distance(slingshotBack->getPosition()) >= radius) {
				Vec2 displacement = INPUTS->getMousePosition() - slingshotBack->getPosition();
				currentBird->setPosition(slingshotBack->getPosition() + displacement.getNormalized() * radius);
			}
			else {
				currentBird->setPosition(INPUTS->getMousePosition());
			}
			//printAngle->setString(std::to_string(birdAngle));
		}
		else if (currentBird != NULL && (currentBird->state == Bird::LAUNCHED || currentBird->state == Bird::HIT)) {
			if (currentBird->type == Bird::YELLOW) {
				currentBird->lastState = currentBird->state;
				currentBird->state = Bird::SPINNING;
			}
			else if (currentBird->type == Bird::BLUE) {
				currentBird->lastState = currentBird->state;
				currentBird->state = Bird::ORBITING;
				currentBird->orbitBody1 = cursor;
			}
		}
	}
	//If we let go of the left mouse button and we're holding a bird, let go and launch the bird!
	if (INPUTS->getMouseButtonRelease(MouseButton::BUTTON_LEFT))
	{
		if (currentBird != NULL && currentBird->state == Bird::ORBITING) {
			currentBird->state = currentBird->lastState;
		}
		if (currentBird != NULL && currentBird->state == Bird::SPINNING) {
			currentBird->state = currentBird->lastState;
		}
		if (currentBird != NULL && currentBird->state == Bird::GRABBED) {
			Vec2 shotForce = (slingshotBack->getPosition() - currentBird->getPosition()) * 500.0f;
			float rot = currentBird->getRotation();
			shotForce = shotForce.rotateByAngle(slingshotBack->getPosition() - currentBird->getPosition(), CC_DEGREES_TO_RADIANS(rot));
			currentBird->getPhysicsBody()->applyImpulse(shotForce);
			currentBird->state = Bird::LAUNCHED;
		}
		else {
			for (int i = 0; i < birds.size(); i++) {
				if (INPUTS->getMousePosition().distance(birds[i]->getPosition()) <= birds[i]->getContentSize().width / 2.0f * birds[i]->getScale()) {
					if (birds[i]->state == Bird::WAITING) {
						if (currentBird != NULL && currentBird->state == Bird::LOADED) {
							currentBird->state = Bird::WAITING;
							currentBird->setPosition(currentBird->startPos);
							currentBird->getPhysicsBody()->setEnabled(false);
						}
						birds[i]->state = Bird::LOADED;
						currentBird = birds[i];
						break;
					}
				}

			}
		}
	}
}

void HelloWorld::updateKeyboardInputs()
{
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE))
	{
		//if you want keyboard input

	}

	//Release
	if (INPUTS->getKeyRelease(KeyCode::KEY_SPACE))
	{
		
		
	}
	//if held
	if (INPUTS->getKey(KeyCode::KEY_A))
	{

}
	if (INPUTS->getKeyPress(KeyCode::KEY_R))
	{
		currentBird->setPosition(currentBird->startPos);
		currentBird->state = Bird::WAITING;
		currentBird->getPhysicsBody()->setEnabled(false);
		currentBird->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}

}

void HelloWorld::CheckCollision()
{
	for (int i = 0; i < birds.size(); i++) {
		if (birds[i]->checkCollision(planetG) == true) {
			birds[i]->orbit(planetBig, 0.0f, planetG->getContentSize().width / 2.0f * planetG->getScale());
		}
		else {
			if (birds[i]->state != Bird::ORBITING) {
				birds[i]->orbitBody1 = NULL;
			}
		}
		//friction
		if (birds[i]->checkCollision(planetBig) == true) {
			birds[i]->state = Bird::HIT;
			birds[i]->getPhysicsBody()->setLinearDamping(0.001f);
		}
		else {
			birds[i]->getPhysicsBody()->setLinearDamping(0.0f);
		}

		//screen wrapping
		if (birds[i]->getPosition().x >= DISPLAY->getWindowSizeAsVec2().x + 200) {
			birds[i]->setPosition(Vec2(-200, birds[i]->getPositionY()));
		}
		else if (birds[i]->getPosition().x <= -200) {
			birds[i]->setPosition(Vec2(DISPLAY->getWindowSizeAsVec2().x + 200, birds[i]->getPositionY()));
		}
		if (birds[i]->getPosition().y >= DISPLAY->getWindowSizeAsVec2().y + 200) {
			birds[i]->setPosition(Vec2(birds[i]->getPositionX(), -200));
		}
		else if (birds[i]->getPosition().y <= -200) {
			birds[i]->setPosition(Vec2(birds[i]->getPositionX(), DISPLAY->getWindowSizeAsVec2().y + 200));
		}
		//collisions with pigs
		for (int j = 0; j < pigs.size(); j++) {
			if (pigs[j]->getPhysicsBody()->isEnabled() == false && birds[i]->checkCollision(pigs[j]) == true) {
				pigs[j]->getPhysicsBody()->setEnabled(true);
				pigs[j]->state = Bird::HIT;
			}
		}
	}

	for (int i = 0; i < pigs.size(); i++) {
		if (pigs[i]->checkCollision(planetG) == true) {
			pigs[i]->orbit(planetBig, 0.0f, planetG->getContentSize().width / 2.0f * planetG->getScale());
		}
		//friction
		if (pigs[i]->checkCollision(planetBig) == true) {
			pigs[i]->getPhysicsBody()->setLinearDamping(0.001f);
		}
		else {
			pigs[i]->getPhysicsBody()->setLinearDamping(0.0f);
		}

		//screen wrapping
		if (pigs[i]->getPosition().x >= DISPLAY->getWindowSizeAsVec2().x + 200) {
			pigs[i]->setPosition(Vec2(-200, pigs[i]->getPositionY()));
		}
		else if (pigs[i]->getPosition().x <= -200) {
			pigs[i]->setPosition(Vec2(DISPLAY->getWindowSizeAsVec2().x + 200, pigs[i]->getPositionY()));
		}
		if (pigs[i]->getPosition().y >= DISPLAY->getWindowSizeAsVec2().y + 200) {
			pigs[i]->setPosition(Vec2(pigs[i]->getPositionX(), -200));
		}
		else if (pigs[i]->getPosition().y <= -200) {
			pigs[i]->setPosition(Vec2(pigs[i]->getPositionX(), DISPLAY->getWindowSizeAsVec2().y + 200));
		}

		//collisions with pigs
		for (int j = 0; j < pigs.size(); j++) {
			if (pigs[j]->getPhysicsBody()->isEnabled() == false && pigs[i] != pigs[j] && pigs[i]->checkCollision(pigs[j]) == true) {
				pigs[j]->getPhysicsBody()->setEnabled(true);
				pigs[j]->state = Bird::HIT;
			}
		}
	}
}

void HelloWorld::DrawWorld()
{
	//Creating the background
	background = Sprite::create("UA/Background/bg.jpg");
	background->setAnchorPoint(Vec2(0.5f, 0.5f));
	background->setScale(1.2f);
	background->setPosition(director->getWinSizeInPixels().width / 2, director->getWinSizeInPixels().height / 2);
	this->addChild(background, -100);

	//Create Small Planet for slingshot to sit on
	planetSmall = Sprite::create("UA/Level/planet.png");
	planetSmall->setPosition(200, 100);
	planetSmall->setScale(0.30f);
	planetSmall->setAnchorPoint(Vec2(0.5f, 0.5f));
	PhysicsBody* planet_body = PhysicsBody::createCircle(planetSmall->getContentSize().width / 2.0f);
	planet_body->setDynamic(false);
	planet_body->setEnabled(false);
	planetSmall->setPhysicsBody(planet_body);
	this->addChild(planetSmall);

	//Create Big Planet for the pigs!
	planetBig = Sprite::create("UA/Level/planet.png");
	planetBig->setPosition(1200, 500);
	planetBig->setScale(0.75f);
	planetBig->setAnchorPoint(Vec2(0.5f, 0.5f));
	planet_body = PhysicsBody::createCircle(planetBig->getContentSize().width / 2.05f );
	planet_body->setDynamic(false);
	planetBig->setPhysicsBody(planet_body);
	this->addChild(planetBig);
	//Create Gravity Radius
	planetG = Sprite::create("UA/Level/circ.png");
	planetG->setPosition(1200, 500);
	planetG->setScale(2.8f);
	planetG->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(planetG, -1);

	//Creating the slingshot
	slingshotBack = Sprite::create("UA/Level/SlingshotBack.png");
	slingshotBack->setPosition(225, 240);
	slingshotBack->setAnchorPoint(Vec2(0.5f, 0.8f));
	slingshotBack->setScale(0.50f);
	this->addChild(slingshotBack, 0);	//Adding the slingshot to the world, setting the layer to 0 so it's behind the bird.
	slingshotFront = Sprite::create("UA/Level/SlingshotFront.png");
	slingshotFront->setPosition(225, 200);
	slingshotFront->setAnchorPoint(Vec2(0.5f, 0.5f));		//Set the anchor point to 0, 0, the bottom left, so when parented to the back of the slingshot the sprites line up!														
	slingshotFront->setScale(0.50f);
	this->addChild(slingshotFront, 2);	//Adding the slingshot to the world, setting the layer to 2 so it's infront the bird.

}
//Init the static physics world pointer. Set it to be a nullptr which means it points to nothing
PhysicsWorld* HelloWorld::physicsWorld = nullptr;
Scene* HelloWorld::sceneHandle = nullptr;