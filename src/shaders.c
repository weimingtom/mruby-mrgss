/*
 * shaders.c
 *
 *  Created on: Jul 10, 2015
 *      Author: Manuel
 */
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <michelle.h>
#include <michelle/sdl.h>

const static GLchar* vertexSource = "#version 150\n"
    "in vec4 position;\n"
    "in vec2 texture_coord;\n"
    "out vec2 texcoord;\n"
    "out vec4 poscoord;\n"
    "uniform mat4 Model;\n"
    "uniform mat4 View;\n"
    "uniform mat4 Projection;\n"
    "void main() {\n"
    "texcoord = texture_coord;\n"
    "gl_Position = Projection * View * Model * position;\n"
    "}\n";

const static GLchar* fragmentSource = "#version 150\n"
    "in vec2 texcoord;\n"
    "in vec4 poscoord;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "	gl_FragColor = texture2D(tex, texcoord).xyzw;\n"
    "}";


static GLuint defaultShaderProgram = 0;

GLuint
compile_shader_program (GLuint vertexShader, GLuint fragmentShader)
{
  GLuint shaderProgram = glCreateProgram ();
  glAttachShader (shaderProgram, vertexShader);
  glAttachShader (shaderProgram, fragmentShader);
  glDeleteShader (vertexShader);
  glDeleteShader (fragmentShader);
  glLinkProgram (shaderProgram);
  glUseProgram (shaderProgram);
  return shaderProgram;
}

GLuint
compile_shader (const char *shader, int type)
{
  const char* shade = shader;
  GLuint shaderid = glCreateShader (type);
  glShaderSource (shaderid, 1, &shade, NULL);
  glCompileShader (shaderid);
  return shaderid;
}

GLuint
get_default_shader ()
{
  if(defaultShaderProgram == 0){

      defaultShaderProgram =  compile_shader_program(compile_shader(vertexSource, GL_VERTEX_SHADER), compile_shader(fragmentSource, GL_FRAGMENT_SHADER));
  }
  return defaultShaderProgram;
}
