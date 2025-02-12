#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	VideoMode vm(1920,1080);

	RenderWindow window(vm, "Timber!!!", Style::Default);

	Texture textureBackground;

	textureBackground.loadFromFile("graphics/forest2.png");

	Sprite spriteBackground;

	spriteBackground.setTexture(textureBackground);

	spriteBackground.setPosition(0, 0);

	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;

	float beeSpeed = 0.0f;

	Texture textureBird;
	textureBird.loadFromFile("graphics/bird.png");
	Sprite spriteBird;
	spriteBird.setTexture(textureBird);
	spriteBird.setPosition(0, 800);

	bool birdActive = false;

	float birdSpeed = 0.0f;

	Texture textureCloud;
	Texture textureBalloon;

	textureBalloon.loadFromFile("graphics/balloon.png");
	textureCloud.loadFromFile("graphics/cloud.png");

	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	Sprite spriteBalloon;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteBalloon.setTexture(textureBalloon);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150);
	spriteCloud3.setPosition(0, 300);
	spriteBalloon.setPosition(0, 150);

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	bool balloonActive = false;

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	float balloonSpeed = 0.0f;

	Clock clock;
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	bool paused = true;
	int score = 0;

	sf::Text messageText;
	sf::Text scoreText;

	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	int abilityUses = 3;
	bool slowTimeActive = false;
	sf::Clock abilityClock;
	float slowTimeDuration = 5.0f;
	float normalTimeBarSpeed = timeBarWidthPerSecond;
	float slowedTimeBarSpeed = timeBarWidthPerSecond / 3;
	bool timePaused = false;

	sf::Text abilityText;
	abilityText.setFont(font);
	abilityText.setCharacterSize(50);
	abilityText.setFillColor(sf::Color::White);
	abilityText.setPosition(1400, 20);

	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch2.png");

	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		branches[i].setOrigin(220, 20);
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player2.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip2.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{


			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				//spriteAxe.setPosition(2000,
					//spriteAxe.getPosition().y);
			}

			if (slowTimeActive) {
				if (abilityClock.getElapsedTime().asSeconds() >= slowTimeDuration) {
					slowTimeActive = false;
					timeBarWidthPerSecond = normalTimeBarSpeed;
				}
			}

			std::stringstream abilitySS;
			abilitySS << "Power-Ups Left: " << abilityUses;
			abilityText.setString(abilitySS.str());

		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && abilityUses > 0 && !timePaused) {
			timePaused = true;
			abilityClock.restart();
			abilityUses--;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (event.type == Event::Closed)
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			score = 0;
			timeRemaining = 6;
			abilityUses = 3;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			spriteRIP.setPosition(675, 2000);

			spritePlayer.setPosition(580, 720);

			acceptInput = true;


		}

		if (acceptInput)
		{

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				spritePlayer.setScale(-1, 1);
				spritePlayer.setOrigin(spritePlayer.getLocalBounds().width, 0);
				score++;

				timeRemaining += (2 / score) + .15;

				//spriteAxe.setPosition(AXE_POSITION_RIGHT,
					//spriteAxe.getPosition().y);



				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;


				acceptInput = false;

				chop.play();

			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				spritePlayer.setScale(1, 1);
				spritePlayer.setOrigin(0, 0);
				score++;

				timeRemaining += (2 / score) + .15;

				//spriteAxe.setPosition(AXE_POSITION_LEFT,
					//spriteAxe.getPosition().y);


				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;


				acceptInput = false;

				chop.play();

			}


		}

		if (!paused)
		{
			Time dt = clock.restart();

			if (!timePaused) {
				timeRemaining -= dt.asSeconds();
			}

			if (timePaused && abilityClock.getElapsedTime().asSeconds() >= 5.0f) {
				timePaused = false;
			}
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));


			if (timeRemaining <= 0.0f) {

				paused = true;

				messageText.setString("Out of time!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();


			}

			int birdDirection = 1;
			float minY = 300;
			float maxY = 600;

			if (!birdActive) {
				birdSpeed = (rand() % 200) + 200;

				if (rand() % 2 == 0) {
					spriteBird.setPosition(-100, (rand() % (int)(maxY - minY)) + minY); // Left side
					birdDirection = 1;
				}
				else {
					spriteBird.setPosition(1920 + 100, (rand() % (int)(maxY - minY)) + minY); // Right side
					birdDirection = -1;
				}

				birdActive = true;
			}
			else {
				float time = dt.asSeconds();

				spriteBird.setPosition(
					spriteBird.getPosition().x + (birdDirection * birdSpeed * time),
					spriteBird.getPosition().y + (sin(spriteBird.getPosition().x * 0.02) * 100 * time)
				);

				if (spriteBird.getPosition().x < -100 || spriteBird.getPosition().x > 1920 + 100) {
					birdActive = false;
				}
			}

			int beeDirection = 1;

			if (!beeActive) {
				beeSpeed = (rand() % 200) + 200;

				if (rand() % 2 == 0) {
					spriteBee.setPosition(-100, (rand() % (int)(maxY - minY)) + minY);
					beeDirection = 1;
				}
				else {
					spriteBee.setPosition(1920 + 100, (rand() % (int)(maxY - minY)) + minY);
					beeDirection = -1;
				}

				beeActive = true;
			}
			else {
				float time = dt.asSeconds();

				spriteBee.setPosition(
					spriteBee.getPosition().x + (beeDirection * beeSpeed * time),
					spriteBee.getPosition().y + (sin(spriteBee.getPosition().x * 0.02) * 100 * time)
				);

				if (spriteBee.getPosition().x < -100 || spriteBee.getPosition().x > 1920 + 100) {
					beeActive = false;
				}
			}

			if (!balloonActive) {
				balloonSpeed = (rand() % 200);
				float startX = (rand() % 1920);
				spriteBalloon.setPosition(startX, 100);
				balloonActive = true;
			}
			else {
				spriteBalloon.setPosition(
					spriteBalloon.getPosition().x,
					spriteBalloon.getPosition().y - (balloonSpeed * dt.asSeconds())
				);

				if (spriteBalloon.getPosition().y < -100) {
					balloonActive = false;
				}
			}

			if (!cloud2Active)
			{

				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;


			}
			else
			{

				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{

				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);

				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}


			if (!cloud1Active)
			{

				srand((int)time(0) * 30);
				cloud1Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);

				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; i++)
			{

				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);

				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);

				spritePlayer.setPosition(2000, 660);

				messageText.setString("SQUISHED!!");

				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				death.play();


			}


		}

		window.clear();

		window.draw(spriteBackground);

		window.draw(abilityText);

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteBalloon);

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);

		//window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(spriteBird);

		window.draw(scoreText);

		window.draw(timeBar);


		if (paused)
		{
			window.draw(messageText);
		}

		window.display();


	}

	return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}


}