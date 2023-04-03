#include "Game.h"
#include <iostream>

GraphicBoard::GraphicBoard()
{
	int board[8][8] =
	{ 0,1,0,1,0,1,0,1,
	  1,0,1,0,1,0,1,0,
	  0,1,0,1,0,1,0,1,
	  0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,
	  2,0,2,0,2,0,2,0,
	  0,2,0,2,0,2,0,2,
	  2,0,2,0,2,0,2,0
	};

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			gBoard[i][j] = board[i][j];

	boardTexture.loadFromFile("images/board.png");
	pawTexture.loadFromFile("images/figures.png");

	boardSprite.setTexture(boardTexture);

	for (int i = 0; i < 24; i++)
	{
		figures.push_back(new sf::Sprite(pawTexture));
	}

	int k = 0;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			if (gBoard[i][j] != 0)
			{
				int n = gBoard[i][j] == 1 ? 0 : 1;
				figures[k]->setTextureRect(sf::IntRect(size * 5, size*n, size, size));
				figures[k]->setPosition(size*j + 28, size*i + 28);
				k++;
			}
}

GraphicBoard::~GraphicBoard()
{
	for (int i = 0; i < figures.size(); i++)
	{
		delete figures[i];
	}
}

void GraphicBoard::draw(sf::RenderWindow & window)
{
	window.draw(boardSprite);
	for (int i = 0; i < figures.size(); i++)
	{
		window.draw(*figures[i]);
	}
}

bool GraphicBoard::checkMouseClick(sf::Vector2i mousePos, int & pawNumber)
{
	for (int i = 0; i < figures.size(); i++)
	{
		if (figures[i]->getGlobalBounds().contains(mousePos.x, mousePos.y))
		{
			pawNumber = i;
			return true;
		}
	}
	return false;
}

void GraphicBoard::changeFigurPosition(float wX, float wY,int pawnNumber)
{
	figures[pawnNumber]->setPosition(wX, wY);
}

void GraphicBoard::setFigurPosition(sf::Vector2f position, int pawnNumber)
{
	figures[pawnNumber]->setPosition((position.x - 1)*size + 28, (position.y - 1)*size + 28);
}

void GraphicBoard::deleteFigure(sf::Vector2f position)
{
	int i = 0;
	for (auto it = figures.begin(); it < figures.end(); it++)
	{
		if (figures[i]->getGlobalBounds().contains((position.x - 1)*size + 28, (position.y - 1)*size + 28))
		{
			delete figures[i];
			figures.erase(it);
			break;
		}
		else
		{
			i++;
		}
	}

}

void GraphicBoard::transformFigure(sf::Vector2f position,int type)
{
	type--;
	for (int i = 0; i < figures.size(); i++)
	{
		if (figures[i]->getGlobalBounds().contains((position.x - 1)*size + 28, (position.y - 1)*size + 28))
		{
			if (type == 0)
			{
				figures[i]->setTextureRect(sf::IntRect(size * 4, size*0, size, size));
			}
			else
			{
				figures[i]->setTextureRect(sf::IntRect(size * 4, size * 1, size, size));
			}
			break;
		}
	}
}

Board::Board()
{
	gMessage = MG_BADMOVE;
	specialPawn = 25;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			gField[i][j] = FLD_EMPTY;
	//////////////////////////////////////////Setting up Starting position for paw
	gField[0][1] = FLD_PAW1;
	gField[0][3] = FLD_PAW1;
	gField[0][5] = FLD_PAW1;
	gField[0][7] = FLD_PAW1;
	gField[1][0] = FLD_PAW1;
	gField[1][2] = FLD_PAW1;
	gField[1][4] = FLD_PAW1;
	gField[1][6] = FLD_PAW1;
	gField[2][1] = FLD_PAW1;
	gField[2][3] = FLD_PAW1;
	gField[2][5] = FLD_PAW1;
	gField[2][7] = FLD_PAW1;

	gField[5][0] = FLD_PAW2;
	gField[5][2] = FLD_PAW2;
	gField[5][4] = FLD_PAW2;
	gField[5][6] = FLD_PAW2;
	gField[6][1] = FLD_PAW2;
	gField[6][3] = FLD_PAW2;
	gField[6][5] = FLD_PAW2;
	gField[6][7] = FLD_PAW2;
	gField[7][0] = FLD_PAW2;
	gField[7][2] = FLD_PAW2;
	gField[7][4] = FLD_PAW2;
	gField[7][6] = FLD_PAW2;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (gField[i][j] != FLD_EMPTY)
			{
				if (gField[i][j] == FLD_PAW1)
				{
					//pawList.push_back(new Queen(sf::Vector2f(j + 1, i + 1), 1));
					pawList.push_back(new ClassicPaw(sf::Vector2f(j + 1, i + 1), 1));
				}
				else
				{
					pawList.push_back(new ClassicPaw(sf::Vector2f(j + 1, i + 1), 2));
				}
			}
		}
}

Board::~Board()
{
	for (int i = 0; i < pawList.size(); i++)
	{
		delete pawList[i];
	}
}


bool Board::checkPlayerMove(sf::Vector2f startingPosition, sf::Vector2f finalPosition, sf::Vector2f &pawHitPosition, int team)
{
	hittingPossibilites.clear();
	gMessage = MG_BADMOVE;

	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->getTeam() == team)
		{
			checkHittingPossibilites(i);
		}
	}
	bool correctMove=false;
	for (int i = 0; i < hittingPossibilites.size(); i++)
	{
		if (hittingPossibilites[i] == finalPosition)
		{
			correctMove = true;
		}
	}
	if (hittingPossibilites.size() == 0)
	{
		correctMove = true;
	}
	if (correctMove == false)
	{
		return false;
	}

	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->checkPosition(startingPosition))
		{
			if (specialPawn < 25)
			{
				if (i != specialPawn)
				{
					return false;
				}
			}
			if (pawList[i]->getTeam() != team)
			{
				return false;
			}
			if (!pawList[i]->checkMove(finalPosition))
			{
				return false;
				gMessage = MG_BADMOVE;
			}
			if (checkCollision(finalPosition))
			{
				if (abs(finalPosition.x - startingPosition.x) >= 2)
				{
					if (!checkHitting(finalPosition, i,pawHitPosition))
					{
						gMessage = MG_BADMOVE;
						return false;
					}
					gMessage = MG_HIT;
				}
				if (gMessage == MG_BADMOVE)
					gMessage = MG_MOVE;
				////////////////////////Przesz³o wszystkie warunki/////////////////////////////
				gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] = FLD_EMPTY;
				if (pawList[i]->getTeam() == 1)
				{
					gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] = FLD_PAW1;
				}
				else
				{
					gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] = FLD_PAW2;
				}
				if (gMessage == MG_HIT)
				{
					gField[(int)pawHitPosition.y-1][(int)pawHitPosition.x-1] = FLD_EMPTY;
					pawList[i]->setPosition(finalPosition);
					deletePaw(pawHitPosition);
					hittingPossibilites.clear();
					for (int k = 0; k < pawList.size(); k++)
					{
						if (pawList[k]->checkPosition(finalPosition))
						{
							if (checkHittingPossibilites(k))
							{
								gMessage = MG_REHIT;
								specialPawn = k;
							}
						}
					}
				}
				else
				{
					pawList[i]->setPosition(finalPosition);
				}
				specialPawn = 25;
				return true;
			}
			gMessage = MG_BADMOVE;
			return false;
		}
	}

	gMessage = MG_BADMOVE;
	return false;
}

bool Board::checkCollision(sf::Vector2f finalPosition)
{
	if (gField[(int)finalPosition.y-1][(int)finalPosition.x-1] == FLD_EMPTY)
		return true;
	return false;
}

bool Board::checkHitting(sf::Vector2f finalPosition, int pawNumber, sf::Vector2f & pawPosition)
{
	//////////////////////////Must be different for Queen and Paw/////////////////////
	ClassicPaw *paw;
	if (paw = dynamic_cast<ClassicPaw*>(pawList[pawNumber]))
	{
		int team = pawList[pawNumber]->getTeam();
		sf::Vector2f pawHitPosition;
		if (pawList[pawNumber]->getPosition().x > finalPosition.x)
		{
			pawHitPosition.x = pawList[pawNumber]->getPosition().x - 1;
		}
		else
		{
			pawHitPosition.x = finalPosition.x - 1;
		}
		if (pawList[pawNumber]->getPosition().y > finalPosition.y)
		{
			pawHitPosition.y = pawList[pawNumber]->getPosition().y - 1;

		}
		else
		{
			pawHitPosition.y = finalPosition.y - 1;
		}
		for (int i = 0; i < pawList.size(); i++)
		{
			if (pawList[i]->checkPosition(pawHitPosition) && pawList[i]->getTeam() != team)
			{
				gMessage = MG_HIT;
				pawPosition = pawHitPosition;
				return true;
			}
		}
		return false;
	}
	else
	{
		int team = pawList[pawNumber]->getTeam();
		sf::Vector2f startingPosition = pawList[pawNumber]->getPosition();
		sf::Vector2f pawHitPosition;
		int hitNumber=0;
		for (;;)
		{
			if (startingPosition.x > finalPosition.x)
			{
				startingPosition.x--;
			}
			else
			{
				startingPosition.x++;
			}
			if (startingPosition.y > finalPosition.y)
			{
				startingPosition.y--;
			}
			else
			{
				startingPosition.y++;
			}
			if (startingPosition == finalPosition)
			{
				break;
			}
			if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] != FLD_EMPTY)
			{
				for (int i = 0; i < pawList.size(); i++)
				{
					if (pawList[i]->checkPosition(startingPosition))
					{
						if (pawList[i]->getTeam() == team)
						{
							return false;
						}
						pawHitPosition = startingPosition;
						hitNumber++;
						if (hitNumber > 1)
						{ 
							return false;
						}
					}
				}
			}
		}
		pawPosition = pawHitPosition;
		return true;
	}
}

void Board::deletePaw(sf::Vector2f pawHitPosition)
{
	int i = 0;
	for (auto it = pawList.begin(); it < pawList.end(); it++)
	{
		if (pawList[i]->checkPosition(pawHitPosition))
		{
			delete pawList[i];
			pawList.erase(it);
			break;
		}
		else
		{
			i++;
		}
	}
}

int Board::transformPaw(sf::Vector2f pawToTransform)
{
	Queen *paw;
	int i = 0;
	int team = 0;
	for (auto it = pawList.begin(); it < pawList.end(); it++)
	{
		if (paw = dynamic_cast<Queen*>(pawList[i]))
		{
			break;
		}
		if (pawList[i]->checkPosition(pawToTransform))
		{
			team = pawList[i]->getTeam();
			delete pawList[i];
			pawList.erase(it);
			pawList.push_back(new Queen(pawToTransform, team));
			return team;
		}
		else
		{
			i++;
		}
	}
	return team;
}

bool Board::checkHittingPossibilites(int pawNumber)
{
	int team = pawList[pawNumber]->getTeam();
	Queen *pawType;
	sf::Vector2f pawPosition;
	int size = hittingPossibilites.size();
	FIELD teamType = team == 1 ? FLD_PAW1 : FLD_PAW2;

	////////////////////////////////////////////////QUEEN//////////
	if (pawType = dynamic_cast<Queen*>(pawList[pawNumber]))
	{
		pawPosition = pawList[pawNumber]->getPosition();
		sf::Vector2f startingPosition = pawPosition;

		///////////////////////////////////LEFT UP///////////////////////////////////////////////////////////////////////
		while (startingPosition.x > 0 && startingPosition.y > 0)
		{
			startingPosition.x--;
			startingPosition.y--;
			if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				while (startingPosition.x > 0 && startingPosition.y > 0)
				{
					startingPosition.x--;
					startingPosition.y--;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
			else if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				while (startingPosition.x > 0 && startingPosition.y > 0)
				{
					startingPosition.x--;
					startingPosition.y--;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
		startingPosition = pawPosition;
		//////////////////////////////////////////////LEFT DOWN//////////////////////////////////////////////////////////
		while (startingPosition.x > 0 && startingPosition.y < 9)
		{
			startingPosition.x--;
			startingPosition.y++;
			if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				while (startingPosition.x > 0 && startingPosition.y < 9)
				{
					startingPosition.x--;
					startingPosition.y++;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
			else if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				while (startingPosition.x > 0 && startingPosition.y < 9)
				{
					startingPosition.x--;
					startingPosition.y++;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
		////////////////////////////////////RIGHT UP//////////////////////////////////////////////
		startingPosition = pawPosition;
		while (startingPosition.x < 9 && startingPosition.y > 0)
		{
			startingPosition.x++;
			startingPosition.y--;
			if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				while (startingPosition.x < 9 && startingPosition.y > 0)
				{
					startingPosition.x++;
					startingPosition.y--;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
			else if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				while (startingPosition.x < 9 && startingPosition.y > 0)
				{
					startingPosition.x++;
					startingPosition.y--;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
		startingPosition = pawPosition;
		////////////////////////////////////////////////RIGHT DOWN////////////////////////////////////
		while (startingPosition.x < 9 && startingPosition.y < 9)
		{
			startingPosition.x++;
			startingPosition.y++;
			if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				while (startingPosition.x < 9 && startingPosition.y < 9)
				{
					startingPosition.x++;
					startingPosition.y++;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
			else if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				while (startingPosition.x < 9 && startingPosition.y < 9)
				{
					startingPosition.x++;
					startingPosition.y++;
					if (gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] == FLD_EMPTY)
					{
						hittingPossibilites.push_back(sf::Vector2f(startingPosition.x, startingPosition.y));
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
	}
	//////////////////////////////////////////////CLASSIC PAW//////////////////////////
	else
	{
		pawPosition = pawList[pawNumber]->getPosition();
		if (pawPosition.x - 2 > 0 && pawPosition.y - 2 > 0 && gField[(int)pawPosition.y - 3][(int)pawPosition.x - 3]==FLD_EMPTY)
		{
			if (gField[(int)pawPosition.y - 2][(int)pawPosition.x - 2] ==FLD_PAW1 && teamType==FLD_PAW2)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x - 2, pawPosition.y - 2));
			}
			else if (gField[(int)pawPosition.y - 2][(int)pawPosition.x - 2] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x - 2, pawPosition.y - 2));
			}
		}
		if (pawPosition.x - 2 > 0 && pawPosition.y + 2 < 9 && gField[(int)pawPosition.y + 1][(int)pawPosition.x - 3] == FLD_EMPTY)
		{
			if (gField[(int)pawPosition.y][(int)pawPosition.x - 2] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x - 2, pawPosition.y + 2));
			}
			else if (gField[(int)pawPosition.y][(int)pawPosition.x - 2] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x - 2, pawPosition.y + 2));
			}
		}
		if (pawPosition.x + 2 < 9 && pawPosition.y - 2 > 0 && gField[(int)pawPosition.y - 3][(int)pawPosition.x + 1] == FLD_EMPTY)
		{
			if (gField[(int)pawPosition.y - 2][(int)pawPosition.x] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x + 2, pawPosition.y - 2));
			}
			else if (gField[(int)pawPosition.y - 2][(int)pawPosition.x] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x + 2, pawPosition.y - 2));
			}
		}
		if (pawPosition.x + 2 < 9 && pawPosition.y + 2 < 9 && gField[(int)pawPosition.y + 1][(int)pawPosition.x + 1] == FLD_EMPTY)
		{
			if (gField[(int)pawPosition.y][(int)pawPosition.x] == FLD_PAW1 && teamType == FLD_PAW2)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x + 2, pawPosition.y + 2));
			}
			else if (gField[(int)pawPosition.y][(int)pawPosition.x] == FLD_PAW2 && teamType == FLD_PAW1)
			{
				hittingPossibilites.push_back(sf::Vector2f(pawPosition.x + 2, pawPosition.y + 2));
			}
		}
	}
	if (hittingPossibilites.size() == size)
	{
		return false;
	}
	return true;
}

bool Board::checkGameCondition(int team)
{
	hittingPossibilites.clear();
	int pawNumber=0;
	bool isMove = false;
	Queen * paw;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->getTeam() == team)
		{
			pawNumber++;
			sf::Vector2f pawPosition = pawList[i]->getPosition();
			checkHittingPossibilites(i);
			if (paw = dynamic_cast<Queen*>(pawList[i]) )
			{ 
				if (team == 1)
				{
					if (pawPosition.x - 1 > 0 && pawPosition.y - 1 > 0 && gField[(int)pawPosition.y - 2][(int)pawPosition.x - 2] == FLD_EMPTY)
					{
						isMove = true;
						break;
					}
					else if (pawPosition.x + 1 < 9 && pawPosition.y - 1 > 0 && gField[(int)pawPosition.y - 2][(int)pawPosition.x] == FLD_EMPTY)
					{
						isMove = true;
						break;
					}
				}
				else
				{
					if (pawPosition.x - 1 > 0 && pawPosition.y + 1 < 9 && gField[(int)pawPosition.y][(int)pawPosition.x - 2] == FLD_EMPTY)
					{
						isMove = true;
						break;
					}
					if (pawPosition.x + 1 < 9 && pawPosition.y + 1 < 9 && gField[(int)pawPosition.y][(int)pawPosition.x] == FLD_EMPTY)
					{
						isMove = true;
						break;
					}
				}
			}
			if (team == 1)
			{
				if (pawPosition.x - 1 > 0 && pawPosition.y + 1 < 9 && gField[(int)pawPosition.y][(int)pawPosition.x - 2] == FLD_EMPTY)
				{
					isMove = true;
					break;
				}
				if (pawPosition.x + 1 < 9 && pawPosition.y + 1 < 9 && gField[(int)pawPosition.y][(int)pawPosition.x] == FLD_EMPTY)
				{
					isMove = true;
					break;
				}
			}
			else
			{
				if (pawPosition.x - 1 > 0 && pawPosition.y - 1 > 0 && gField[(int)pawPosition.y - 2][(int)pawPosition.x - 2] == FLD_EMPTY)
				{
					isMove = true;
					break;
				}
				else if (pawPosition.x + 1 < 9 && pawPosition.y - 1 > 0 && gField[(int)pawPosition.y - 2][(int)pawPosition.x] == FLD_EMPTY)
				{
					isMove = true;
					break;
				}
			}
		}
	}

	if (isMove == true || hittingPossibilites.size() != 0)
	{
		return true;
	}
	return false;
}

bool ClassicPaw::checkMove(sf::Vector2f coordniates)
{

	if (abs(getPosition().x - coordniates.x) != abs(getPosition().y - coordniates.y))
	{
		return false;
	}

	if (abs(getPosition().x -coordniates.x)>2 || abs(getPosition().x - coordniates.x) <=0)
	{
		return false;
	}
	if (getTeam() == 1)
	{
		if (coordniates.y < getPosition().y && abs(getPosition().x - coordniates.x) == 1)
			return false;
	}
	else
	{
		if (coordniates.y > getPosition().y && abs(getPosition().x - coordniates.x) == 1)
			return false;
	}
	return true;
}

ClassicPaw::ClassicPaw(sf::Vector2f position, int team) : Paw(position,team)
{
}

bool Paw::checkPosition(sf::Vector2f position)
{
	if (this->position.x == position.x && this->position.y == position.y)
	{
		return true;
	}
	return false;
}

bool Queen::checkMove(sf::Vector2f coordinates)
{
	sf::Vector2f currentPosition = getPosition();

	if (abs(currentPosition.x - coordinates.x) != abs(currentPosition.y - coordinates.y))
	{
		return false;
	}
	else if (currentPosition.x - coordinates.x == 0)
	{
		return false;
	}
	return true;
}

Queen::Queen(sf::Vector2f position, int team) : Paw(position,team)
{
}
