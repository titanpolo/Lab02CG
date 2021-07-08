//
//  main.cpp
//  TemplateJanela
//
//  Created by Mauren Louise Sguario on 18/11/20.
// 1 - Mover as inicializações initOpenGL(), limpar main()
// 2 - gWindow -> variavel global

#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//PI
#define PI 3.14159265358979323846

//Time
#include <time.h>

using namespace glm;

//Global Variables
const int gWindowWidth = 720;
const int gWindowHeight = 720;
const char* TITLE = { "Lab02" };
GLFWwindow* gWindow = NULL;

//Vertex Structure
struct Vertex
{
    vec3 position;
    vec3 color;
};
Vertex sincFunction[81];
Vertex circle[50];
Vertex pointsCircle[50];

//Vertex Shader
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"uniform mat4 projection;"
"out vec3 vertColor;"
"void main()"
"{"
"	vertColor = color;"
//"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"   gl_Position = projection * vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

//Fragment Shader
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"in vec3 vertColor;"
"void main()"
"{"
"    frag_color = vec4(vertColor, 1.0f);"
"}";

bool initOpenGL();

//------------------------------ Exercises Determination ----------------------------------------------
void triangulo();
void pontos();
void casa();
void sinc();
void circulo();
void circuloRNG();
void elipseRNG();
void elipseRNGTriangulo();

//------------------------------ Shader Variables ----------------------------------------------
GLuint vbo, vao, vs, fs, ibo, vboC, vaoC;
GLint shaderProgram, modelLoc2;

//------------------------------ Control Variables ----------------------------------------------
int count = 1;
void glfw_key(GLFWwindow* window, int key, int scancode, int action, int mode);

//------------------------------ MAIN ----------------------------------------------

int main(void)
{
    if (!initOpenGL())
    {
        std::cerr << "Initialization failed" << std::endl;
        return -1;
    }  
    triangulo();
    //Entre 0 e 1 - 0 -> preto, 1 - Branco (0 a 255, 0 -> preto, 255 - Branco);
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(gWindow))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Render the image
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
        modelLoc2 = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(projection));

        switch (count) {
            case 1:
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
            case 2:
                glPointSize(10.0);
                glDrawArrays(GL_POINTS, 0, 5);
                break;
            case 3:
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                break;
            case 4:
                glDrawArrays(GL_LINE_STRIP, 0, 81);
                break;
            case 5:
                glDrawArrays(GL_TRIANGLE_FAN, 1, 40);
                break;
            case 6:
                glDrawArrays(GL_TRIANGLE_FAN, 1, 40);
                glBindVertexArray(0);
                glPointSize(5.0);
                glBindVertexArray(vaoC);
                glDrawArrays(GL_POINTS, 0, 12);
                break;
            case 7:
                glDrawArrays(GL_TRIANGLE_FAN, 1, 40);
                glBindVertexArray(0);
                glBindVertexArray(vaoC);
                glDrawArrays(GL_LINES, 0, 12);
                break;
            case 8:
                glDrawArrays(GL_TRIANGLE_FAN, 1, 40);
                glBindVertexArray(0);
                glBindVertexArray(vaoC);
                glDrawArrays(GL_TRIANGLES, 0, 12);
                break;
        }
        /* Swap front and back buffers */
        glBindVertexArray(0);
        glfwSwapBuffers(gWindow);
    }
    //Clean UP
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}

//------------------------------ Init OpenGL ----------------------------------------------

bool initOpenGL()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //only to support MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(gWindow);


    //Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    glfwSetKeyCallback(gWindow, glfw_key);
    return true;
}


//------------------------------ Exercício 1 ----------------------------------------------
void triangulo() {
    //Array of vertices for a triangle
    GLfloat vertices[]{
        -0.8f,  -0.9f, 0.0f,    0.0f, 1.0f, 0.0f, //Top
        -0.7f, -0.7f, 0.0f,     0.0f, 1.0f, 0.0f, //Right
        -0.9f, -0.7f, 0.0f,     0.0f, 1.0f, 0.0f, //Left

        -0.8f, -0.2f, 0.0f,     1.0f, 0.0f, 0.0f, //Top
        -0.9f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f, //Right
        -0.7f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f //Left
    };

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

}
//------------------------------ Exercício 2 ----------------------------------------------
void pontos() {
    GLfloat vertices[]{
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   //Ponto 0
      2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   //Ponto 1
      2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f,   //Ponto 2
      0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f,   //Ponto 3
      1.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f    //Ponto 4
    };

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


//------------------------------ Exercício 3 ----------------------------------------------
void casa() {
    GLfloat vertices[]{
     //Square
     -8.0f,  0.0f, 0.0f, 0.5f, 0.3f, 1.0f, //Top left
      8.0f,  0.0f, 0.0f, 0.5f, 0.3f, 1.0f, //Top Right
      8.0f, -8.0f, 0.0f, 0.5f, 0.3f, 1.0f, //Bottom Right
     -8.0f, -8.0f, 0.0f, 0.5f, 0.3f, 1.0f, //Bottom Left

     //Triangle
      0.0f, 6.0f, 0.0f, 1.0f, 0.5f, 0.0f,  //Top
      8.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f,  //Right
     -8.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f,  //Left

     //Rectangle Door
     -1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top left
      1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top Right
      1.0f, -8.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Right
     -1.0f, -8.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Left

     //Left Square Window
     -6.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top left
     -3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top Right
     -3.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Right
     -6.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Left

     //Right Square Window
      3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top left
      6.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Top Right
      6.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Right
      3.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Left
    };

    GLuint indices[] = {
      0, 1, 2, 0, 2, 3,		  //Base Square
      4, 5, 6,				  //Roof Triangle
      7, 8, 9, 7, 9, 10,	  //Door Square
      11, 12, 13, 11, 13, 14, //Window Square
      15, 16, 17, 15, 17, 18, //Window Square
    };

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


//------------------------------ Exercício 4 ----------------------------------------------
void sinc() {
    int k = 0;
    for (float x = -10.0f; x <= 10.0f; x += 0.25f)
    {
        if (x == 0.00) {
            sincFunction[k].position = { 0 , 1.0f, 0.0f };
            sincFunction[k].color = { 0.0f, 0.0f, 0.0f };
        }
        else {
            sincFunction[k].position = { x , sin(x) / x, 0.0f };
            sincFunction[k].color = { 0.0f, 0.0f, 0.0f };
        }
        k++;
    }

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sincFunction), sincFunction, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


//------------------------------ Exercício 5 ----------------------------------------------
void circulo() {
    vec3 pontoCentral = { 0.0, 0.0, 0.0 };
    float raio = 3.0;
    int k = 0;
    for (float i = 0.0; i <= 30.0; i++)
    {
        double theta = 2.0*PI * i / 29.0;
        float x = (raio * cos(theta)) + pontoCentral[0];
        float y = (raio * sin(theta)) + pontoCentral[1];

        circle[k].position = { x, y, 1.0 };
        circle[k].color = { 0.4, 0.5, 0.0 };
        k++;
    }

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


//------------------------------ Exercício 6 ----------------------------------------------
void circuloRNG() {
    Vertex pontosCirculo[12];
    float raio = 3.0;
    int k = 0;
    for (float i = 0.0; i <= 11.0; i++)
    {
        float rho = ((float)rand() / (float)(RAND_MAX));
        float phi = ((float)rand() / (float)(RAND_MAX) * (2 * PI));
        float x = (sqrt(rho) * cos(phi));
        float y = (sqrt(rho) * sin(phi));
        x = x * raio;
        y = y * raio;

        pontosCirculo[k].position = { x, y, 1.0 };
        pontosCirculo[k].color = { rand(), rand(), rand() };
        k++;
    }

    //Buffers on the GPU
    glGenBuffers(1, &vboC);
    glBindBuffer(GL_ARRAY_BUFFER, vboC);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pontosCirculo), pontosCirculo, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vaoC);
    glBindVertexArray(vaoC);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


void elipse() {
    vec3 pontoCentral = { 0.0, 0.0, 0.0 };
    float raio = 3.0;
    int k = 0;
    for (float i = 0.0; i <= 30.0; i++)
    {
        double theta = 2.0 * PI * i / 29.0;
        float x = (raio * cos(theta)) + pontoCentral[0];
        float y = (raio * sin(theta)) + pontoCentral[1];
        x = x * (raio + 2.0)/2.0;
        y = y * raio/2.0;

        circle[k].position = { x, y, 1.0 };
        circle[k].color = { 0.4, 0.5, 0.0 };
        k++;
    }

    //Buffers on the GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

//------------------------------ Exercício 7 & 8 --------------------------------------------
void elipseRNG() {
    Vertex pontosCirculo[12];
    float raio = 3.0;
    int k = 0;
    for (float i = 0.0; i <= 11.0; i++)
    {
        float rho = ((float)rand() / (float)(RAND_MAX));
        float phi = ((float)rand() / (float)(RAND_MAX) * (2 * PI));
        float x = (sqrt(rho) * cos(phi));
        float y = (sqrt(rho) * sin(phi));
        x = x * (raio + 2.0)*1.6;
        y = y * raio*1.6;

        pontosCirculo[k].position = { x, y, 1.0 };
        pontosCirculo[k].color = { 0.0f, 0.0f, 0.0f };
        k++;
    }

    //Buffers on the GPU
    glGenBuffers(1, &vboC);
    glBindBuffer(GL_ARRAY_BUFFER, vboC);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pontosCirculo), pontosCirculo, GL_STATIC_DRAW);

    //Modern Opengl requires that we use a vertex array object
    glGenVertexArrays(1, &vaoC);
    glBindVertexArray(vaoC);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Create vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    //Create fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    //Create a program shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

//------------------------------ GLFW_KEY ----------------------------------------------
void glfw_key(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(gWindow, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        count++;
        switch (count) {
        case 2:
            pontos();
            break;
        case 3:
            casa();
            break;
        case 4:
            sinc();
            break;
        case 5:
            circulo();
            break;
        case 6:
            circuloRNG();
            break;
        case 7:
            elipse();
            elipseRNG();
            break;
        case 8:
            elipseRNG();
            break;
        default:
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
            return;
        }
    }
}