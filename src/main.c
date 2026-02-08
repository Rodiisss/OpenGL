#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../shaders/shader.h"
#include "../include/stb_image.h"
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

  stbi_set_flip_vertically_on_load(1);
  int width, height, nrChannels;
  unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

  if (!data) {
      printf("Failed to load texture\n");
      return -1;
  }

  printf("Loaded texture: %dx%d with %d channels\n", width, height, nrChannels);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

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


  unsigned int VAO, VBO, EBO;

  float vertices[] = {
     0.5f,  0.5f, 0.0f,    1.0f, 1.0f,// top right
     0.5f, -0.5f, 0.0f,    1.0f, 0.0f,// bottom right
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,// bottom left
    -0.5f,  0.5f, 0.0f,    0.f, 1.0f  // top left 
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };  
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);


  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  
  
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  
  // glBindVertexArray(0);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.60f, 0.81f, 0.83f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    float timeValue = glfwGetTime(); 
    float offset = sin(timeValue) * 0.5f;  // Oscillate between -0.5 and 0.5

    glBindTexture(GL_TEXTURE_2D, texture);
    
    use(&myShader);
    setFloat(&myShader, "offset", offset);


    // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(myShader2.ID, "myColor");

    // use(&myShader2);
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

    glfwPollEvents();
    glfwSwapBuffers(window);
    
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  
  
}
