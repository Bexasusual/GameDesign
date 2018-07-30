#pragma once
#include "Box.h"
#include "targetver.h"
#include <tchar.h>
#include <map>
#include <cassert>

class Game {

public:
	static void Start();

private:
	static sf::RenderWindow window;

	static bool isOff;	//���ֿ���

	static bool replay;	//�Ƿ���;�˳���Ϸ

	static bool isFirst; //�Ƿ��һ�δ���Play״̬
	
	static double score;

	static int chance;

	enum GameState {		//��ʾ״̬
		Uninitialized, Exit,Set,
		Menuing, Play, Help, Scoring
	};
	static GameState state;	
	
	static void Gameloop(Box_Group& bg, sf::Sprite& sprite, sf::Text& stage_text, string filename);
	//��Ϸѭ��

	static bool IsQuit();	//�ж��Ƿ������Ϸ
	
	static void ShowHelp();		//չʾ��������

	static void ShowMenu();		//��Ӧ�˵�

	static void ShowSet(string filename);	//���ý���

	static void ShowScore(string filename);	//չʾ�ɼ�

	static void UpdateScore(int score, string filename);	//ˢ�³ɼ�

	static void poll_image(Box_Group& bg, sf::Event& evt, sf::Sprite& sprite);	//��ӦBox���ת��������������ʱ�����

	static void poll_direction(Box_Group& bg, sf::Event& evt, sf::Text& stage_text);	//��ӦBox��ķ���ת������������ʱ�����

};

Game::GameState Game::state = Uninitialized;
sf::RenderWindow Game::window;
bool Game::isOff = false;
bool Game::replay = false;
bool Game::isFirst = true;
double Game::score = 0;
int Game::chance = 3;

void Game::Start()
{
	sf::Music game_music;	//��Ϸ����
	sf::Music menu_music;	//����������
	if (!menu_music.openFromFile("musics/Grace.wav"))cout << "Error1..." << endl;
	if(!game_music.openFromFile("musics/To the Moon.wav"))cout << "Error2..." << endl;
	menu_music.setLoop(true);
	game_music.setLoop(true);
	menu_music.play();
	if (state != Uninitialized) return;	//��ʼ��
	window.create(sf::VideoMode(1200, 816, 32), "Game");
	sf::Texture gameimage;
	gameimage.loadFromFile("images/GameScreen.jpg");
	sf::Sprite sprite(gameimage);
	sf::Font font;
	if (!font.loadFromFile("fonts/����ܼ�������ʦ����.ttf")) cout << "Error..." << endl;
	int isGamePlay, isMenuPlay; //��¼��Ϸ���ֺ����˵������Ƿ����ڲ���
	while (1) {
		isGamePlay = 0; isMenuPlay = 1;
		isFirst = true;
		if (!isOff) {
			game_music.stop();
			menu_music.play();
		}
		else {
			game_music.stop();
			menu_music.stop();
		}
		chance = 3;		//ÿ����Ϸʱ�ĳ�ʼ��
		score = 0;
		replay = false;
		Box_Group bg;
		sf::Text stage_text;
		stage_text.setFont(font);
		ShowMenu();
		while (!IsQuit()) {
			if (isFirst == true&&state == Play) {
				if (!isOff) {
					if (state == Play) {
						game_music.play();
						menu_music.stop();
						isGamePlay = 1;
						isMenuPlay = 0;
					}
				}
				isFirst = false;
			}
			Gameloop(bg, sprite, stage_text, "BoxGame_Score.txt");	//��ѭ��
			if (replay) break;	//��;�˳���Ϸ					
			if (!chance) {		//��Ϸ������Ĳ���
				sf::Text score_text;
				score_text.setFont(font);
				string a = "Your score is: ";
				char num[20] = { '0' };
				int Score = int(score);
				UpdateScore(Score, "BoxGame_Score.txt");
				int size = 1, index = 0, temp = Score, length = 0;
				while (temp > 0) {
					temp /= 10;
					size *= 10;
					length++;	//����
				}
				size /= 10;
				int cur_length = length;
				while (Score > 0) {
					int n = Score / size;
					num[index] = char(n + '0');
					cur_length--;
					index++;
					Score -= n * size;
					size /= 10;
				}
				if (cur_length != 0) {	//��0
					for (int i = length - cur_length; i < length; i++) {
						num[i] = '0';
					}
				}
				string b = a + num;
				score_text.setString(b);
				score_text.setCharacterSize(100);
				score_text.setFillColor(sf::Color::White);
				score_text.setStyle(sf::Text::Bold);
				score_text.setPosition(200, 200);
				window.draw(score_text);
				window.display();
				Sleep(3000);
				window.clear(sf::Color (100, 150, 200));
				break;
			}
		}
		if (state == Exit) {	//�˳���Ϸ
			game_music.stop();
			menu_music.stop();
			window.close();
			return;
		}
	}
}

void Game::UpdateScore(int score, string filename)	//���³ɼ�
{
	vector<Score> v;
	ifstream in(filename);
	if (!in)exit(-1);
	string word, m, n;
	int count = 1;
	while (in >> word) {
		if (count == 1)m = word;
		else if (count == 2)n = word;
		count++;
		if (count == 3) {	//countΪ3��ʾһ��ѧ������Ϣ�Ѿ��ռ�ȫ
			Score a(m, n);
			v.push_back(a);
			count = 1;
		}
	}
	in.close();
	Score b("1", score);	//�����³ɼ�
	cout << "Score = " << b.score_number << endl;
	v.push_back(b);
	sort(v.begin(), v.end(), Score());
	ofstream out(filename, ios::out);
	for (unsigned int i = 0; i < v.size(); i++) {
		if (i >= 10)break;		//ֻ��¼ǰʮ��
		out << i + 1 << "    " << v[i].score_number << endl;
	}
	out.close();
	return;
}

void Game::poll_direction(Box_Group& bg, sf::Event& evt, sf::Text& stage_text)	//��Ҫ�������������ֱ��Ӧ����ͼ,��ת���������ͼ,�Լ��ν�ͼ
{
	sf::Clock clock;
	sf::Time elaspsed;
	string a = "Stage ";
	char num[2] = { '0' };
	num[0] = char(bg.get_count() + '0');
	string b = a + num;
	stage_text.setString(b);
	stage_text.setCharacterSize(80);
	stage_text.setFillColor(sf::Color::White);
	stage_text.setStyle(sf::Text::Bold);
	stage_text.setPosition(875, 20);
	while (1) {
		elaspsed = clock.getElapsedTime();
		if (elaspsed.asSeconds() > 21) {
			return;		//ǿ�Ƽ���
		}
		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				state = Exit;
				return;
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Escape) {
					state = Menuing;
					return;
				}
			}
			else if (evt.type == sf::Event::MouseButtonPressed) {
				return;
			}
			else {
				window.clear(sf::Color(100, 150, 200));
				window.draw(stage_text);
				bg.show_Box_direction(window);
			}
		}
	}
}

void Game::poll_image(Box_Group& bg, sf::Event& evt, sf::Sprite& sprite) //��ͼ
{
	sf::Clock clock;
	sf::Time elaspsed;
	int quiz = bg.get_question();
	string question_str;
	question_str =  bg.question_string(quiz);
	while (1) {
		elaspsed = clock.getElapsedTime();
		if (elaspsed.asSeconds() > 11) {
			cout << "False!" << endl;
			chance--;
			bg.show_over_time(window);
			return;		
		}
		while (window.pollEvent(evt)) {
			elaspsed = clock.getElapsedTime();
			if (elaspsed.asSeconds() > 11) {
				bg.show_over_time(window);
				chance--;
				return;		//ǿ�Ƽ���������ʾ�ش����
			}
			if (evt.type == sf::Event::Closed) {
				state = Exit;
				return;
			}
			else if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Escape) {
					state = Menuing;
					return;
				}
			}
			else if (evt.type == sf::Event::MouseButtonPressed) {
				if (bg.isRight(quiz, window, elaspsed, clock)) {
					elaspsed = clock.getElapsedTime();	//������ʱ���й�
					if (bg.get_count() == 6 && bg.get_level() > 3) {
						score += 2 * (bg.get_count() * 100) * double(20 - elaspsed.asSeconds());
					}
					else {
						score += (bg.get_level() - 1) * (bg.get_count() * 100) * double(20 - elaspsed.asSeconds());
					}
				}
				else {
					score -= 1000;
					chance--;
				}
				cout << score << '\n' << endl;
				return;
			}
			else {
				window.clear(sf::Color(100, 150, 200));
				window.draw(sprite);
				bg.show_question(question_str, window);
				bg.show_Box_image(window);
			}
		}
	}
}

bool Game::IsQuit()
{
	if(state == Exit) return true;
	return false;
}

void Game::Gameloop(Box_Group& bg, sf::Sprite& sprite, sf::Text& stage_text,string filename)
{
	sf::Event cevt;
	window.pollEvent(cevt);
	int step = bg.get_level() + bg.get_count() - 1; //��ȡ��ǰ��Ҫ��ת�Ĳ���
	if (step > 8) step = 8;

	if (bg.get_level() >= 4 && bg.get_count() != 6) {		//ÿ3��Ϊһ������
		bg.extend();
		if (bg.get_count() == 6) {
			chance += 3;	//��������������
		}
	}

	switch (state) {
	case Menuing:
	{
		replay = true;
		return;
	}
	case Scoring:
	{
		return ShowScore("BoxGame_Score.txt");
	}
	case Help:
	{
		return ShowHelp();
	}
	case Set:
	{
		return ShowSet(filename);
	}
	case Play:
	{
		poll_direction(bg, cevt,stage_text);
		if (state == Menuing || state == Exit)return;
		window.clear(sf::Color(100, 150, 200));
		bg.change_Box(step, window);
		poll_image(bg, cevt, sprite);
		return;
	}
	}
}

void Game::ShowMenu()
{
	Menu mainMenu;
	Menu::MenuResult result = mainMenu.Show(window);
	switch (result)
	{
	case Menu::Exit:
		state = Exit;
		return;
	case Menu::Play:
		state = Play;
		return;
	case Menu::ShowScore:
		state = Scoring;
		return;
	case Menu::Help:
		state = Help;
		return;
	case Menu::Setting:
		state = Set;
		return;
	}
}

void Game::ShowHelp()
{
	sf::Event cevt;
	window.pollEvent(cevt);
	sf::Texture image;
	image.loadFromFile("images/Help.jpg");
	sf::Sprite sprite(image);
	window.draw(sprite);
	window.display();
	while (1) {
		while (window.pollEvent(cevt)) {
			if (cevt.type == sf::Event::KeyPressed){
				if (cevt.key.code == sf::Keyboard::Escape){
					return ShowMenu();
				}
			}
			if (cevt.type == sf::Event::Closed){
				state = Exit;
				return;
			}
		}
	}
}

void Game::ShowSet(string filename)
{
	sf::Event cevt;
	window.pollEvent(cevt);
	sf::Texture image_off;
	sf::Texture image_on;
	image_off.loadFromFile("images/Setting_Off.jpg");
	image_on.loadFromFile("images/Setting_On.jpg");
	sf::Sprite sprite_off(image_off);
	sf::Sprite sprite_on(image_on);

	Menu set_menu;
	Menu::MenuResult result;
	while (1) {
		window.clear(sf::Color(100, 150, 200));
		if (isOff)window.draw(sprite_off);
		else window.draw(sprite_on);
		window.display();
		while (1) {
			result = set_menu.GetUserSet(window);
			if (result != Menu::Nothing)break;
		}
		switch (result)
		{
		case Menu::Menuing:
		{
			replay = true;
			return;
		}
		case Menu::Off_On:	//�������ý��ڷ��غ���Ч
		{
			if (isOff) {
				isOff = false;
			}
			else {
				isOff = true;
			}
			break;
		}
		case Menu::Clear:
		{
			ofstream out(filename, ios::trunc);	//��ճɼ��ļ�
			out.close();
			break;
		}
		}
	}
}

void Game::ShowScore(string filename)	//չʾ�ɼ�
{
	ifstream in(filename);	//��ȡ�ļ�
	if (!in)exit(-1);
	sf::Event cevt;
	window.pollEvent(cevt);
	sf::Texture image;
	image.loadFromFile("images/Score.jpg");
	sf::Sprite sprite(image);
	window.draw(sprite);
	sf::Font font;
	if (!font.loadFromFile("Fonts/����ܼ�������ʦ����.ttf"))cout << "Error..." << endl;
	string word, order, number;
	int count = 1;
	float n = 1;
	while (in >> word) {
		switch (count)
		{
		case 1:
			order = word;
			count = 2;
			break;
		case 2:
			number = word;
			string cur = order + ".    " + number;
			sf::Text text;
			text.setFont(font);
			text.setString(cur);
			text.setCharacterSize(70);
			text.setFillColor(sf::Color(50,0,255));
			if(n<=5)text.setPosition(100,n * 148 - 74);
			else text.setPosition(650, (n - 5) * 148 - 74);
			n++;
			window.draw(text);
			count = 1;
			break;
		}
	}
	window.display();
	while (1) {
		while (window.pollEvent(cevt)) {
			if (cevt.type == sf::Event::KeyPressed) {
				if (cevt.key.code == sf::Keyboard::Escape) {
					return ShowMenu();
				}
			}
			if (cevt.type == sf::Event::Closed) {
				state = Exit;
				return;
			}
		}
	}
}