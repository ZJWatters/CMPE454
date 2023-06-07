// Pass 2 fragment shader
//
// Compute the ambient occlusion factor

#version 300 es
precision mediump float;

#define NUM_SAMPLE_OFFSETS 1000  // must match the same renderer.h


// gBuffers

uniform sampler2D positionBuffer;   // texture [0,1]x[0,1] of position in VCS
uniform sampler2D normalBuffer;     // texture [0,1]x[0,1] of normal in VCS
uniform sampler2D depthBuffer;      // texture [0,1]x[0,1] of depth

// Sample offsets

uniform vec3 sampleOffsets[NUM_SAMPLE_OFFSETS]; // random (x,y,z) offsets, all with z > 0 and all of length <= 1
uniform int numSampleOffsetsToUse;              // number of offsets to use when sampling
uniform float offsetScale;                      // scale each offset by this

uniform mat4 VCS_to_CCS;  // projection matrix

// inputs

in vec2 texCoords;

// outputs

out float occlusionFactor;


void main()

{
  // Output the occlusion factor
  //
  // YOUR CODE HERE
	vec3 vcsPosition = texture(positionBuffer, texCoords).xyz;
	vec3 vcsNormal = texture(normalBuffer, texCoords).rgb;
	float depth = texture(depthBuffer, texCoords).z;

    // Discard fragments that are at maximum depth
	if (depth >= 1.0) discard;

	vec3 zcsNormal = normalize(vcsNormal);

    // find a perpendicular vector to the normal
    vec3 perpY = vec3(0.0,0.0,0.0);

    if (zcsNormal.x == 0.0)
        if (zcsNormal.y == 0.0 || zcsNormal.z == 0.0)
            perpY.x = 1.0;     /* v = (0 0 z) or (0 y 0) */
        else {
            perpY.y = -zcsNormal.z; /* v = (0 y z) */
            perpY.z = zcsNormal.y;
        }
    else if (zcsNormal.y == 0.0)
        if (zcsNormal.z == 0.0)
            perpY.z = 1.0;     /* v = (x 0 0) */
        else {
            perpY.x = -zcsNormal.z; /* v = (x 0 z) */
            perpY.z = zcsNormal.x;
        }
    else {
        perpY.x = -zcsNormal.y;   /* v = (x y z) or (x y 0) */
        perpY.y = zcsNormal.x;
    }
    
    // 3rd axis = cross product of first 2 vectors
	vec3 perpX = cross(zcsNormal,perpY);

    // rotation matrix
    mat3 R = mat3(perpX, perpY, zcsNormal);  

    occlusionFactor = 0.0;
    for (int i = 0; i<numSampleOffsetsToUse; i++){

        // samplePos is the postition in the VCS
        vec3 samplePos = vcsPosition + (offsetScale * R * sampleOffsets[i]);

        // position in the CCS to find ccsDepth in [0,1]
        vec4 position = VCS_to_CCS * vec4(samplePos,1.0);
        float ccsDepth = position.z*0.5 + 0.5f;

        // get depth of the sample and update occlusion factor accordingly
        float gBufferDepth = texture(depthBuffer, samplePos.xy).z;
        occlusionFactor += (ccsDepth <= gBufferDepth ? 1.0 : 0.0);
       }
   
    occlusionFactor = (occlusionFactor/float(numSampleOffsetsToUse));
}
