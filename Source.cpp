#if defined _WIN32 || defined _WIN64

#include<math.h>
#include<SFML/Graphics.hpp>
#include<vector>
#include<ctime>
#include<iostream>
#include<string>
#include <Windows.h>

#endif


using namespace sf;
using namespace std;

const int scrX = GetSystemMetrics(SM_CXSCREEN);
const int scrY = GetSystemMetrics(SM_CYSCREEN);
const double pi = 3.14, g = 9.8;																							//постоянные
double const h = 1, t = 0.1;					   				  															//порядок точности и временной шаг
const double m = 1.5, l = 100;																								//определяющие компоненты маятника
double p1 = 0, p2 = 0, alfa = pi / 2, beta = alfa, * A;																		//компоненты состояния
const double x = scrX / 2, y = scrY / 3, l_g = 250, kr = 20;																//графика
const int i1 = 50;																											// длина хвоста
double mu = 1;

double f_alfa(double alfa, double beta, double p1, double p2)
{
	return (p1 - p2 * (cos(alfa - beta))) / (m * l * l * (1 + mu * sin(alfa - beta) * sin(alfa - beta)));
}
double f_betta(double alfa, double beta, double p1, double p2)
{
	return (p2 * (1 + mu) - p1 * mu * cos(alfa - beta)) / (m * l * l * (1 + mu * sin(alfa - beta) * sin(alfa - beta)));
}
double f_p1(double alfa, double beta, double p1, double p2)
{
	return -m * (1 + mu) * g * l * sin(alfa) - (p1 * p2 * sin(alfa - beta)) / (l * l * m * (1 + mu * sin(alfa - beta) * sin(alfa - beta))) + (sin(2 * (alfa - beta)) * (p1 * p1 * mu - 2 * p1 * p2 * mu * cos(alfa - beta) + p2 * p2 * (1 + mu))) / (2 * l * l * m * (1 + mu * sin(alfa - beta) * sin(alfa - beta)) * (1 + mu * sin(alfa - beta) * sin(alfa - beta)));
}
double f_p2(double alfa, double beta, double p1, double p2)
{
	return -m * mu * g * l * sin(beta) + (p1 * p2 * sin(alfa - beta)) / (l * l * m * (1 + mu * sin(alfa - beta) * sin(alfa - beta))) - (sin(2 * (alfa - beta)) * (p1 * p1 * mu - 2 * p1 * p2 * mu * cos(alfa - beta) + p2 * p2 * (1 + mu))) / (2 * l * l * m * (1 + mu * sin(alfa - beta) * sin(alfa - beta)) * (1 + mu * sin(alfa - beta) * sin(alfa - beta)));
}


double* Y(double alfa, double beta, double p1, double p2)
{
	double k1[4], k2[4], k3[4], k4[4];
	double y[4];

	k1[0] = t * f_alfa(alfa, beta, p1, p2);
	k1[1] = t * f_betta(alfa, beta, p1, p2);
	k1[2] = t * f_p1(alfa, beta, p1, p2);
	k1[3] = t * f_p2(alfa, beta, p1, p2);

	k2[0] = t * f_alfa(alfa + h / 2 * k1[0], beta + h / 2 * k1[1], p1 + h / 2 * k1[2], p2 + h / 2 * k1[3]);
	k2[1] = t * f_betta(alfa + h / 2 * k1[0], beta + h / 2 * k1[1], p1 + h / 2 * k1[2], p2 + h / 2 * k1[3]);
	k2[2] = t * f_p1(alfa + h / 2 * k1[0], beta + h / 2 * k1[1], p1 + h / 2 * k1[2], p2 + h / 2 * k1[3]);
	k2[3] = t * f_p2(alfa + h / 2 * k1[0], beta + h / 2 * k1[1], p1 + h / 2 * k1[2], p2 + h / 2 * k1[3]);

	k3[0] = t * f_alfa(alfa + h / 2 * k2[0], beta + h / 2 * k2[1], p1 + h / 2 * k2[2], p2 + h / 2 * k2[3]);
	k3[1] = t * f_betta(alfa + h / 2 * k2[0], beta + h / 2 * k2[1], p1 + h / 2 * k2[2], p2 + h / 2 * k2[3]);
	k3[2] = t * f_p1(alfa + h / 2 * k2[0], beta + h / 2 * k2[1], p1 + h / 2 * k2[2], p2 + h / 2 * k2[3]);
	k3[3] = t * f_p2(alfa + h / 2 * k2[0], beta + h / 2 * k2[1], p1 + h / 2 * k2[2], p2 + h / 2 * k2[3]);

	k4[0] = t * f_alfa(alfa + h * k3[0], beta + h * k3[1], p1 + h * k3[2], p2 + h * k3[3]);
	k4[1] = t * f_betta(alfa + h * k3[0], beta + h * k3[1], p1 + h * k3[2], p2 + h * k3[3]);
	k4[2] = t * f_p1(alfa + h * k3[0], beta + h * k3[1], p1 + h * k3[2], p2 + h * k3[3]);
	k4[3] = t * f_p2(alfa + h * k3[0], beta + h * k3[1], p1 + h * k3[2], p2 + h * k3[3]);

	y[0] = alfa + h / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
	y[1] = beta + h / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
	y[2] = p1 + h / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
	y[3] = p2 + h / 6 * (k1[3] + 2 * k2[3] + 2 * k3[3] + k4[3]);

	return y;
}



int main()

{
	bool flag = 0;
	bool pause = 0, begin = 0;
	int i = 0, q = 0, tail = 2;
	int r1 = kr / mu, r2 = kr;



	RenderWindow window(sf::VideoMode(scrX, scrY), L"Двойной маятник");
	window.setPosition(Vector2i(0, 0));
	window.setFramerateLimit(50);

	CircleShape shar1(r1), shar2(r2), shar3(5);
	shar1.setFillColor(sf::Color::Red);
	shar2.setFillColor(sf::Color::Green);
	shar3.setFillColor(sf::Color::White);
	shar3.setPosition(x - 5, y - 5);
	shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
	shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);



	VertexArray line(Lines, 2);
	line[0].position = Vector2f(x, y);
	line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
	VertexArray line1(Lines, 2);
	line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
	line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
	CircleShape slide[i1];
	for (int i = 0; i < i1; i++)
		slide[i].setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - 2, y + l_g * cos(alfa) + l_g * cos(beta) - 2);
	CircleShape slide1[i1];
	for (int i = 0; i < i1; i++)
		slide1[i].setPosition(x + l_g * sin(alfa) - 2, y + l_g * cos(alfa) - 2);

	VertexArray tail1(Points, 100000);
	for (int j = 0; j < 100000; j++)
		tail1[j].position = Vector2f(x, y);




	while (window.isOpen())

	{

		Font font;
		font.loadFromFile("arial.ttf");

		sf::Text text;
		text.setFont(font);
		text.setString(L"Изменить начальный угол: одновременно зажать (' A ' и/или ' B ') и (' up ' и/или ' down ')");
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		sf::Text text1;
		text1.setFont(font);
		text1.setString(L"Изменить соотношение масс: ' M ' + ' up ' или ' M ' + ' down '");
		text1.setCharacterSize(24);
		text1.setFillColor(sf::Color::White);

		sf::Text text2;
		text2.setFont(font);
		text2.setString(L"Старт/пауза: пробел");
		text2.setCharacterSize(24);
		text2.setFillColor(sf::Color::White);

		sf::Text text3;
		text3.setFont(font);
		text3.setString(L"Вид следа: ' T '");
		text3.setCharacterSize(24);
		text3.setFillColor(sf::Color::White);

		text1.setPosition(0, 24);
		text2.setPosition(0, 48);
		text3.setPosition(0, 72);




		if (begin)
		{
			if (pause)
			{





				A = Y(alfa, beta, p1, p2);
				alfa = A[0];
				beta = A[1];
				p1 = A[2];
				p2 = A[3];

				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
				shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);




				for (int j = 0; j < 99999; j++)
				{
					tail1[j] = tail1[j + 1];
				}
				tail1[100000 - 1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));



				if (tail == 1)
				{
					for (int j = 0; j < i1 - 1; j++)																					//хвост
					{
						slide[j] = slide[j + 1];
						i = i1 - 1;
					}
					slide[i].setFillColor(Color::Blue);
					slide[i].setRadius(2);
					slide[i].setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - 2, y + l_g * cos(alfa) + l_g * cos(beta) - 2);

					for (int j = 0; j < i1 - 1; j++)
					{
						slide1[j] = slide1[j + 1];
						i = i1 - 1;
					}
					slide1[i].setFillColor(Color::Cyan);
					slide1[i].setRadius(2);
					slide1[i].setPosition(x + l_g * sin(alfa) - 2, y + l_g * cos(alfa) - 2);

					for (int j = 0; j < 100000; j++)
						tail1[j].position = Vector2f(x, y);

				}
				else
				{
					if (tail == 0)
					{
						for (int i = 0; i < i1; i++)
							slide[i].setPosition(x, y);
						for (int i = 0; i < i1; i++)
							slide1[i].setPosition(x, y);
						for (int j = 0; j < 100000; j++)
							tail1[j].position = Vector2f(x, y);
					}
					else
					{
						for (int i = 0; i < i1; i++)
							slide[i].setPosition(x, y);
						for (int i = 0; i < i1; i++)
							slide1[i].setPosition(x, y);

						for (int j = 0; j < 99999; j++)
						{
							tail1[j] = tail1[j + 1];
						}
						tail1[100000 - 1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
					}

				}

				line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));

			}
		}
		else
		{
			if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::Up) && (alfa < pi))
			{
				alfa += pi / 180;

				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
				shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);
				line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
			}
			if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::Down) && (alfa > -pi))
			{
				alfa -= pi / 180;

				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
				shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);
				line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
			}
			if (Keyboard::isKeyPressed(Keyboard::B) && Keyboard::isKeyPressed(Keyboard::Up) && (beta < pi))
			{
				beta += pi / 180;

				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
				shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);
				line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
			}
			if (Keyboard::isKeyPressed(Keyboard::B) && Keyboard::isKeyPressed(Keyboard::Down) && (beta > -pi))
			{
				beta -= pi / 180;

				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
				shar2.setPosition(x + l_g * sin(alfa) + l_g * sin(beta) - r2, y + l_g * cos(alfa) + l_g * cos(beta) - r2);
				line[1].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[0].position = Vector2f(x + l_g * sin(alfa), y + l_g * cos(alfa));
				line1[1].position = Vector2f(x + l_g * sin(alfa) + l_g * sin(beta), y + l_g * cos(alfa) + l_g * cos(beta));
			}

			if (Keyboard::isKeyPressed(Keyboard::M) && Keyboard::isKeyPressed(Keyboard::Up) && (mu < 2))
			{
				mu += 0.05;
				r1 = kr / mu;
				shar1.setRadius(r1);
				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
			}
			if (Keyboard::isKeyPressed(Keyboard::M) && Keyboard::isKeyPressed(Keyboard::Down) && (mu > 0.5))
			{
				mu -= 0.05;
				r1 = kr / mu;
				shar1.setRadius(r1);
				shar1.setPosition(x + l_g * sin(alfa) - r1, y + l_g * cos(alfa) - r1);
			}
		}
		window.clear(Color::Transparent);
		sf::Event event;

		while (window.pollEvent(event))

		{
			if (flag == false)
			{
				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					begin = true;
					pause = !pause;
					flag = true;
				}

				if (Keyboard::isKeyPressed(Keyboard::T))
				{
					if (tail == 0)
						tail = 1;
					else
						if (tail == 1)
							tail = 2;
						else
							tail = 0;
					flag = true;
				}
			}



			else if (event.type == Event::KeyReleased) flag = false;

			if (event.type == sf::Event::Closed)

				window.close();

		}
		window.draw(tail1);
		for (int j = 0; j < i1; j++)
			window.draw(slide[j]);
		for (int j = 0; j < i1; j++)
			window.draw(slide1[j]);
		window.draw(line);
		window.draw(line1);
		window.draw(shar3);
		window.draw(shar1);
		window.draw(shar2);
		window.draw(text);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.display();
	}

	return 0;
}
