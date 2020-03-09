#include "Angel.h"

const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;
const int NumVertices = 3 * NumTriangles;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

point4 points[NumVertices];
vec3   normals[NumVertices];
GLuint  ModelView, Projection;
int Index = 0;

void triangle(const point4& a, const point4& b, const point4& c) {

    normals[Index] = vec3(a.x, a.y, a.z);  points[Index] = a;  Index++;
    normals[Index] = vec3(b.x, b.y, b.z);  points[Index] = b;  Index++;
    normals[Index] = vec3(c.x, c.y, c.z);  points[Index] = c;  Index++;
}

point4 unit(const point4& p) {
    float len = p.x * p.x + p.y * p.y + p.z * p.z;

    point4 t;
    if (len > DivideByZeroTolerance) {
        t = p / sqrt(len);
        t.w = 1.0;
    }

    return t;
}

void divide_triangle(const point4& a, const point4& b, const point4& c, int count) {
    if (count > 0) {
        point4 v1 = unit(a + b);
        point4 v2 = unit(a + c);
        point4 v3 = unit(b + c);
        divide_triangle(a, v1, v2, count - 1);
        divide_triangle(c, v2, v3, count - 1);
        divide_triangle(b, v3, v1, count - 1);
        divide_triangle(v1, v3, v2, count - 1);
    }
    else {
        triangle(a, b, c);
    }
}

void tetrahedron(int count) {
    point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
    };

    divide_triangle(v[0], v[1], v[2], count);
    divide_triangle(v[3], v[2], v[1], count);
    divide_triangle(v[0], v[3], v[1], count);
    divide_triangle(v[0], v[2], v[3], count);
}

void init() {
    tetrahedron(NumTimesToSubdivide);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
    NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
    sizeof(normals), normals);

    GLuint program = InitShader("vshader56.glsl", "fshader56.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
    BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
    BUFFER_OFFSET(sizeof(points)));

    point4 light_position(2.0, 3.0, 2.0, 1.0);
    color4 light_ambient(0.2, 0.2, 0.8, 1.0);
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);

    color4 material_ambient(0.633, 0.727811, 0.633, 1.0);
    color4 material_diffuse(0.07568, 0.61424, 0.07568, 1.0);
    color4 material_specular(0.0215, 0.1745, 0.0215, 1.0);
    float  material_shininess = 960.8;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);

    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);

    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    point4 at(-1.0, -1.0, 0.0, 5);
    point4 eye(0.0, 0.0, 5, 2.0);
    vec4   up(0.0, 2.0, 1.0, 0.0);

    mat4 model_view = LookAt(eye, at, up);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);



    point4 at2(1.0, 1.0, 0.0, 5);
    point4 eye2(0.0, 0.0, 5, 2.0);
    vec4   up2(0.0, 2.0, 1.0, 0.0);

    mat4 model_view2 = LookAt(eye2, at2, up2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view2);

    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 033: // Escape Key
    case 'q': case 'Q':
        exit(EXIT_SUCCESS);
        break;
    }
}

void reshape(int width, int height) {

    glViewport(0, 0, width, height);
    GLfloat aspect = GLfloat(width) / height;
    mat4  projection = Perspective(60.0, aspect, 0.5, 20.0);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Sphere");
    glewExperimental = GL_TRUE;
    glewInit();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
