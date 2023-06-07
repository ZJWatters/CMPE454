// strokefont.h


#ifndef STROKEFONT_H
#define STROKEFONT_H


#include "headers.h"
#include "gpuProgram.h"
#include <string>


typedef enum { LEFT, CENTRE, RIGHT } Alignment;


class StrokeFont {

    static char* fontVertexShader;
    static char* fontFragmentShader;

    GPUProgram* gpuProg;

public:

    StrokeFont() {
        gpuProg = new GPUProgram();
        gpuProg->init(fontVertexShader, fontFragmentShader, "strokefont");
    }

    void drawStrokeString(string str, float x, float y, float height, float theta, Alignment alignment);
};


#endif
