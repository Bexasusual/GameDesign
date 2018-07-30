#pragma once
#include "stdafx.h"
#include "Score.h"

class Box {
public:
	Box()
	{
		srand((unsigned)time(NULL));
		direction = ((rand() % 4) + 1);
		if (!texture_image.loadFromFile("images/Box.jpg")) {
			cout << "Error." << endl;
			return;
		}
		image.setTexture(texture_image);
		switch (direction) {	//分情况决定初始状态
		case 1:
			if (!texture_image_direction.loadFromFile("images/Box_Up.jpg")) {
				cout << "Error." << endl;
				return;
			}
			break;
		case 2:
			if (!texture_image_direction.loadFromFile("images/Box_Right.jpg")) {
				cout << "Error." << endl;
				return;
			}
			break;
		case 3:
			if (!texture_image_direction.loadFromFile("images/Box_Down.jpg")) {
				cout << "Error." << endl;
				return;
			}
			break;
		case 4:
			if (!texture_image_direction.loadFromFile("images/Box_Left.jpg")) {
				cout << "Error." << endl;
				return;
			}
			break;
		}
		image_direction.setTexture(texture_image_direction);
		image_direction.setOrigin(90, 90);		
		image.setOrigin(90, 90);	//设置旋转中心
	}
	~Box() {}

	//图像旋转动画
	void image_change(int dire, sf::RenderWindow &window)	//转向动画
	{
		if (dire == 1) {
			image_direction.rotate(90);		//指标直接转向
			for (int i = 0; i < 15; i++) {
				image.rotate(6);
				window.clear(sf::Color(100, 150, 200));
				window.draw(image);
				window.display();
				Sleep(50);
			}
		}
		else if (dire == -1) {
			image_direction.rotate(-90);	//指标直接转向
			for (int i = 0; i < 15; i++) {		//箱子旋转动画
				image.rotate(-6);
				window.clear(sf::Color(100, 150, 200));
				window.draw(image);
				window.display();
				Sleep(50);
			}
		}
		return;
	}   

	//改变方向
	void change_direction(sf::RenderWindow &window)		//随机函数，确定旋转方向
	{
		srand((unsigned)time(NULL));
		int Change = ((rand() % 2) + 1);
		if (Change == 1) {
			if (++direction > 4) direction = 1;	//direction++;if(direction > 4) direction = 1;的简写
			return image_change(1,window);
		}
		else {
			if (--direction < 1) direction = 4;
			return image_change(-1,window);
		}
	}

	//展示箱子图
	void show_image(sf::RenderWindow &window)const	// 展示(display)的工作由Box_Group完成
	{
		window.draw(image);
		return;
	}

	//展示箱子及其指标图片
	void show_direcion(sf::RenderWindow &window)const //箱子上显示指向标,展示(display)的工作由Box_Group完成
	{
		window.draw(image_direction);
		return;
	}

	//获取方向
	int get_direction()const { return direction;}	

	//设置位置
	void set_position(float m,float n)	//改变箱子以及指标精灵的位置
	{
		x = m; y = n;
		image.setPosition(x, y);
		image_direction.setPosition(x, y);
		return;
	}

private:
	int direction;	// 1,2,3或4, 分别表示上右下左
	float x; float y;	//  箱子中心位置(非必要成员，方便设置及调整)
	sf::Texture texture_image;		//  箱子图片名称
	sf::Texture texture_image_direction;	   //  指标图片名称(显示在箱子上层)
	sf::Sprite image;		
	sf::Sprite image_direction;
};

class Box_Group {
public:
	Box_Group():count(2),level(1)
	{
		if (!buffer1.loadFromFile("sounds//Incorrect.wav") || !buffer2.loadFromFile("sounds//correct1.wav") //初始化音效
			|| !buffer3.loadFromFile("sounds//correct2.wav")) cout << "Error..." << endl;
		if (!font.loadFromFile("Fonts/字体管家语文老师的字.ttf")) cout << "Error..." << endl;	//初始化字体
		//-*-设置默认文本属性-*-//
		question_text.setFont(font);	
		question_text.setCharacterSize(50);
		question_text.setFillColor(sf::Color::Black);
		question_text.setStyle(sf::Text::Bold);
		question_text.setPosition(130, 20);
		correct_text.setFont(font);
		correct_text.setString("Correct!");
		correct_text.setCharacterSize(120);
		correct_text.setFillColor(sf::Color::Yellow);
		correct_text.setStyle(sf::Text::Bold);
		correct_text.setPosition(350, 300);
		incorrect_text.setFont(font);
		incorrect_text.setString("Incorrect!");
		incorrect_text.setCharacterSize(120);
		incorrect_text.setFillColor(sf::Color::Red);
		incorrect_text.setStyle(sf::Text::Bold);
		incorrect_text.setPosition(350, 300);
		over_time_text.setFont(font);		
		over_time_text.setString("Out of Time!");
		over_time_text.setCharacterSize(120);
		over_time_text.setFillColor(sf::Color::Red);
		over_time_text.setStyle(sf::Text::Bold);
		over_time_text.setPosition(280, 300);

		//创建Box对象，设置其位置，并将其加入vector
		a.set_position(420, 408);
		b.set_position(780, 408);
		vb.push_back(&a);	//注意加入顺序
		vb.push_back(&b);		
	}
	~Box_Group(){}

	//随机决定并旋转箱子
	void change_Box(int Step, sf::RenderWindow &window)
	{
		for (int i = 0; i < Step; i++) {	//生成随机数确定要转动的箱子
			srand((unsigned)time(NULL));
			int order = (rand() % (count));
			vb[order]->change_direction(window);	//调用转向函数
			Sleep(200);
		}
		cout << "Done." << endl;
		return;
	}

	//展示全体箱子
	void show_Box_image(sf::RenderWindow &window)const	
	{
		for (unsigned int i = 0; i < vb.size(); i++) {
			vb[i]->show_image(window);		//调用各个箱子的图像函数
		}
		window.display();
		return;
	}

	//展示全体箱子及指标
	void show_Box_direction(sf::RenderWindow &window)const	//展示静态图 
	{
		for (unsigned int i = 0; i < vb.size(); i++) {	//调用各个箱子的指向函数
			vb[i]->show_direcion(window);
		}
		window.display();
		return;
	}

	//设置问题的字符串
	string question_string(int i)	//获取问题string
	{
		int order = i + 1;//箱子序号要在数组基础上加一
		string a = "Which is the correct direction of Box ";
		char num[3] = { '\0' };
		num[0] = char(order + '0');
		num[1] = '?';
		string b = a + num;
		return b;
	}	 

	//显示问题
	void show_question(string& str, sf::RenderWindow& window)	//由string生成题目text
	{
		question_text.setString(str);
		window.draw(question_text);
		return;
	}

	//显示回答正确
	void show_right(sf::RenderWindow& window)
	{
		window.draw(correct_text);
		window.display();
		Sleep(300);
		return;
	}

	//显示回答错误
	void show_false(sf::RenderWindow& window)
	{
		window.draw(incorrect_text);
		window.display();
		Sleep(300);
		return;
	}
	
	//显示回答超时
	void show_over_time(sf::RenderWindow& window)
	{
		window.draw(over_time_text);
		window.display();
		Sleep(300);
		return;
	}

	//获取问题（即询问的箱子的序号）
	int get_question()
	{
		//选定一个箱子的方向作为题目			
		srand((unsigned)time(NULL));
		int quiz = rand() % vb.size();
		return quiz;	//返回箱子序号
	}

	//判断正误(判断，显示，计时)
	bool isRight(int dire, sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock)
	{
		int user_answer = 1;
		Menu user_answer_menu;
		Menu::MenuResult result;
		while (1) {
			elasp = clock.getElapsedTime();		//作答时间太长判错
			if (elasp.asSeconds() > 11) {
				cout << "False!" << endl;
				show_over_time(window);
				return false;
			}
			result = user_answer_menu.GetUserAnswer(window, elasp, clock);
			if (result != Menu::Nothing)break;
		}
		switch (result) {
		case Menu::Up:
			user_answer = 1;
			break;
		case Menu::Right:
			user_answer = 2;
			break;
		case Menu::Down:
			user_answer = 3;
			break;
		case Menu::Left:
			user_answer = 4;
			break;
		}
		if (user_answer==vb[dire]->get_direction()) {
			level++;
			cout << "Right!" << endl;
			if (get_count() == 6) {		//播放不同音效
				sound.setBuffer(buffer3);
				sound.play();
				Sleep(1000);
			}
			else {
				sound.setBuffer(buffer2);
				sound.play();
				Sleep(500);
			}
			show_right(window);	//输出正确信息
			return true;
		}
		cout << "False!" << endl;
		sound.setBuffer(buffer1);
		sound.play();
		show_false(window);	//输出错误信息
		return false;
	}

	//获取箱子组的大小
	int size()const
	{
		return vb.size();
	}

	//获取等级
	int get_count()const { return count; }

	//获取关卡数
	int get_level()const { return level; }

	//升级
	void extend()	   //扩展函数，往其中增加一个箱子
	{
		//加入箱子的同时，改变原先的箱子的位置(根据箱子数量决定各位置)
		count++;
		level = 1;
		switch (count) {	
		case 3:
			vb[0]->set_position(600, 200);
			vb[1]->set_position(380, 510);
			c.set_position(820, 510);
			vb.push_back(&c);
			break;
		case 4:
			vb[0]->set_position(420, 228);
			vb[1]->set_position(780, 228);
			vb[2]->set_position(420, 588);
			d.set_position(780, 588);
			vb.push_back(&d);
			break;
		case 5:
			vb[0]->set_position(390, 198);
			vb[1]->set_position(810, 198);
			vb[2]->set_position(600, 408);
			vb[3]->set_position(390, 618);
			e.set_position(810, 618);
			vb.push_back(&e);
			break;
		case 6:
			vb[0]->set_position(290, 228);
			vb[1]->set_position(600, 228);
			vb[2]->set_position(910, 228);
			vb[3]->set_position(290, 588);
			vb[4]->set_position(600, 588);
			f.set_position(910, 588);
			vb.push_back(&f);
			break;
		}
	}

private:
	int count;	//确定级别及箱子个数
	int level; //关卡数
	int step;	//转动总步数(由级别和关卡数决定)
	sf::SoundBuffer buffer1, buffer2, buffer3;
	sf::Sound sound;
	sf::Font font;
	sf::Text question_text;
	sf::Text correct_text;
	sf::Text incorrect_text;
	sf::Text over_time_text;
	vector<Box *> vb;
	Box a, b, c, d, e, f;	
};