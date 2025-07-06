#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>

int InitializeOpenGL(GLFWwindow*& window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int CreateCircleProgram(unsigned int &VAO, int amountOfTriangles, const char* vertexShaderSource, const char* fragmentShaderSource);
void LoadShader(const char* shaderPath, const char* shaderOutput);
void CheckShaderCompilation(unsigned int vertexShader);
void CheckShaderLink(unsigned int shaderProgram);
void GetCircle2DVertex(float* startingPoint, float angle, float radius, float* outputVertex);
void CreateTrianglesInsideCircle(float* vertices, float* startingPoint, int amountOfTriangles);