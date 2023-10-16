
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include"iostream"
#include <vector>
#include<list>


using namespace sf;

class Entity {
public:
	String name;
	float w, h, dx, dy, x, y, speed;
	int  health;
	bool life, onGround;//�������� ���������� ��������� ���������� �� �����
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Entity(String F, float X, float Y, float W, float H) {
		File = F;
		w = W; h = H; speed = 0; dx = 0; dy = 0;
		life = true; x = X; y = Y;
	}
	virtual void update(float time) = 0;

	FloatRect getRect() {
		return FloatRect(this->x, this->y, this->w, this->h);
	}
};




////////////////////////////////////////////////////����� ������////////////////////////
class Player:public Entity {

public:
	Music dead;
	int CurrentFrame = 0, playerScore, health = 100, heart = 2 , money = 0;
	Sound JumpSound , healthUp , moneySound;
	SoundBuffer bufferJump, healthUpBuffer , moneyBuffer;

	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	
	Player(String F, float X, float Y, float W, float H):Entity(F,X,Y,W,H) {
		dead.openFromFile("music/dead.ogg");
		//dead.setVolume(2);
	    playerScore = 0; health = 100; onGround = false;
		sprite.setTextureRect(IntRect(0, 293, w, h));
		bufferJump.loadFromFile("music/jump.wav");
		healthUpBuffer.loadFromFile("music/health_up.ogg");
		moneyBuffer.loadFromFile("music/money.ogg");
		moneySound.setBuffer(moneyBuffer);
		moneySound.setVolume(4);
		healthUp.setBuffer(healthUpBuffer);
		healthUp.setVolume(0.4);
		JumpSound.setBuffer(bufferJump);
		JumpSound.setVolume(2);
		texture.loadFromFile(File);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	void control(float time) {
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			state = left;
			speed = 0.2;
			CurrentFrame += time * 0.25;
			if (CurrentFrame > 8)CurrentFrame -= 8;
			sprite.setTextureRect(IntRect((w + 2) * CurrentFrame + (w), 293 + 2, -w, h));
		}
			
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			state = right;
			speed = 0.2;
			CurrentFrame += time * 0.25;
			if (CurrentFrame > 8)CurrentFrame -= 8;
			sprite.setTextureRect(IntRect((w + 2)*CurrentFrame, 293+2, w, h));
			
		}

		if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
			state = up; dy = -0.5; onGround = false;//�� ��������� ����� ������,�������� � ��������, ��� �� �� �� �����
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) & state == right) {
			if (CurrentFrame > 5)CurrentFrame -= 5;
			sprite.setTextureRect(IntRect(50* CurrentFrame, 100 + 2, w, h));
			speed = 0.8; 
		}
		if (Keyboard::isKeyPressed(Keyboard::Space) & state == left) {
			if (CurrentFrame > 5)CurrentFrame -= 5;
			sprite.setTextureRect(IntRect(50 * CurrentFrame + w, 100 + 2, -w, h));
			speed = 0.8; 
		}
		
	}
	void update(float time)
	{
		if (speed == 0) {
			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(0, 293, w, h));
			}
			if (dx < 0)
			{
				sprite.setTextureRect(IntRect(0+w, 293, -w, h));
			}
		}
		control(time);//������� ���������� ����������
		switch (state)//��� �������� ��������� �������� � ����������� �� ���������
		{
		case right: dx = speed; break;//��������� ���� ������
		case left: dx = -speed; break;//��������� ���� �����
		case up: break;//����� ��������� �������� ������ (�������� �� ��������)
		case down: break;//����� ��������� �� ����� ������ ��������� (�������� �� ��������)
		case jump: break;//����� ����� ���� ����� ��������
		case stay: break;//� ����� ����		
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		y += dy * time;
		checkCollisionWithMap(0, dy);//������������ ������������ �� Y
		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
		if (heart <= 0) { dead.play(); life = false; health = 0; heart = 0; }
		else if (health <= 0) { x = 550; y = 736; health = 100; heart -= 1; }
		dy = dy + 0.0015 * time;//������ ���������� � �����,����� ��� ������ ������
		speed = 0;//���� ������ �� ����� ��������� ��������� ��������
		if (health > 100) { health = 100; }
		//std::cout << x << ' ' << y << std::endl;
	}


	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == 's' || TileMap[i][j] == '0')//���� ������� ��� ������ �����? ��
				{
					if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; }// � ����� ����� �����
				}
				if (TileMap[i][j] == 'j') {
					dy = -1;
					JumpSound.play();
				}
				if (TileMap[i][j] == 'h') {
					health += 20;
					TileMap[i][j] = ' ';
					healthUp.play();
					
				}
				if (TileMap[i][j] == 'm') {
					TileMap[i][j] = ' ';
					money += 5;
					moneySound.play();
				}
			}
	}
	
};
class Enemy :public Entity {
public:
	
	Enemy(String F, float X, float Y, int W, int H , Image&image ,String name) :Entity(F, X, Y, W, H) {
		this->name = name;
		health = 100;
		image.createMaskFromColor(Color(0, 0, 34));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);

		sprite.setTextureRect(IntRect(0, 0, w, h));
		dx = 0.3;//���� ��������.���� ������ ������ ���������

	}
	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++) {//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == 's')//���� ������� ��� ������ �����, ��
				{
					if (Dy > 0) { y = i * 32 - h; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; dx = -0.3; sprite.scale(-1, 1); }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; dx = 0.3; sprite.scale(-1, 1); }// � ����� ����� �����
				}
			}
		}
	} 

	void update(float time)
	{

		if (health <= 0) { life = false; health = 0;  }
		//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
		checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		x += dx * time;
		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������

	}
	
};



class LB {
public:
	Sprite s;
	Texture t;
	int max;
	RectangleShape bar;
	Text text;
	int p_health;
	int p_heart;
	int e_health;
	int p_money;
	LB(Image&i , Text text , int p_heart , int p_money ){
		i.createMaskFromColor(Color(255,255,255));
		t.loadFromImage(i);
		s.setTexture(t);
		s.setTextureRect(IntRect(0, 0,124,10));
		bar.setFillColor(Color(0, 0, 0));//������ ������������� ������������� ������ � ���������� ������ ���������� ��������
		max = 100;
		this->p_money = p_money;
		this->p_heart = p_heart;

		
		this->text = text;
	}
	void update(int k)// k-������� ��������

	{
		


		if (k > 0)
			if (k < max)
				bar.setSize(Vector2f((max - k) * 132.5 / max ,10));//���� �� ������������ � ��� ���� ������ ���������, �� ������������� ����� �������� (����� ������) ��� ������� ��������������
	}

	void draw(RenderWindow& window)
	{
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();

		s.setPosition(center.x - size.x / 2, center.y - size.y / 2 );//������� �� ������
		bar.setPosition(center.x - size.x / 2 , center.y - size.y / 2 );

 // �������� ����������
		std::ostringstream playerHearts;
		std::ostringstream playerMoney;
		playerHearts << p_heart;
		//������� � ��� ����� �����, �� ���� ��������� ������
		playerMoney << p_money;
		text.setString( "Herat:" + playerHearts.str() + ' ' + "Money:" + playerMoney.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str() 
		text.setPosition(center.x - size.x /2, center.y - size.y / 2 +20);//������ ������� ������

		

		window.draw(s);//������� ������ ������� ��������
		window.draw(bar);//������ �� ��� ������ �������������, �� ��� �� ��������� �
		window.draw(text);//����� ���� �����
		
	}

};

class MovingPlatform :public Entity {
public:
	float moveTimer = 0.0;
	MovingPlatform(String F, float X, float Y, int W, int H , String name):Entity(F, X, Y, W, H) {
		this->name = name;
		image.loadFromFile(F);
		image.createMaskFromColor(Color(255,255,255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);

		sprite.setTextureRect(IntRect(0, 0, w, h));
		dx = 0.3;//���� ��������.���� ������ ������ ���������

	}
	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++) {//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == 's')//���� ������� ��� ������ �����, ��
				{
					if (Dy > 0) { y = i * 32 - h; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; dx = -0.3; sprite.scale(-1, 1); }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; dx = 0.3; sprite.scale(-1, 1); }// � ����� ����� �����
				}
			}
		}
	}

	void update(float time)
	{

		moveTimer += time;if (moveTimer>2500){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
		checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		x += dx * time;
		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������

	}

};

bool startgame() {

	RenderWindow window/*(VideoMode(640, 480), "SonicGame"); */ (VideoMode(1920, 1080), "SonicGame", Style::Fullscreen);
	view.reset(FloatRect(0, 0, 640,480));

	SoundBuffer eoy1;
	eoy1.loadFromFile("music/hit.wav");
	Sound eoy;
	eoy.setBuffer(eoy1);

	SoundBuffer oy1;
	oy1.loadFromFile("music/oy.ogg");
	Sound oy;
	oy.setBuffer(oy1);
	//musicNext.setVolume(100);

	Music music;
	music.openFromFile("music/MusicLevel1.ogg");
	music.setVolume(2);
	music.play();



	Image map_img; map_img.loadFromFile("images/map1.png"); map_img.createMaskFromColor(Color(255, 255, 255));
	Texture map; map.loadFromImage(map_img);
	Sprite s_map; s_map.setTexture(map);

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	text.setFillColor(Color(254, 231, 97));//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);



	Player p("images/MilesTailsPrower.gif", 64, 736, 45, 32);
	Image image;





	Image i;
	i.loadFromFile("images/HealthBar.png");


	////////////////////////////////////////////////////������ ���������� �����/////////////////////////////////////////////////////////////////
	image.loadFromFile("images/enemy.gif");
	std::list<Entity*> entities;
	std::list<Entity*>::iterator it;
	std::vector<int> enemyXY = { 550, 420 , 451 ,228 };
	std::vector<int> PlatformXY = { 250,670 };
	int EXy = 0;
	int PXy = 0;
	for (int i = 0; i < map_h; i++) {
		for (int j = 0; j < map_w; j++) {
			if (TileMap[i][j] == 'e') {
				entities.push_back(new Enemy("images/enemy.gif", enemyXY[EXy], enemyXY[EXy + 1], 91, 59, image, "EasyEnemy"));
				EXy += 2;
			}
			////////////////////////////////////////////////////����� ���������� �����/////////////////////////////////////////////////////////////////
			if (TileMap[i][j] == 'p') {
				entities.push_back(new MovingPlatform("images/MovingPlatform1.png", PlatformXY[PXy], PlatformXY[PXy + 1], 94,18, "MovingPlatform"));
				PXy += 2;
			}
		}
	}
	////////////////////////////////////////////////////����� ���������� ���������/////////////////////////////////////////////////////////////////


	float currentFrame = 0;
	Clock clock;
	while (window.isOpen())
	{
		if (p.x == 32 & p.y == 96) { return false; }
		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//��� �������� ����
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//��� ������ �� ����


		LB H(i, text, p.heart , p.money);
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
		if (p.life) {
			/*if(p.x > 100 & p.x < 1100)*/viewXY(p.x, p.y);
		}
		if (p.life) {
			p.update(time);
		}



		/////////////////////////������ �������� ���� ����� �������� �� ����� ������ �� ���� �������//////////////////////
		for (it = entities.begin(); it != entities.end();) {
			Entity* b = *it;//��� ������ ��������� ����� �� ������ (*it) , � ������ ������ b , �� ��� ���� ���� delete(b); ,a  delete(*it) ������; 
			(b)->update(time);
			if ((*it)->life == false) { it = entities.erase(it); delete(b); }
			else it++;

		}
		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->getRect().intersects(p.getRect())) {
				if ((*it)->name == "EasyEnemy") {
					if ((p.dy > 0) & (p.onGround == false)) {
						eoy.play();
						(*it)->dx = 0; p.dy = -0.2; (*it)->health = 0;
					}
					else if (p.x > (*it)->x) { p.x += 10; p.health -= 20;oy.play();}
					else if (p.x < (*it)->x) { p.x -= 10; p.health -= 20; oy.play();}
				}
				else if ((*it)->name == "MovingPlatform") {
					Entity* movPlat = *it;
					if ((p.dy > 0) || (p.onGround == false))//��� ���� ����� ��������� � ��������� ����� ������, �.� ������ ����
						if (p.y + p.h < movPlat->y + movPlat->h)//���� ����� ��������� ���� ���������, �.� ��� ��� ���� ������� (�� �� ��� ��������� ��� �� ���������� � ����������)
						{
							p.y = movPlat->y - p.h + 3; p.x += movPlat->dx * time; p.dy = 0; p.onGround = true; // �� ����������� ������ ���, ����� �� ��� �� ����� �� ���������
						}
				}
			}
		}
		/////////////////////////����� �������� ���� ����� �������� �� ����� ������ �� ���� �������//////////////////////



		window.setView(view);
		window.clear(Color(216,118,68));



		for (int i = 0; i < map_h; i++)
			for (int j = 0; j < map_w; j++)
			{

				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 190, 64, 64));
				if (TileMap[i][j] == 'd')  s_map.setTextureRect(IntRect(260,230, 100,100));
				if (TileMap[i][j] == 'm')  s_map.setTextureRect(IntRect(140, 330, 64, 64));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(190, 190, 64, 64));
				if (TileMap[i][j] == '0')  s_map.setTextureRect(IntRect(62, 190, 64, 64));
				if (TileMap[i][j] == 'j')  s_map.setTextureRect(IntRect(255, 275, 64, 32));
				if (TileMap[i][j] == 'h')s_map.setTextureRect(IntRect(334, 395, 120, 120));
				s_map.setPosition(j * 32, i * 32);


				window.draw(s_map);

			}

		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}

		if (p.life == false) {

			p.dy = 0.4;
			p.onGround = false;
			p.sprite.setTextureRect(IntRect(516,257,41,25));
			//music.stop();
			}
		window.draw(p.sprite);




		H.update(p.health);
		H.draw(window);

		window.display();


	}
}

void GameRaning() {
	if (startgame()) { GameRaning(); }
	
}


void menu(RenderWindow& window) {


	Texture menuTexture1, menuTexture2, menuBackGround;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/222.png");
	menuBackGround.loadFromFile("images/menuBg.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackGround);

	Sound go;
	SoundBuffer goBuffer;
	goBuffer.loadFromFile("music/go.wav");
	go.setBuffer(goBuffer);
	go.setVolume(3);

	Music mainMusic;
	mainMusic.openFromFile("music/mainMusic.ogg");
	mainMusic.setVolume(3);
	mainMusic.play();

	bool isMenu = 1;
	int menunum = 0;


	menu1.setPosition(100, 60);
	menu2.setPosition(1760,60);
	menuBg.setPosition(0, 0);

	while (isMenu) {

		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menunum = 0;
		window.clear(Color(0, 0, 147));

		if (IntRect(100, 60, 250, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menunum = 1; }
		if (IntRect(1760, 60, 250, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menunum = 2; }
	
		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (menunum == 1) { 
			mainMusic.stop(); 
			go.play();
			 GameRaning();
			 mainMusic.play();
			}
			if (menunum == 2) { window.close(); isMenu = false;}

		}
		
		
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);

		window.display();

	}
}



int main()
{
	RenderWindow window/*(VideoMode(640, 480), "SonicGame");*/ (sf::VideoMode(1376, 768), "Menu", Style::Fullscreen);
	menu(window);
	return 0;
}