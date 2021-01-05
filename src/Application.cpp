#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "ParticleSystem.h"
#include "Curve.h"

const static int WINDOW_WIDTH = 800;
const static int WINDOW_HEIGHT = 600;

//std::string curve_type("circle");
std::string curve_type("heart");
Curve c(320.0f, curve_type);
ParticleSystem particles(15.0f, 1000, WINDOW_WIDTH, WINDOW_HEIGHT, c);

void InitGL(void)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glMatrixMode(GL_PROJECTION);
}

void Render(ParticleSystem& particles, Curve& curve)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glOrtho(0, WINDOW_WIDTH * 1.5, 0, WINDOW_HEIGHT * 1.5, 0, 1);

	glColor4f(1.0f, 0.416f, 0.416f, 1);

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for (auto& l : curve.GetCurve())
		glVertex2f(l(0), l(1));
	glEnd();

	glPointSize(8);
	glBegin(GL_POINTS);
	for (auto& p : particles.GetParticles())
		glVertex2f(p.x(0), p.x(1));
	glEnd();
}

void key_callback(GLFWwindow* window, int key, int scannode, int action, int mode)
{
	//std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		particles.Restart();
}

int main(int argc, char** argv)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPH", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glfwSetKeyCallback(window, key_callback);

	InitGL();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		particles.Update();
		Render(particles, c);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}