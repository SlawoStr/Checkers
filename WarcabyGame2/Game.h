#pragma once
#include <SFML/Graphics.hpp>


class GraphicBoard
{
private:
	static const int size = 56;
	int gBoard[8][8];
	sf::Sprite boardSprite;
	sf::Texture boardTexture;
	sf::Texture pawTexture;
	std::vector<sf::Sprite*>figures;

public:
	GraphicBoard();
	~GraphicBoard();
	void draw(sf::RenderWindow &window);
	bool checkMouseClick(sf::Vector2i mousePos,int & pawNumber);
	sf::Vector2f getPawPosition(const int pawNumber) { return figures[pawNumber]->getPosition(); }
	void changeFigurPosition(float wX, float wY, int pawnNumber);
	void setFigurPosition(sf::Vector2f position, int pawnNumber);
	void deleteFigure(sf::Vector2f position);
	void transformFigure(sf::Vector2f position,int type);
};

enum FIELD{FLD_EMPTY,FLD_PAW1,FLD_PAW2,FLD_QUEEN1,FLD_QUEEN2};
enum MESSAGE{MG_HIT,MG_REHIT,MG_MOVE,MG_TRANSFORM,MG_BADMOVE};




class Paw
{
private:
	sf::Vector2f position;
	int team;
public:
	Paw(sf::Vector2f position, int team) { this->position = position; this->team = team; }
	virtual bool checkMove(sf::Vector2f coordniates)=0;
	bool checkPosition(sf::Vector2f position);
	int getTeam() { return team; }
	void setPosition(sf::Vector2f position) { this->position = position; }
	sf::Vector2f getPosition() { return position; }
};

class ClassicPaw : public Paw
{
public:
	bool checkMove(sf::Vector2f coordniates);
	ClassicPaw(sf::Vector2f position,int team);
};

class Queen : public Paw
{
public:
	bool checkMove(sf::Vector2f coordinates);
	Queen(sf::Vector2f position, int team);
};

class Board
{
private:
	FIELD gField[8][8];
	MESSAGE gMessage;
	std::vector<Paw*>pawList;
	std::vector<sf::Vector2f>hittingPossibilites;
	int specialPawn;
public:
	Board();
	~Board();
	bool checkPlayerMove(sf::Vector2f startingPosition, sf::Vector2f finalPosition,sf::Vector2f & pawHitPosition,int team);
	bool checkCollision(sf::Vector2f finalPosition);
	bool checkHitting(sf::Vector2f finalPosition,int pawNumber,sf::Vector2f & pawHitPosition);
	MESSAGE getGameMessage()const { return gMessage; }
	void deletePaw(sf::Vector2f pawHitPosition);
	int transformPaw(sf::Vector2f pawToTransform);
	bool checkHittingPossibilites(int pawNumber);
	bool checkGameCondition(int team);
};