/*{
	"CREDIT" : "Simple Asset by Bruce Lane",
	"CATEGORIES" : [
		"ci"
	],
	"DESCRIPTION": "",
	"INPUTS": [
		{
			"NAME": "inputImage",
			"TYPE" : "image"
		},
		{
			"NAME": "iFactor",
			"TYPE" : "float",
			"MIN" : 0.1,
			"MAX" : 2.0,
			"DEFAULT" : 1.0
		},
		{
			"NAME": "iRotationSpeed",
			"TYPE" : "float",
			"MIN" : -0.3,
			"MAX" : 0.3,
			"DEFAULT" : 0.0
		},
		{
			"NAME": "iEffect",
			"TYPE" : "float",
			"MIN" : -0.5,
			"MAX" : 0.5,
			"DEFAULT" : 0.0
		}
	],
}
*/

void main(void)
{
    vec2 uv = gl_FragCoord.xy / RENDERSIZE.xy; 
 	uv.x -= 0.15;
    uv.x *= 1.5;

    // stretch centered
	
	float xZ = (uv.x - 0.5)*(0.1-iEffect)*2.0;
	float yZ = (uv.y - 0.5)*(0.1-iEffect)*2.0;
	vec2 cZ = vec2(xZ, yZ);
	uv = uv+cZ;
	
	// rotate
	float rad = radians(360.0 * fract(iTime*iRotationSpeed));
	mat2 rotate = mat2(cos(rad),sin(rad),-sin(rad),cos(rad));
	uv = rotate * (uv - 0.5) + 0.5;
    vec3 rgb = texture(iChannel0, uv).xyz;
    fragColor=vec4(rgb, 1.0);	
}
