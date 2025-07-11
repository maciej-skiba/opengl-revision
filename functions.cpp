#include "functions.hpp"

int numOfDimensionsInVertex = 3;
int amountOfVerticesInTriangle = 3;
int amountOfCircleCenterVertices = 1;

int InitializeOpenGL(GLFWwindow*& window)
{
        /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "cant load glew" << std::endl;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glClearColor(0.45f, 0.91f, 0.45f, 1);
    }
}

std::string LoadShader(const std::string& shaderPath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try 
    {
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return "";
    }
}

unsigned int CreateCircleProgram(unsigned int &VAO, int amountOfTriangles, const char* vertexShaderSource, const char* fragmentShaderSource)
{   
    int totalAmountOfVertices = (amountOfTriangles + amountOfCircleCenterVertices) * numOfDimensionsInVertex;
    float circleVertices[totalAmountOfVertices];
    float startingPoint[2] = { 0.0f, 0.0f };

    CreateCircleVertices(circleVertices, startingPoint, amountOfTriangles);

    PrintArray(circleVertices, totalAmountOfVertices);

    //Vertex Array Object
    glGenVertexArrays(1, &VAO); // tworzy VAO
    glBindVertexArray(VAO);   //aktywacja tego VAO
    
    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO); //tworzy pusty VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //ustaw VBO jako aktywny array buffer; wszelkie rzeczy na GL_ARRAY_BUFFER będą dotyczyły obiektu VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW); //wypełnij bufor (GPU) danymi

    unsigned int elementIndices[amountOfTriangles * numOfDimensionsInVertex]; 
    CreateCircleTriangles(elementIndices, amountOfTriangles);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices), elementIndices, GL_STATIC_DRAW); 

    glVertexAttribPointer(
        0,                  // index = location w shaderze (0 dla aPos)
        3,                  // ile wartości (vec3 → 3)
        GL_FLOAT,           // typ danych
        GL_FALSE,           // normalizacja
        3 * sizeof(float),  // odstęp między kolejnymi atrybutami (stride)
        (void*)0            // offset od początku
    );
    glEnableVertexAttribArray(0); //wybierz layout 0 jako aktywny

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    CheckShaderCompilation(vertexShader);
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    CheckShaderCompilation(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderLink(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void GetCircle2DVertex(float* startingPoint, float angle, float radius, float* outputVertex)
{
    float angleInRadians = angle * (M_PI / 180.0f);

    float x = radius * cos(angleInRadians);

    if (abs(x) < 0.001f)
    {
        x = 0;
    }
    
    float y = radius * sin(angleInRadians);
    
    if (abs(y) < 0.001f)
    {
        y = 0;
    }

    outputVertex[0] = x + startingPoint[0];
    outputVertex[1] = y + startingPoint[1];
}

void CreateCircleVertices(float* vertices, float* startingPoint, int amountOfTriangles)
{
    float tempVertex[2];
    float deltaAngle = 360.0f / amountOfTriangles;

    // Create center of the circle
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    
    int offset = numOfDimensionsInVertex * amountOfCircleCenterVertices;

    for (int point = 0; point < amountOfTriangles; point++)
    {
        int triangleFirstPoint_X_index = point * numOfDimensionsInVertex + offset;
        int triangleFirstPoint_Y_index = triangleFirstPoint_X_index + 1;
        int triangleFirstPoint_Z_index = triangleFirstPoint_Y_index + 1;

        GetCircle2DVertex(startingPoint, deltaAngle * point, 0.5f, tempVertex);  

        // first vertex of the triangle (equal to second vertex of previous triangle)
        vertices[triangleFirstPoint_X_index] = tempVertex[0];
        vertices[triangleFirstPoint_Y_index] = tempVertex[1];
        vertices[triangleFirstPoint_Z_index] = 0.0f;
    }
}

void CheckShaderCompilation(unsigned int vertexShader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void CheckShaderLink(unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
}

void PrintArray(float* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << "[" << i << "]:" << arr[i] << std::endl;
    }
}

void CreateCircleTriangles(unsigned int* elementIndices, int amountOfTriangles)
{
    for (int triangleIndex = 0; triangleIndex < amountOfTriangles; triangleIndex++)
    {
        elementIndices[triangleIndex * amountOfVerticesInTriangle] = 0;
        elementIndices[triangleIndex * amountOfVerticesInTriangle + 1] = triangleIndex + 1;

        if (triangleIndex == amountOfTriangles - 1)
        {
            // for the last triangle, choose point number 1 to create a full circle
            elementIndices[triangleIndex * amountOfVerticesInTriangle + 2] = 1;
        }
        else
        {
            elementIndices[triangleIndex * amountOfVerticesInTriangle + 2] = triangleIndex + 2;
        }
    }
}