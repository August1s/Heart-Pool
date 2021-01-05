#include "Curve.h"

#include <complex>


Curve::Curve(float a, std::string& s)
	:A(a), Style(s)
{
	if (Style == "circle")
	{
		/* ʹ��Բ�� R = a */
		Xoffset = 600, Yoffset = 450;	
		for (float t = 0.0f; t <= 2 * M_PI; t += 0.01)
		{
			float x = A * cos(t) + Xoffset;
			float y = A * sin(t) + Yoffset;
			points.push_back(Eigen::Vector2f(x, y));
		}
	}
	else if (Style == "heart")
	{
		/* ʹ�������� R = a(1 - sin��)*/
		Xoffset = 600, Yoffset = 750;
		for (float t = 0.0f; t < 2 * M_PI; t += 0.01)
		{
			float x = A * (1 - sin(t)) * cos(t) + Xoffset;
			float y = A * (1 - sin(t)) * sin(t) + Yoffset;
			points.push_back(Eigen::Vector2f(x, y));
		}
	}
	else
	{
		std::cout << "Wrong Style" << std::endl;
	}
}

float Curve::GetR(float x, float y) const
{
	return sqrt(pow((x - Xoffset), 2) + pow((y - Yoffset), 2));
}

float Curve::GetTheta(float x, float y) const
{
	std::complex<float> c((x - Xoffset), (y - Yoffset));
	float t = std::arg(c);
	return t >= 0 ? t : t + 2 * M_PI;	// �������ǵķ�Χ
}

float Curve::GetCurveR(float x, float y) const
{
	if (Style == "circle")
		return A;
	else if (Style == "heart")
		return A * (1 - sin(GetTheta(x, y)));
	else
		return 0;
}

Eigen::Vector2d Curve::GetXY(float t) const
{
	float x = A * (1 - sin(t)) * cos(t) + Xoffset;
	float y = A * (1 - sin(t)) * sin(t) + Yoffset;
	return Eigen::Vector2d(x, y);
}
