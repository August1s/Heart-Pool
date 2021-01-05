#pragma once

#include <Eigen/Core>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Curve.h"



// particle data structure
// stores position, velocity, and force for integration
// stores density (rho) and pressure values for SPH
struct Particle
{
	Particle(float _x, float _y) : x(_x, _y), v(0.f, 0.f), f(0.f, 0.f), rho(0), p(0.f) {}
	Eigen::Vector2d x, v, f;
	float rho, p;
};


class ParticleSystem
{
private:
	
	float H;				// kernel radius
	float HSQ;				// radius^2 for optimization	 
	int particles_num;		// max num

	std::vector<Particle> particles;

	float REST_DENS = 1000.f;		// rest density
	float GAS_CONST = 2000.f;		// const for equation of state
	float MASS = 65.f;				// assume all particles have the same mass
	float VISC = 250.f;				// viscosity constant
	float DT = 0.0008f;				// integration timestep
	Eigen::Vector2d G = Eigen::Vector2d(0.f, 10000 * -9.8f) ;		//	重力
	
	// smoothing kernels defined in Müller and their gradients
	float POLY6;
	float SPIKY_GRAD;
	float VISC_LAP;

	// simulation parameters
	float EPS; // boundary epsilon
	float BOUND_DAMPING = 0.7f;

	float VIEW_WIDTH;
	float VIEW_HEIGHT;

	Curve curve;

public:
	ParticleSystem(float radius, int num, float view_width, float view_height, Curve& c);

	void Integrate();

	void ComputeDensityPressure();

	void ComputeForces();

	void Update();

	void Restart();

	void Collision(Particle& p);	// 碰撞后粒子的速度变化

	inline const std::vector<Particle>& GetParticles() const { return particles; }
};
