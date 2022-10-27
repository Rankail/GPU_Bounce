#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <queue>
#include <list>

int POINT_COUNT_ROOT;
int COMPUTE_SIZE;
int COUNT;

constexpr unsigned int COUNT_SQUARE_ROOT = 3163;
constexpr unsigned int WORKGROUP_SIZE = 32;

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 800;

GLuint vbo;

void resetData()
{
	std::vector<GLfloat> data(COUNT * 4);
	for (int i = 0; i < COUNT * 4; i += 4)
	{
		float angle = (float)i / (float)(COUNT * 4.f) * 2.f * 3.1415926535f;
		data[i + 0] = 0.f;
		data[i + 1] = 0.f;
		data[i + 2] = angle;
		data[i + 3] = 0.f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
}

bool paused = true;
bool prevTime = 0;
float speed = .2f;
bool reverse = false;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_SPACE)
	{
		paused = !paused;
		prevTime = glfwGetTime();
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_RIGHT)
	{
		speed *= 2.f;
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_LEFT)
	{
		speed *= 0.5f;
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_ENTER)
	{
		reverse = !reverse;
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_E)
	{
		resetData();
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, 1);
	}
}

int main(int argc, char **argv)
{
	std::cout << "Wurzel von Anzahl Punkte:" << std::endl;
	std::cin >> POINT_COUNT_ROOT;

	std::cout << "Punkt Groesse:" << std::endl;
	int POINT_SIZE;
	std::cin >> POINT_SIZE;

	if (POINT_COUNT_ROOT < 1 || POINT_SIZE < 1)
	{
		std::cout << "nein" << std::endl;
		return -1;
	}

	COMPUTE_SIZE = (int)ceil(COUNT_SQUARE_ROOT / (float)WORKGROUP_SIZE);
	COUNT = POINT_COUNT_ROOT * POINT_COUNT_ROOT;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Compute Shader", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, WIDTH, HEIGHT);

	float vertices[] = {
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3};

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);

	GLuint ibo;
	glCreateBuffers(1, &ibo);

	glBindVertexArray(vao);

	// initializes data that will be send to gpu
	std::vector<GLfloat> data(COUNT * 4);
	for (int i = 0; i < COUNT * 4; i += 4)
	{
		float angle = (float)i / (float)(COUNT * 4.f) * 2.f * 3.1415926535f;
		data[i + 0] = 0.f;
		data[i + 1] = 0.f;
		data[i + 2] = angle;
		data[i + 3] = 0.f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	Shader computeShader;
	computeShader.AddSourceAndCompile("./rsc/shaders/shader.comp", GL_COMPUTE_SHADER);
	computeShader.Link();

	Shader graphicShader;
	graphicShader.AddSourceAndCompile("./rsc/shaders/shader.vert", GL_VERTEX_SHADER);
	graphicShader.AddSourceAndCompile("./rsc/shaders/shader.frag", GL_FRAGMENT_SHADER);
	graphicShader.Link();

	double prevTime = glfwGetTime();

	glPointSize((float)POINT_SIZE);

	std::list<float> fpss;

	float count = 0;

	while (!glfwWindowShouldClose(window))
	{
		double curTime = glfwGetTime();
		double dt = curTime - prevTime;
		prevTime = curTime;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		if (!paused)
		{
			computeShader.Use();
			// vertex buffer is used as shader storage buffer
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo);
			computeShader.SetFloat("deltaTime", (float)dt * speed * (1 - reverse * 2));
			glDispatchCompute(COMPUTE_SIZE, COMPUTE_SIZE, 1);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
		}

		// computed data is directly used as vertex buffer
		graphicShader.Use();
		glDrawArrays(GL_POINTS, 0, (GLsizei)(data.size() / 4));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}