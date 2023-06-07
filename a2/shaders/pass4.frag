// Pass 4 fragment shader
//
// Apply diffuse lighting to fragment.  Later do Phong lighting.
//
// Determine whether fragment is in shadow.  If so, reduce intensity to 50%.

#version 300 es
precision mediump float;

uniform sampler2D positionBuffer;   // texture [0,1]x[0,1] of position in VCS
uniform sampler2D normalBuffer;     // texture [0,1]x[0,1] of normal in VCS
uniform sampler2D depthBuffer;      // texture [0,1]x[0,1] of depth
uniform sampler2D blurredBuffer;    // texture [0,1]x[0,1] of blurred occlusion factors

uniform vec3 kd;   // material properties
uniform vec3 ks;
uniform vec3 Ia;
uniform float shininess;

uniform vec3 Iin;  // light colour
uniform vec3 L;    // direction to light in VCS

uniform int showOcclusion;

// inputs

in vec2 texCoords;

// outputs

out vec4 fragColour;   // fragment's final colour

void main()

{
  // YOUR CODE HERE
	vec4 vcsPosition = texture(positionBuffer, texCoords);
	vec4 vcsNormal = texture(normalBuffer, texCoords);
	float depth = texture(depthBuffer, texCoords).z;
	float occulsionFactor = texture(blurredBuffer, texCoords).r;
  
	// Discard fragments that are at maximum depth
	if (depth >= 1.0) discard;

	mediump vec3 N = vec3(normalize(vcsNormal));
	mediump vec3 V = vec3(normalize( -1.0 * vcsPosition)); 

	mediump float NdotL = (N.x * L.x) + (N.y * L.y) + (N.z + L.z);
	
	// light is below the surface
	if (NdotL < 0.0)
		NdotL = 0.0;

	// reflection direction
	mediump vec3 R = (2.0 * NdotL) * N - L;
	mediump float RdotV = (R.x * V.x) + (R.y * V.y) + (R.z + V.z);

	// diffuse component
	mediump vec3 diffuse;
	diffuse = NdotL * Iin * kd;  //NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b );

	// specular component
	mediump vec3 specular;
	specular = pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
  
	// light + colour 
	mediump vec3 Iout = diffuse + specular + Ia;

	// inlude occulsionFactor if needed
	Iout *= (showOcclusion == 1 ? occulsionFactor : 1.0);

	// Phong illumination
	fragColour = vec4( Iout, 1.0 );
}
