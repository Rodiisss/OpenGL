#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../shaders/shader.h"
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


  //Create Shader
  Shader myShader = createShader("shaders/vertex.glsl", "shaders/fragment.glsl");
  if (myShader.ID == 0) {
      // Handle error
      return -1;
  }

  Shader myShader2 = createShader("shaders/vertex.glsl", "shaders/fragmentdiff.glsl");
  if (myShader2.ID == 0) {
    return -1;
  }


  unsigned int VBO1, VBO2, VAO1, VAO2, EBO;
    float triangle1[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // left 
        -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // right
        -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top 
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

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

    use(&myShader);
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(myShader2.ID, "myColor");

    use(&myShader2);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

    glfwPollEvents();
    glfwSwapBuffers(window);
    
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  
  
}
