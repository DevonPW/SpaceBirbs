#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "InputHandler.h"
#include "DisplayHandler.h"

USING_NS_CC;

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

	bird1 = Sprite::create("UA/Birds/spr_Bird.png");
	bird1->setScale(0.25f);
	bird1->setAnchorPoint(Vec2(0.5f, 0.5f));
	bird1->setPosition(Vec2(200, 150));
	
	PhysicsBody* bird_body = PhysicsBody::createCircle(bird1->getContentSize().width / 2.0f);
	bird_body->setDynamic(true);
	bird1->setPhysicsBody(bird_body);

	this->addChild(bird1);

	//Create the Pig!
	pig1 = Sprite::create("UA/Enemies/spr_Pig.png");
	pig1->setScale(0.25f);
	pig1->setAnchorPoint(Vec2(0.5f, 0.8f));
	pig1->setRotation(90);
	pig1->setPosition(Vec2(1200, 500));
	this->addChild(pig1);


	//Allow for the update() function to be called by cocos
	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float deltaTime)
{
	//The main update loop!!

	//if bird in slingshot?

	//if bird has launched??

	//how much force to add

	//void UpdateLocations based on gravity??
	physicsWorld->setGravity(Vec2(0.0f, 0.0f));
	//Update based on gravity???
	UpdatePositions(deltaTime);

	//Update rotations
	UpdateRot(deltaTime);

	//Did I hit something? 
	CheckCollision();

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
	//If we click within a 50 pixel radius of the slingshot position, we're grabbing the bird. 
	if (INPUTS->getMouseButtonPress(MouseButton::BUTTON_LEFT))
	{
		//get starting position to use as reference for figuring out power
	
		
	}
	//If mouse is held
	if (INPUTS->getMouseButton(MouseButton::BUTTON_LEFT))
	{
		angle++;
		radius = 5;
		Vec2 temp;
		temp = INPUTS->getMousePosition();
		//bird1->setPosition();
		bird1->setPosition(INPUTS->getMousePosition());
		//Vec2 temp;
		//temp = INPUTS->getMousePosition();
		//std::cout << temp.x << "," << temp.y << std::endl;
		////Replace with pull back from slingshot 
	}
	//If we let go of the left mouse button and we're holding a bird, let go and launch the bird!
	if (INPUTS->getMouseButtonRelease(MouseButton::BUTTON_LEFT))
	{
		//use slingshot position plus where mouse is released to figure out velocity
		bird1->getPhysicsBody()->setVelocity((slingshotBack->getPosition() - INPUTS->getMousePosition()) * 5.0f);
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
	if (INPUTS->getKeyPress(KeyCode::KEY_G))
	{
		bird1->setPosition(200, 150);
		bird1->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
	else if (INPUTS->getKeyRelease(KeyCode::KEY_G))
	{
		//When the user releases the G key, we are resetting gravity to the proper value.
		//This is where it is useful that we got the reference to the physics world in the create scene function
		//IMPORTANT NOTE: You might remember gravity is defined as -9.81 m/s/s from physics. Cocos2D's physics engines work on a 10x scale. You can use -9.81 but your forces will have to be dialed down to compensate
		
	}

}

void HelloWorld::UpdatePositions(float dt)
{
	

}

void HelloWorld::UpdateRot(float dt)
{

}

void HelloWorld::CheckCollision()
{
	
}





void HelloWorld::DrawWorld()
{
	//Creating the background
	background = Sprite::create("UA/Background/bg.jpg");
	background->setAnchorPoint(Vec2(0.5f, 0.5f));
	background->setPosition(director->getWinSizeInPixels().width / 2, director->getWinSizeInPixels().height / 2);
	this->addChild(background, -100);

	//Create Small Planet for slingshot to sit on
	planetSmall = Sprite::create("UA/Level/planet.png");
	planetSmall->setPosition(200, 100);
	planetSmall->setScale(0.30f);
	planetSmall->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(planetSmall);

	//Create Big Planet for the pigs!
	planetBig = Sprite::create("UA/Level/planet.png");
	planetBig->setPosition(1000, 500);
	planetBig->setScale(0.75f);
	planetBig->setAnchorPoint(Vec2(0.5f, 0.5f));
	PhysicsBody* planet_body = PhysicsBody::createCircle(planetBig->getContentSize().width / 2.0f);
	planet_body->setDynamic(false);
	planetBig->setPhysicsBody(planet_body);
	this->addChild(planetBig);
	//Create Gravity Radius
	planetG = Sprite::create("UA/Level/circ.png");
	planetG->setPosition(1000, 500);
	planetG->setScale(2.25f);
	planetG->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(planetG, -1);

	//Creating the slingshot
	slingshotBack = Sprite::create("UA/Level/SlingshotBack.png");
	slingshotBack->setPosition(225, 200);
	slingshotBack->setAnchorPoint(Vec2(0.5f, 0.5f));
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