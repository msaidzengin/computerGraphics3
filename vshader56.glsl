#version 150 



in   vec4 vPosition;
in   vec3 vNormal;
in   vec3 vColor;

// output values that will be interpolated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

out vec3 deneme;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    fN = normalize(vec3(ModelView*vec4(vNormal,0.0)));
    fE = -normalize(vec3(ModelView*vPosition));
    fL = normalize(vec3(LightPosition-ModelView*vPosition)); //light in camera coords
    
    deneme = vColor;
    
    gl_Position = Projection*ModelView*vPosition;
}
