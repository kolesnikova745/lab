#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace sf;

int N = 35, M = 25; //// задание размеров поля
int scale = 25;
int w = scale*N;
int h = scale*M;

int direction = 2, size = 4; //// начальное направление движения, начальный размер змейки

struct Snake {
public:
	int x, y;
}snake[750];

struct Fruct {
public:
	int x, y;
}eda;

class Rules {
public:
	int health;
	bool life;
	bool start;
	bool turn;
	void game();
	void spawn();
	Rules() {
		turn = false;
		
	}
}rules;

void Rules::game() {
	if ((life == true) && (start == true)) {
		for (int i = size; i > 0; --i)       //// алгоритм передвижения змейки
		{
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}

		if (direction == 0) snake[0].y += 1;
		if (direction == 1) snake[0].x -= 1;
		if (direction == 2) snake[0].x += 1;
		if (direction == 3) snake[0].y -= 1;

		for (int i = 1; i < N; i++)                       //// проверка на столкновение со стенами
			if ((snake[0].y == 0) && (snake[0].x == i))
				life = false;

		for (int i = 1; i < M; i++)
			if ((snake[0].y == i) && (snake[0].x == 0))
				life = false;

		for (int i = 1; i < M; i++)
			if ((snake[0].y == i) && (snake[0].x == N - 1))
				life = false;

		for (int i = 1; i < N; i++)
			if ((snake[0].y == M - 1) && (snake[0].x == i))
				life = false;

		for (int i = 1; i < size; i++)
			if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) life = false;

		if (life == true) {
			if ((snake[0].x == eda.x) && (snake[0].y == eda.y)) { //// условие роста змейки
				size++;
				spawn();
			}
		}
	}
}

void Rules::spawn() {              //// генерация еды
	if (rules.start) {
		eda.x = rand() % N;
		eda.y = rand() % M;
		for (int i = 1; i < size; i++) {
			if ((snake[i].x == eda.x) && (snake[i].y == eda.y)) {
				spawn();
			}
		}
		for (int i = 0; i < N; i++)
			if ((eda.x == i) && (eda.y == 0)) spawn();

		for (int i = 1; i < N; i++)
			if ((eda.x == i) && (eda.y == M - 1)) spawn();

		for (int i = 1; i < M; i++)
			if ((eda.x == 0) && (eda.y == i)) spawn();

		for (int i = 1; i < M; i++)
			if ((eda.x == N - 1) && (eda.y == i)) spawn();
	}
}


bool startgame() {

	srand(time(0));

	rules.game();

	RenderWindow window(VideoMode(w, h), "Game Snake", Style::Close);
	window.setVerticalSyncEnabled(true);

	Font font, font2;
	font.loadFromFile("gost_b.ttf");

	Text textScore("", font, 25);
	textScore.setColor(Color::Black);
	textScore.setStyle(Text::Bold);

	Text textstart("", font, 35);
	textstart.setColor(Color::White);
	textstart.setStyle(Text::Bold);

	Text textgameover("", font, 55);
	textgameover.setColor(Color::Red);
	textgameover.setStyle(Text::Bold);

	Text textEndScore("", font, 35);
	textEndScore.setColor(Color::White);
	textEndScore.setStyle(Text::Bold);

	Text textRestart("", font, 35);
	textRestart.setColor(Color::White);
	textRestart.setStyle(Text::Bold);

	Text textexit("", font, 35);
	textexit.setColor(Color::White);
	textexit.setStyle(Text::Bold);

	Text textRecord("", font, 25);
	textRecord.setColor(Color::Black);
	textRecord.setStyle(Text::Bold);

	Texture area, xsnake, food, ysnake, rhead, lhead, uhead, dhead;
	Texture	wll, wll2, wll3;
	area.loadFromFile("images/grass.png");
	xsnake.loadFromFile("images/xsnake.png");
	food.loadFromFile("images/iapple.png");
	ysnake.loadFromFile("images/ysnake.png");
	rhead.loadFromFile("images/righthead.png");
	lhead.loadFromFile("images/lefthead.png");
	uhead.loadFromFile("images/uphead.png");
	dhead.loadFromFile("images/downhead.png");
	wll.loadFromFile("images/wall.png");
	wll2.loadFromFile("images/wall2.png");
	wll3.loadFromFile("images/wall3.png");

	area.setSmooth(true);
	xsnake.setSmooth(true);
	food.setSmooth(true);
	ysnake.setSmooth(true);
	wll.setSmooth(true);
	wll2.setSmooth(true);
	wll3.setSmooth(true);
	rhead.setSmooth(true);
	lhead.setSmooth(true);
	uhead.setSmooth(true);
	dhead.setSmooth(true);

	Sprite grass(area);
	Sprite oxsnake(xsnake);
	Sprite apple(food);
	Sprite oysnake(ysnake);
	Sprite righthead(rhead);
	Sprite lefthead(lhead);
	Sprite uphead(uhead);
	Sprite downhead(dhead);
	Sprite wall(wll);
	Sprite wall2(wll2);
	Sprite wall3(wll3);


	Clock clock;
	float timer = 0, delay = 0.06; //// скорость игры

	std::ofstream fout;
	std::ifstream fin;
	int record;
	int max = 0;

	snake[0].x = 5;         //// начальное положение змейки
	snake[0].y = M / 2;

	rules.spawn();

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}
		if ((!rules.life) && (!rules.start)) {
			if (Keyboard::isKeyPressed(Keyboard::Tab)) { rules.life = true; rules.start = true; return true; }
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }

		if ((!rules.life) && (rules.start)) {                //// рестарт игры
			if (Keyboard::isKeyPressed(Keyboard::R)) {
				window.clear();
				rules.life = true;
				size = 4; direction = 2;
				rules.turn = false;
				return true;
			}
		}

		if (rules.life) {                                      //// задание направления движения змейки
			if (Keyboard::isKeyPressed(Keyboard::Up))
				if ((direction != 0) && (rules.turn)) {
					direction = 3;
					rules.turn = false;
				}

			if (Keyboard::isKeyPressed(Keyboard::Down))
				if ((direction != 3) && (rules.turn)) {
					direction = 0;
					rules.turn = false;
				}
			if (Keyboard::isKeyPressed(Keyboard::Left))
				if ((direction != 2) && (rules.turn)) {
					direction = 1;
					rules.turn = false;
				}
			if (Keyboard::isKeyPressed(Keyboard::Right))
				if ((direction != 1) && (rules.turn)) {
					direction = 2;
					rules.turn = false;
				}
		}

		if (timer > delay) { timer = 0; rules.game(); rules.turn = true; }

		////////////////////////////////////// Прорисовка объектов  //////////////////////////////////////////////////////
		window.clear();

		for (int i = 1; i < N - 1; i++)
			for (int j = 1; j < M - 1; j++) {
				grass.setPosition(i*scale, j*scale);
				window.draw(grass);
			}

		if (rules.start) {
			for (int j = 1; j < size; j++) {
				if ((snake[j].y < snake[j - 1].y) || (snake[j].y > snake[j - 1].y)) {
					oysnake.setPosition(snake[j].x*scale, snake[j].y*scale);
					window.draw(oysnake);
				}
				else {
					for (int i = 1; i < size; i++) {
						oxsnake.setPosition(snake[j].x*scale, snake[j].y*scale);
						window.draw(oxsnake);
					}
				}
			}
			if (direction == 2) {
				righthead.setPosition(snake[0].x*scale, snake[0].y*scale);
				window.draw(righthead);
			}

			if (direction == 1) {
				lefthead.setPosition(snake[0].x*scale, snake[0].y*scale);
				window.draw(lefthead);
			}

			if (direction == 3) {
				uphead.setPosition(snake[0].x*scale, snake[0].y*scale);
				window.draw(uphead);
			}

			if (direction == 0) {
				downhead.setPosition(snake[0].x*scale, snake[0].y*scale);
				window.draw(downhead);
			}
		}

		for (int i = 0; i < N; i++) {
			wall.setPosition(i*scale, (M - 1)*scale);
			window.draw(wall);
		}

		for (int i = 0; i < M; i++) {
			wall2.setPosition((N - 1)*scale, i*scale);
			window.draw(wall2);
		}

		for (int i = 0; i < N; i++) {
			wall.setPosition(i*scale, 0 * scale);
			window.draw(wall);
		}

		for (int i = 0; i < M; i++) {
			wall3.setPosition(0 * scale, i*scale);
			window.draw(wall3);
		}
		if (rules.start) {
			apple.setPosition(eda.x*scale, eda.y*scale);
			window.draw(apple);
		}

		if (rules.start) {
			std::ostringstream playerScoreString;
			playerScoreString << size - 4;
			textScore.setString("SCORE: " + playerScoreString.str());
			textScore.setPosition(25, -4);
			window.draw(textScore);
		}

		if ((!rules.life) && (rules.start)) {
			std::ostringstream playerScoreString;
			playerScoreString << size - 4;
			textgameover.setString("GAME OVER");
			textgameover.setPosition(300, 100);
			window.draw(textgameover);
			textEndScore.setString("Ваш результат: " + playerScoreString.str());
			textEndScore.setPosition(300, 225);
			window.draw(textEndScore);
			textRestart.setString("Для рестарта нажмите «R»");
			textRestart.setPosition(220, 325);
			window.draw(textRestart);
			textexit.setString("Выход «Escape»");
			textexit.setPosition(315, 425);
			window.draw(textexit);
		}

		if (!rules.start) {
			textstart.setString("Начать игру «Tab»");
			textstart.setPosition(290, 250);
			window.draw(textstart);
			textexit.setString("Выход «Escape»");
			textexit.setPosition(315, 330);
			window.draw(textexit);
		}


		if (!rules.life) {
			fin.open("record.txt");
			fin >> max;
			fin.close();

			record = size - 4;

			if (record > max) max = record;

			fout.open("record.txt");
			fout << max;
			fout.close();
		}

		fin.open("record.txt");
		fin >> max;
		fin.close();

		if (rules.start) {
			std::ostringstream recordplayer;
			recordplayer << max;
			textRecord.setString("RECORD: " + recordplayer.str());
			textRecord.setPosition(720, -4);
			window.draw(textRecord);
		}
		
		window.display();
	}
}

void gameplay() {
	if (startgame()) 
	{ gameplay(); }
}

int main() {
	gameplay();
return 0; }