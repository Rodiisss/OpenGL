#include <stdio.h>
#include <stdlib.h>
#include "../include/glad/gl.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const char *fragmentShaderSourceDiff = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vec4(0.3f, 0.2f, 0.6f, 1.0f);\n"
  "}\0";


int main (void){

  //Initialize GLFW
  if (!glfwInit()){
    exit(EXIT_FAILURE);
  }

  //Create window
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT, "test", NULL, NULL);
  if (!window){
    printf("Failed to create window\n");
    glfwDestroyWindow(window);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  //Initialize Glad (Must be initialized after creating an active OpenGL context)
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("Failed to initialize glad\n");
    return -1;
  }   


  //Create Vertex Shader
  
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
  }

  //Create Fragment Shader 

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
    
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
  }

  unsigned int fragmentShaderDiff;
  fragmentShaderDiff = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderDiff, 1, &fragmentShaderSourceDiff, NULL);
  glCompileShader(fragmentShaderDiff);
    
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(fragmentShaderDiff, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::DIFF::COMPILATION_FAILED\n %s", infoLog);
  }


  //Link vertexShader and fragmentShader into a Shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  }

  unsigned int shaderProgramDiff;
  shaderProgramDiff = glCreateProgram();
  glAttachShader(shaderProgramDiff, vertexShader);
  glAttachShader(shaderProgramDiff, fragmentShaderDiff);
  glLinkProgram(shaderProgramDiff);

  glGetProgramiv(shaderProgramDiff, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgramDiff, 512, NULL, infoLog);
  }


  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);  
  glDeleteShader(fragmentShaderDiff);

  unsigned int VBO1, VBO2, VAO1, VAO2, EBO;
    float triangle1[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };   

    float triangle2[] = {
       0.0f, -0.5f, 0.0f,  // left
       0.9f, -0.5f, 0.0f,  // right
       0.45f, 0.5f, 0.0f   // top 
      
    };

  // float vertices[] = {
  //      0.75f,  0.75f, 0.0f,  // top right
  //      0.75f, -0.75f, 0.0f,  // bottom right
  //     -0.75f, -0.75f, 0.0f,  // bottom left
  //     -0.75f,  0.75f, 0.0f   // top left 
  // };
  // unsigned int indices[] = {  // note that we start from 0!
  //     0, 1, 3,   // first triangle
  //     1, 2, 3    // second triangle
  // };  
  

  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &VBO2);
  glGenVertexArrays(1, &VAO1);
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.60f, 0.81f, 0.83f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(shaderProgram);
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(shaderProgramDiff);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

    glfwPollEvents();
    glfwSwapBuffers(window);
    
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  
  
}
