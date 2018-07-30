#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <list>

class Menu{
public:
	enum MenuResult { Nothing, Exit, Play, ShowScore, Help , Menuing, Setting,	//ǰ���������˵�����
		Off_On , Clear,			//�м���������������
		Up , Down , Left , Right}; //���ĸ��������������
	
	struct MenuItem
	{
	public:
		//�����洢��������Χ�ĵ��ģ�����
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult Show(sf::RenderWindow& window);	//��ʾ�˵�
	MenuResult GetUserAnswer(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock);	//��ȡ�û����
	MenuResult GetUserSet(sf::RenderWindow& window);	//��ȡ�û�����

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);	//�������˵������ý���ĵ������Ӧ
	MenuResult HandleClick(int x, int y);	//�жϵ�����򲢷�����Ӧ����
	MenuResult GetAnswerResponse(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock);	//����Ϸ����ʱ�������Ӧ

	//�����洢������������
	std::list<MenuItem> menuItems;
};

//��ȡ�û���
Menu::MenuResult Menu::GetUserAnswer(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock)		//��ȡ�û��𰸵ĺ���
{
	MenuItem leftBottom;
	leftBottom.rect.top = 700;
	leftBottom.rect.left = 320;
	leftBottom.rect.width = 110;
	leftBottom.rect.height = 110;
	leftBottom.action = Left;
	
	MenuItem upBottom;
	upBottom.rect.top = 700;
	upBottom.rect.left = 480;
	upBottom.rect.width = 100;
	upBottom.rect.height = 110;
	upBottom.action = Up;

	MenuItem downBottom;
	downBottom.rect.top = 700;
	downBottom.rect.left = 620;
	downBottom.rect.width = 100;
	downBottom.rect.height = 110;
	downBottom.action = Down;

	MenuItem rightBottom;
	rightBottom.rect.top = 700;
	rightBottom.rect.left = 770;
	rightBottom.rect.width = 110;
	rightBottom.rect.height = 110;
	rightBottom.action = Right;

	menuItems.push_back(leftBottom);
	menuItems.push_back(upBottom);
	menuItems.push_back(downBottom);
	menuItems.push_back(rightBottom);

	return GetAnswerResponse(window, elasp, clock);

}

//��ȡ�û�����
Menu::MenuResult Menu::GetUserSet(sf::RenderWindow& window)
{	
	MenuItem soundBottom;
	soundBottom.rect.top = 450;
	soundBottom.rect.left = 550;
	soundBottom.rect.width = 100;
	soundBottom.rect.height = 90;
	soundBottom.action = Off_On;

	MenuItem scoreBottom;
	scoreBottom.rect.top = 240;
	scoreBottom.rect.left = 500;
	scoreBottom.rect.width = 200;
	scoreBottom.rect.height = 60;
	scoreBottom.action = Clear;

	MenuItem backBottom;
	backBottom.rect.top = 640;
	backBottom.rect.left = 870;
	backBottom.rect.width = 150;
	backBottom.rect.height = 70;
	backBottom.action = Menuing;

	menuItems.push_back(soundBottom);
	menuItems.push_back(scoreBottom);
	menuItems.push_back(backBottom);

	return GetMenuResponse(window);
}

//��ʾ���˵�
Menu::MenuResult Menu::Show(sf::RenderWindow& window)	//����MenuResult
{
	sf::Texture image;
	image.loadFromFile("images/Mainmenu.jpg");
	sf::Sprite sprite(image);

	//Play�ĵ����Χ
	MenuItem playButton;
	playButton.rect.top = 580;
	playButton.rect.left = 220;
	playButton.rect.width = 240;
	playButton.rect.height = 70;
	playButton.action = Play;

	//Exit�ĵ����Χ
	MenuItem exitButton;
	exitButton.rect.top = 700;
	exitButton.rect.left = 715;
	exitButton.rect.width = 240;
	exitButton.rect.height = 70;
	exitButton.action = Exit;

	//Score...
	MenuItem scoreButtom;
	scoreButtom.rect.top = 700;
	scoreButtom.rect.left = 220;
	scoreButtom.rect.width = 240;
	scoreButtom.rect.height = 70;
	scoreButtom.action = ShowScore;

	//Help...
	MenuItem helpBottom;
	helpBottom.rect.top = 580;
	helpBottom.rect.left = 715;
	helpBottom.rect.width = 240;
	helpBottom.rect.height = 70;
	helpBottom.action = Help;

	//Setting...
	MenuItem setBottom;
	setBottom.rect.top = 766;
	setBottom.rect.left = 1150;
	setBottom.rect.width = 50;
	setBottom.rect.height = 50;
	setBottom.action = Setting;

	//�ѵ����Χ�Ž���������
	menuItems.push_back(playButton);
	menuItems.push_back(exitButton);
	menuItems.push_back(scoreButtom);
	menuItems.push_back(helpBottom);
	menuItems.push_back(setBottom);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//�ж������λ���Ƿ��ھ�����������Ӷ�����״̬����
Menu::MenuResult Menu::HandleClick(int x, int y)
{
	//����������list���������ŵĵ�������Թ��ж�
	std::list<MenuItem>::iterator it;

	for (it = menuItems.begin(); it != menuItems.end(); it++){
		sf::Rect<int> menuItemRect = (*it).rect;
		if ((menuItemRect.height + menuItemRect.top) > y
			&& menuItemRect.top < y
			&& menuItemRect.left < x
			&& (menuItemRect.width + menuItemRect.left) > x){
			return (*it).action;
		}
	}

	return Nothing;
}

//������궯��������Ӧ�Ĳ˵���������Ӧ
Menu::MenuResult Menu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while (1){
		while (window.pollEvent(menuEvent))	{
			if (menuEvent.type == sf::Event::Closed){
				return Exit;
			}

			if (menuEvent.type == sf::Event::MouseButtonPressed){
				MenuResult result = HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
				if (result == Nothing) {	//��Ч��������»�ȡ
					break;
				}
				return result;
			}
		}
	}
}

//����Ϸ����ҵ������Ӧ
Menu::MenuResult Menu::GetAnswerResponse(sf::RenderWindow& window,sf::Time& elasp,sf::Clock& clock)
{
	sf::Event answerEvent;

	while (1) {
		while (window.pollEvent(answerEvent)) {
			elasp = clock.getElapsedTime();
			if (elasp.asSeconds() > 11)return Nothing;
			if (answerEvent.type == sf::Event::MouseButtonPressed) {
				MenuResult result = HandleClick(answerEvent.mouseButton.x, answerEvent.mouseButton.y);
				return result;
			}

			if (answerEvent.type == sf::Event::Closed) {
				return Exit;
			}
		}
	}
}