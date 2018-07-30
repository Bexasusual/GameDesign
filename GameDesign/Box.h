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
		switch (direction) {	//�����������ʼ״̬
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
		image.setOrigin(90, 90);	//������ת����
	}
	~Box() {}

	//ͼ����ת����
	void image_change(int dire, sf::RenderWindow &window)	//ת�򶯻�
	{
		if (dire == 1) {
			image_direction.rotate(90);		//ָ��ֱ��ת��
			for (int i = 0; i < 15; i++) {
				image.rotate(6);
				window.clear(sf::Color(100, 150, 200));
				window.draw(image);
				window.display();
				Sleep(50);
			}
		}
		else if (dire == -1) {
			image_direction.rotate(-90);	//ָ��ֱ��ת��
			for (int i = 0; i < 15; i++) {		//������ת����
				image.rotate(-6);
				window.clear(sf::Color(100, 150, 200));
				window.draw(image);
				window.display();
				Sleep(50);
			}
		}
		return;
	}   

	//�ı䷽��
	void change_direction(sf::RenderWindow &window)		//���������ȷ����ת����
	{
		srand((unsigned)time(NULL));
		int Change = ((rand() % 2) + 1);
		if (Change == 1) {
			if (++direction > 4) direction = 1;	//direction++;if(direction > 4) direction = 1;�ļ�д
			return image_change(1,window);
		}
		else {
			if (--direction < 1) direction = 4;
			return image_change(-1,window);
		}
	}

	//չʾ����ͼ
	void show_image(sf::RenderWindow &window)const	// չʾ(display)�Ĺ�����Box_Group���
	{
		window.draw(image);
		return;
	}

	//չʾ���Ӽ���ָ��ͼƬ
	void show_direcion(sf::RenderWindow &window)const //��������ʾָ���,չʾ(display)�Ĺ�����Box_Group���
	{
		window.draw(image_direction);
		return;
	}

	//��ȡ����
	int get_direction()const { return direction;}	

	//����λ��
	void set_position(float m,float n)	//�ı������Լ�ָ�꾫���λ��
	{
		x = m; y = n;
		image.setPosition(x, y);
		image_direction.setPosition(x, y);
		return;
	}

private:
	int direction;	// 1,2,3��4, �ֱ��ʾ��������
	float x; float y;	//  ��������λ��(�Ǳ�Ҫ��Ա���������ü�����)
	sf::Texture texture_image;		//  ����ͼƬ����
	sf::Texture texture_image_direction;	   //  ָ��ͼƬ����(��ʾ�������ϲ�)
	sf::Sprite image;		
	sf::Sprite image_direction;
};

class Box_Group {
public:
	Box_Group():count(2),level(1)
	{
		if (!buffer1.loadFromFile("sounds//Incorrect.wav") || !buffer2.loadFromFile("sounds//correct1.wav") //��ʼ����Ч
			|| !buffer3.loadFromFile("sounds//correct2.wav")) cout << "Error..." << endl;
		if (!font.loadFromFile("Fonts/����ܼ�������ʦ����.ttf")) cout << "Error..." << endl;	//��ʼ������
		//-*-����Ĭ���ı�����-*-//
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

		//����Box����������λ�ã����������vector
		a.set_position(420, 408);
		b.set_position(780, 408);
		vb.push_back(&a);	//ע�����˳��
		vb.push_back(&b);		
	}
	~Box_Group(){}

	//�����������ת����
	void change_Box(int Step, sf::RenderWindow &window)
	{
		for (int i = 0; i < Step; i++) {	//���������ȷ��Ҫת��������
			srand((unsigned)time(NULL));
			int order = (rand() % (count));
			vb[order]->change_direction(window);	//����ת����
			Sleep(200);
		}
		cout << "Done." << endl;
		return;
	}

	//չʾȫ������
	void show_Box_image(sf::RenderWindow &window)const	
	{
		for (unsigned int i = 0; i < vb.size(); i++) {
			vb[i]->show_image(window);		//���ø������ӵ�ͼ����
		}
		window.display();
		return;
	}

	//չʾȫ�����Ӽ�ָ��
	void show_Box_direction(sf::RenderWindow &window)const	//չʾ��̬ͼ 
	{
		for (unsigned int i = 0; i < vb.size(); i++) {	//���ø������ӵ�ָ����
			vb[i]->show_direcion(window);
		}
		window.display();
		return;
	}

	//����������ַ���
	string question_string(int i)	//��ȡ����string
	{
		int order = i + 1;//�������Ҫ����������ϼ�һ
		string a = "Which is the correct direction of Box ";
		char num[3] = { '\0' };
		num[0] = char(order + '0');
		num[1] = '?';
		string b = a + num;
		return b;
	}	 

	//��ʾ����
	void show_question(string& str, sf::RenderWindow& window)	//��string������Ŀtext
	{
		question_text.setString(str);
		window.draw(question_text);
		return;
	}

	//��ʾ�ش���ȷ
	void show_right(sf::RenderWindow& window)
	{
		window.draw(correct_text);
		window.display();
		Sleep(300);
		return;
	}

	//��ʾ�ش����
	void show_false(sf::RenderWindow& window)
	{
		window.draw(incorrect_text);
		window.display();
		Sleep(300);
		return;
	}
	
	//��ʾ�ش�ʱ
	void show_over_time(sf::RenderWindow& window)
	{
		window.draw(over_time_text);
		window.display();
		Sleep(300);
		return;
	}

	//��ȡ���⣨��ѯ�ʵ����ӵ���ţ�
	int get_question()
	{
		//ѡ��һ�����ӵķ�����Ϊ��Ŀ			
		srand((unsigned)time(NULL));
		int quiz = rand() % vb.size();
		return quiz;	//�����������
	}

	//�ж�����(�жϣ���ʾ����ʱ)
	bool isRight(int dire, sf::RenderWindow& window, sf::Time& elasp, sf::Clock& clock)
	{
		int user_answer = 1;
		Menu user_answer_menu;
		Menu::MenuResult result;
		while (1) {
			elasp = clock.getElapsedTime();		//����ʱ��̫���д�
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
			if (get_count() == 6) {		//���Ų�ͬ��Ч
				sound.setBuffer(buffer3);
				sound.play();
				Sleep(1000);
			}
			else {
				sound.setBuffer(buffer2);
				sound.play();
				Sleep(500);
			}
			show_right(window);	//�����ȷ��Ϣ
			return true;
		}
		cout << "False!" << endl;
		sound.setBuffer(buffer1);
		sound.play();
		show_false(window);	//���������Ϣ
		return false;
	}

	//��ȡ������Ĵ�С
	int size()const
	{
		return vb.size();
	}

	//��ȡ�ȼ�
	int get_count()const { return count; }

	//��ȡ�ؿ���
	int get_level()const { return level; }

	//����
	void extend()	   //��չ����������������һ������
	{
		//�������ӵ�ͬʱ���ı�ԭ�ȵ����ӵ�λ��(������������������λ��)
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
	int count;	//ȷ���������Ӹ���
	int level; //�ؿ���
	int step;	//ת���ܲ���(�ɼ���͹ؿ�������)
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