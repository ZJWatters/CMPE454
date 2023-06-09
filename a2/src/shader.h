// shader.h

#ifndef SHADER_H
#define SHADER_H

#include "linalg.h"

#define SHADER_DIR "../shaders/"

extern GLuint windowWidth, windowHeight;
extern float fovy;
extern vec3 eyePosition;
extern vec3 lookAt;
extern float worldRadius;
extern bool showAxes;
extern bool showOcclusion;
extern int numSampleOffsetsToUse;
extern float offsetScale;
extern float blurRadius;

void glErrorReport( char *where );


#endif
