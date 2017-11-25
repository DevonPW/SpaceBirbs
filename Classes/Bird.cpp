#include "Bird.h"

Bird::Bird() {
	create("UA/Birds/spr_Bird.png");
	setScale(0.25f);
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setPosition(Vec2(200, 150));

	PhysicsBody* bird_body = PhysicsBody::createCircle(getContentSize().width / 2.0f);
	bird_body->setDynamic(true);
	setPhysicsBody(bird_body);
}

Bird::~Bird() {

}