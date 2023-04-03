#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

int main()
{

	sf::RenderWindow window(sf::VideoMode(504, 504), "The Draughts");
	GraphicBoard graphicBoard;
	Board gameBoard;
	int pawNumber;
	bool isMove = false;
	float dx, dy;
	sf::Vector2f oldPosition;
	int playerTurn = 1;
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);


			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					if (graphicBoard.checkMouseClick(mousePos, pawNumber))
					{
						isMove = true;
						sf::Vector2f pawPosition = graphicBoard.getPawPosition(pawNumber);
						dx = mousePos.x - pawPosition.x;
						dy = mousePos.y - pawPosition.y;
						oldPosition.x = (mousePos.x - 28) / 56 + 1;
						oldPosition.y = (mousePos.y - 28) / 56 + 1;
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					if (isMove == true)
					{
						isMove = false;
						if ((mousePos.x > 28 && mousePos.x < 476) && (mousePos.y > 28 && mousePos.y < 476))
						{
							sf::Vector2f newCoordinates((mousePos.x - 28) / 56 + 1, (mousePos.y - 28) / 56 + 1);
							sf::Vector2f pawHitPosition;
							if (gameBoard.checkPlayerMove(oldPosition, newCoordinates, pawHitPosition,playerTurn))
							{
								graphicBoard.setFigurPosition(newCoordinates, pawNumber);
								MESSAGE gameMessage = gameBoard.getGameMessage();
								if (gameMessage == MG_HIT || gameMessage==MG_REHIT)
								{
									graphicBoard.deleteFigure(pawHitPosition);
								}
								if (gameMessage != MG_REHIT && gameMessage!=MG_BADMOVE && (newCoordinates.y==8 || newCoordinates.y==1))
								{
									int team;
									team=gameBoard.transformPaw(newCoordinates);
									if (team != 0)
									{
										graphicBoard.transformFigure(newCoordinates, team);
									}
								}
								if (gameMessage == MG_HIT || gameMessage == MG_MOVE)
								{
									playerTurn = playerTurn == 1 ? 2 : 1;
									if (!gameBoard.checkGameCondition(playerTurn))
									{
										std::cout << "Game Over" << std::endl;
									}
								}
							}
							else
							{
								graphicBoard.setFigurPosition(oldPosition, pawNumber);
							}
						}
						else
						{
							graphicBoard.setFigurPosition(oldPosition, pawNumber);
						}
					}
				}
			}
			if (isMove)
			{
				graphicBoard.changeFigurPosition(mousePos.x - dx, mousePos.y - dy, pawNumber);
			}
		}
		window.clear();
		graphicBoard.draw(window);
		window.display();
	}
	return 0;
}