#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "functions.hpp" 

int main(void)
{
    GLFWwindow* window;
    int initSuccess = 1;

    if (InitializeOpenGL(window) != initSuccess)
    {
        return -1;
    }

    unsigned int VAO;
    
    int amountOfTrianglesInCircle = 100;
    extern int numOfDimensionsInVertex;
    extern int amountOfCircleCenterVertices;

    std::string vertShaderStr = LoadShader("../shaders/3.3.vert.shad");
    std::string fragShaderStr = LoadShader("../shaders/3.3.frag.shad");

    const char* vertShader = vertShaderStr.c_str();
    const char* fragShader = fragShaderStr.c_str();


    unsigned int triangleProgram = CreateCircleProgram(VAO, amountOfTrianglesInCircle, vertShader, fragShader);

    // -- uncomment to see the outline only --
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        //processInput(window);
        /* Render here */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(triangleProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (amountOfTrianglesInCircle + amountOfCircleCenterVertices) * numOfDimensionsInVertex, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}