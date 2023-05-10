#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Position.h"

class Player : public GameObject
{
protected:
	bool ghost;
	bool trap;
	bool explosive;

	int player_number;
	int lives;
	int score;
	int timer;

	GameObject* bomb_ptr;

public:
	//Initialisiert alle Attribute
	Player(int player_number);

	//Funktion wertet aus ob eine Bewegung in die Richtung möglich ist
	//und für diese mit alle AKtionen aus.
	bool move(int direction);

	//Routine zur Ausführung der Spieleraktion
	bool action();

	//Dekrementiert die Anzahl der Leben
	void decreaseLives();

	//Gibt zurück ob eine Bewegung in diese Richtung möglich ist
	bool isMoveValid(int direction);

	//Verändert den Spieler in Abhängigkeit des Zustands, z.B. timer-Dekrement
	void proceed();

	//Gibt die String repräsentation des Spieler zurück.
	std::string to_string();

	//Gibt die Spielernummer zurück
	int getPlayerNumber();

	//Ausgabe der Animation mit angegebenen Radius auf der Konsole
	void explosionAnimation(int radius);

	//Funktion zur Bearbeitung eines eingesammelten PickUps
	void processPickup();

	//Gibt das Attribut lives zurück
	int getLives();

	//Gibt das Attribut bombs zurück (Optional)
	int getBombs();

	//Gibt das Attribut score zurück (Optional)
	int getScore();

	//Überschreibt die Funktion der Basisklasse 
	void draw(bool offset);
};
#endif