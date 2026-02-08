#!/bin/bash

gcc -g -lglfw -lGL -lm src/main.c src/gl.c shaders/shader.c src/stb_image.c  -o bin/LearnOpengGL 
