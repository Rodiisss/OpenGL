#version 330 core
out vec4 FragColor;
uniform vec4 myColor;
in vec4 vertexPosition;

void main(){
    FragColor = vertexPosition;
}
