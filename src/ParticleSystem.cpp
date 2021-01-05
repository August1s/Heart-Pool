#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(float radius, int num, float width, float height, Curve& c)
	:H(radius), HSQ(radius*radius), particles_num(num), 
	POLY6(315.f / (65.f * M_PI * pow(radius, 9.f))), SPIKY_GRAD(-45.f / (M_PI * pow(radius, 6.f))), VISC_LAP(45.f / (M_PI * pow(radius, 6.f))),
	EPS(radius), VIEW_WIDTH(1.5 * width), VIEW_HEIGHT(1.5 * height), 
	curve(c)
{
	int count = 0;
	for (float y = VIEW_HEIGHT / 3; y < VIEW_HEIGHT - EPS * 2.f; y += H)
	{
		for (float x = VIEW_WIDTH / 4; x <=  VIEW_WIDTH / 2; x += H)
		{
			if (particles.size() >= particles_num)
				break;
			float jitter = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			particles.push_back(Particle(x + jitter, y));
			count++;
		}
	}

	std::cout << "initializing with " << count << " particles" << std::endl;
}

void ParticleSystem::Integrate()
{
	for (auto& p : particles)
	{
		// forward Euler integration
		p.v += DT * p.f / p.rho;
		p.x += DT * p.v;

		Collision(p);
	}
}

void ParticleSystem::ComputeDensityPressure()
{
	for (auto& pi : particles)
	{
		pi.rho = 0.f;
		for (auto& pj : particles)
		{
			Eigen::Vector2d rij = pj.x - pi.x;
			float r2 = rij.squaredNorm();

			if (r2 < HSQ)
			{
				// this computation is symmetric
				pi.rho += MASS * POLY6 * pow(HSQ - r2, 3.f);
			}
		}
		pi.p = GAS_CONST * (pi.rho - REST_DENS);
	}
}

void ParticleSystem::ComputeForces()
{
	for (auto& pi : particles)
	{
		Eigen::Vector2d fpress(0.f, 0.f);
		Eigen::Vector2d fvisc(0.f, 0.f);
		for (auto& pj : particles)
		{
			if (&pi == &pj)
				continue;

			Eigen::Vector2d rij = pj.x - pi.x;
			float r = rij.norm();

			if (r < H)
			{
				// compute pressure force contribution
				fpress += -rij.normalized() * MASS * (pi.p + pj.p) / (2.f * pj.rho) * SPIKY_GRAD * pow(H - r, 2.f);
				// compute viscosity force contribution
				fvisc += VISC * MASS * (pj.v - pi.v) / pj.rho * VISC_LAP * (H - r);
			}
		}
		Eigen::Vector2d fgrav = G * pi.rho;
		pi.f = fpress + fvisc + fgrav;
	}
}

void ParticleSystem::Update()
{
	ComputeDensityPressure();
	ComputeForces();
	Integrate();
}

void ParticleSystem::Restart()
{
	particles.clear();
	for (float y = VIEW_HEIGHT / 3; y < VIEW_HEIGHT - EPS * 2.f; y += H)
	{
		for (float x = VIEW_WIDTH / 4; x <= VIEW_WIDTH / 2; x += H)
		{
			if (particles.size() >= particles_num)
				break;
			float jitter = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			particles.push_back(Particle(x + jitter, y));
		}
	}
}

void ParticleSystem::Collision(Particle& p)
{
	
	/* 视口矩形作为边界 */
	//if (p.x(0) - EPS < 0.0f)
	//{
	//	p.v(0) *= -BOUND_DAMPING;
	//	p.x(0) = EPS;
	//}
	//if (p.x(0) + EPS > VIEW_WIDTH)
	//{
	//	p.v(0) *= -BOUND_DAMPING;
	//	p.x(0) = VIEW_WIDTH - EPS;
	//}
	//if (p.x(1) - EPS < 0.0f)
	//{
	//	p.v(1) *= -BOUND_DAMPING;
	//	p.x(1) = EPS;
	//}
	//if (p.x(1) + EPS > VIEW_HEIGHT)
	//{
	//	p.v(1) *= -BOUND_DAMPING;
	//	p.x(1) = VIEW_HEIGHT - EPS;
	//}

	if (curve.Style == "circle")
	{
		/* 使用圆形作为边界*/
		float R = curve.GetR(p.x(0), p.x(1));
		float CurveR = curve.GetCurveR(p.x(0), p.x(1));

		if (R + EPS / 2 > CurveR)
		{
			Eigen::Vector2d n = (Eigen::Vector2d(curve.Xoffset, curve.Yoffset) - p.x).normalized();	// 归一化法向量，指向圆心
			Eigen::Vector2d Vn = -(p.v.dot(n)) * n;													// 速度的法向分量
			Eigen::Vector2d Vt = p.v + Vn;															// 速度的切向分量

			p.v = Vt + Vn;																			// 边界碰撞后的速度
			//p.v(0) *= BOUND_DAMPING;
			//p.v(1) *= BOUND_DAMPING;
			//p.x = Eigen::Vector2d(curve.Xoffset, curve.Yoffset) - (CurveR - EPS / 2) * n;
			p.x += n * EPS / 2;
		}
	}
	else if (curve.Style == "heart")
	{
		/* 使用心形线作为边界 */
		float R = curve.GetR(p.x(0), p.x(1));
		float CurveR = curve.GetCurveR(p.x(0), p.x(1));

		if (R + EPS / 2 > CurveR)
		{
			float delta = 0.00001;
			float theta = curve.GetTheta(p.x(0), p.x(1));
			Eigen::Vector2d t = curve.GetXY(theta + delta) - curve.GetXY(theta - delta);// 近似的切向向量
			Eigen::Vector2d n = Eigen::Vector2d(-t(1), t(0));
			n.normalize();																// 近似的单位法向量

			Eigen::Vector2d Vn = -(p.v.dot(n)) * n;
			Eigen::Vector2d Vt = p.v + Vn;
			p.v = Vt + Vn;
			//p.v(0) *= BOUND_DAMPING;
			//p.v(1) *= BOUND_DAMPING;
			//p.x = curve.GetXY(theta) + n * EPS / 2 ;
			p.x += n * EPS / 2;
		}
	}

	


	


	
}
