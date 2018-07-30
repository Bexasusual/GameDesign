#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <list>

class Menu{
public:
	enum MenuResult { Nothing, Exit, Play, ShowScore, Help , Menuing, Setting,	//前六个是主菜单属性
		Off_On , Clear,			//中间三个是设置属性
		Up , Down , Left , Right}; //后四个是作答界面属性
	
	struct MenuItem
	{
	public:
		//用来存储矩形区域范围的点的模板对象
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult Show(sf::RenderWindow& window);	//显示菜单
	MenuResult GetUserAnswer(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock);	//获取用户点击
	MenuResult GetUserSet(sf::RenderWindow& window);	//获取用户设置

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);	//对在主菜单及设置界面的点击的响应
	MenuResult HandleClick(int x, int y);	//判断点击区域并返回相应动作
	MenuResult GetAnswerResponse(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock);	//对游戏进行时点击的响应

	//用来存储点击区域的容器
	std::list<MenuItem> menuItems;
};

//获取用户答案
Menu::MenuResult Menu::GetUserAnswer(sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock)		//获取用户答案的函数
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

//获取用户设置
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

//显示主菜单
Menu::MenuResult Menu::Show(sf::RenderWindow& window)	//返回MenuResult
{
	sf::Texture image;
	image.loadFromFile("images/Mainmenu.jpg");
	sf::Sprite sprite(image);

	//Play的点击范围
	MenuItem playButton;
	playButton.rect.top = 580;
	playButton.rect.left = 220;
	playButton.rect.width = 240;
	playButton.rect.height = 70;
	playButton.action = Play;

	//Exit的点击范围
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

	//把点击范围放进容器里面
	menuItems.push_back(playButton);
	menuItems.push_back(exitButton);
	menuItems.push_back(scoreButtom);
	menuItems.push_back(helpBottom);
	menuItems.push_back(setBottom);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//判断鼠标点击位置是否在矩形区域里面从而返回状态参数
Menu::MenuResult Menu::HandleClick(int x, int y)
{
	//迭代器遍历list容器里面存放的点击区域以供判断
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

//根据鼠标动作做出相应的菜单与设置响应
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
				if (result == Nothing) {	//无效点击，重新获取
					break;
				}
				return result;
			}
		}
	}
}

//对游戏中玩家点击的响应
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