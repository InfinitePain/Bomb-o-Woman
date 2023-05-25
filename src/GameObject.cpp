/*
=============================================================================
 Name        : GameObject.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "GameObject.h"
#include "Console.h"

void GameObject::setPosition(Position p) {
	this->p.setX(p.getX());
	this->p.setY(p.getY());
}

void GameObject::setPlayground(Playground* pg) {
	this->playground = pg;
}

void GameObject::setTop(GameObject* go_ptr) {
	this->top_ptr = go_ptr;
}

Position GameObject::getPosition() {
	return p;
}

char GameObject::getType() {
	return symbol;
}

void GameObject::draw(bool offset) {
	if (offset) {
		Console::zeichne_punkt(p.getX() + Configuration::PLAYGROUND_OFFSETX  + Configuration::PLAYGROUND_BORDERWIDTH, p.getY() + Configuration::PLAYGROUND_OFFSETY + Configuration::PLAYGROUND_BORDERWIDTH, symbol);
	}
	else {
		Console::zeichne_punkt(p.getX(), p.getY(), symbol);
	}
	if (top_ptr != NULL) {
		top_ptr->draw(offset);
	}
}

GameObject::GameObject() : symbol(' ') {}

GameObject::GameObject(Position p, char symbol, Playground* playground) : p(p), symbol(symbol), playground(playground) {}
