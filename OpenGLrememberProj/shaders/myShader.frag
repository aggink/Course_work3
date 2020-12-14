varying vec3 Position;
vec3 Normal;
varying vec2 texCoord;
varying vec3 iLightPos_mv;
varying vec3 iCamPos_mv;

uniform sampler2D iTexture0;
uniform sampler2D iTexture1;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 ma;
uniform vec3 md;
uniform vec3 ms;
 
vec3 texture0 = vec3(texture2D(iTexture0, texCoord).rgb);

void main(void)
{
	Normal = vec3(texture2D(iTexture1, texCoord).rgb);
    Normal = normalize(Normal*2.0 - 1.0);
	
	//ambient
    vec3 color_amb = Ia*ma;
	
	//diffuse
	vec3 light_vector = normalize(iLightPos_mv - Position);
	vec3 color_dif = Id*md*dot( light_vector, Normal);
	
	//specular
	vec3 cam_vector = normalize(iCamPos_mv - Position);
	vec3 reflect_vector = reflect(-light_vector,Normal);
	float cosRC = max(0.0,dot(cam_vector,reflect_vector));	
	vec3 col_spec = Is*ms*pow(cosRC,300);
	
	gl_FragColor = vec4(texture0 * (color_amb + color_dif + col_spec), 1.0);
}