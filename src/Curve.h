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
	std::vector<Eigen::Vector2f> points;		// ���ڻ�������

public:
	float A;									// �����귽�̲���
	std::string& Style;							// ��������
	float Xoffset, Yoffset;						// ������ƽ�Ƶ��ӿ���
	

	Curve(float a, std::string& s);

	void UpdatePoints();

	float GetR(float x, float y) const;			// ���ݶ�άֱ�����꣨��ȥƽ�������ֱ�����꣩���ظõ�ļ���

	float GetTheta(float x, float y) const;		// ���ݶ�άֱ����������   -> [0, 2PI]

	float GetCurveR(float x, float y) const;	// ���ݦȷ������ߵļ���

	Eigen::Vector2d GetXY(float t) const;		// ���ئȶ�Ӧ�����ϵĵ�

	inline const std::vector<Eigen::Vector2f>&GetCurve() const { return points; }

};
