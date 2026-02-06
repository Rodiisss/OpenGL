#!/bin/bash

gcc -lglfw -lGL -lm src/window.c src/gl.c shaders/shader.c  -o window 
