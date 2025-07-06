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
    int pointsPerTriangle = 3;

    const char* vertShader; 
    LoadShader("shaders/3.3.vert.shad", vertShader);
    
    const char* fragShader; 
    LoadShader("shaders/3.3.frag.shad", fragShader);
    unsigned int triangleProgram = CreateCircleProgram(VAO, amountOfTrianglesInCircle, vertShader, fragShader);

    // -- uncomment to see the outline only --
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        //processInput(window);
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(triangleProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, amountOfTrianglesInCircle * pointsPerTriangle);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}