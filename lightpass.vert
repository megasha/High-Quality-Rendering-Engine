
uniform mat4 LightView;
uniform mat4 LightProjection;
uniform mat4 Model;

void main(){
    gl_Position = LightProjection * LightView * Model * gl_Vertex;
}