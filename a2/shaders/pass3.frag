// Pass 3 fragment shader
//
// Blur the occlusion values

#version 300 es
precision mediump float;


uniform sampler2D occlusionBuffer;
uniform sampler2D depthBuffer;
uniform float blurRadius;
uniform vec2 texelSize;

// inputs

in vec2 texCoords;

// outputs

out float blurredOcclusionFactor;


void main()

{
  // Average the occlusion factors in the neighbourhood
  //
  // YOUR CODE HERE
  
  //position of fragment
  vec2 position = vec2(texCoords.x,texCoords.y);

  // Discard fragments that are at maximum depth
  float depth = texture(depthBuffer, texCoords).z;
  if (depth >= 1.0) discard;

  // position of the leftmost corner texel in the neighbourhood
  vec2 firstTexel = position - ((2.0*blurRadius+1.0)/2.0);

  // intilizing variables
  float occSum = 0.0;
  float texelCount = 0.0;
  
  // loop through texels in the square neighbourhood summing the occlusion values
  for(float texelWidth = firstTexel.x; texelWidth < (2.0*blurRadius+1.0); texelWidth += texelSize.x){
	for(float texelHeight = firstTexel.y; texelHeight < (2.0*blurRadius+1.0); texelHeight += texelSize.y){
		occSum += texture(occlusionBuffer,vec2(texelWidth,texelHeight)).r;
		texelCount+=1.0;
	}
  }

  // average over the total number of texels
  occSum = occSum/texelCount;

  blurredOcclusionFactor = occSum;
}
