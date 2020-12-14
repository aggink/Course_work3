varying vec2 texCoord; 
varying vec3 Normal;
varying vec3 Position;
varying vec3 iLightPos_mv;
varying vec3 iCamPos_mv;

uniform vec3 light_pos;
uniform vec3 camera;
void main(void) 
{     
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix*gl_Vertex;   //умножаем матрицу проекции на видовую матрицу и на координаты точки
	Position = gl_Vertex.xyz;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;   
	texCoord = gl_TexCoord[0].xy;   //считываем текстурые координаты в варинг
	Normal = normalize(gl_Normal); /*gl_NormalMatrix*/
	
	iLightPos_mv = (gl_ModelViewMatrix*vec4(light_pos,1.0)).xyz;    
    iCamPos_mv = (gl_ModelViewMatrix*vec4(camera,1.0)).xyz;
}