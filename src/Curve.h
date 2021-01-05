#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Core>

#define _USE_MATH_DEFINES
#include <math.h>


class Curve
{
private:
	std::vector<Eigen::Vector2f> points;		// 用于绘制曲线

public:
	float A;									// 极坐标方程参数
	std::string& Style;							// 曲线种类
	float Xoffset, Yoffset;						// 将曲线平移到视口内
	

	Curve(float a, std::string& s);

	void UpdatePoints();

	float GetR(float x, float y) const;			// 根据二维直角坐标（减去平移量后的直角坐标）返回该点的极径

	float GetTheta(float x, float y) const;		// 根据二维直角坐标计算θ   -> [0, 2PI]

	float GetCurveR(float x, float y) const;	// 根据θ返回曲线的极径

	Eigen::Vector2d GetXY(float t) const;		// 返回θ对应曲线上的点

	inline const std::vector<Eigen::Vector2f>&GetCurve() const { return points; }

};
