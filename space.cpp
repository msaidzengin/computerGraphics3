#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec4  point1;
GLfloat aspect;

const int NumVertices = 400000;

point4 points[NumVertices];
vec3   normals[NumVertices];
vec3   colors[NumVertices];

float groundSize = 0.3;

float movX = 3;
float movY = 0;
float rotateX = 0;
float rotateY = -0.5;

GLuint ModelView, Projection;
int Index = 0;

void quad(int a, int b, int c, int d) {
    
    vec4 u;
    vec4 v;
    vec3 normal;

    for (int i = -30; i < 30; i++) {
        for (int j = -30; j < 30; j++) {

            point4 verticesNew[8] = {
                point4(-groundSize + i * groundSize * 2, -groundSize ,  groundSize + j * groundSize * 2, 1.0),
                point4(-groundSize + i * groundSize * 2,  groundSize ,  groundSize + j * groundSize * 2 , 1.0),
                point4(groundSize + i * groundSize * 2,  groundSize ,  groundSize + j * groundSize * 2 , 1.0),
                point4(groundSize + i * groundSize * 2, -groundSize ,  groundSize + j * groundSize * 2 , 1.0),
                point4(-groundSize + i * groundSize * 2, -groundSize , -groundSize + j * groundSize * 2 , 1.0),
                point4(-groundSize + i * groundSize * 2,  groundSize , -groundSize + j * groundSize * 2 , 1.0),
                point4(groundSize + i * groundSize * 2,  groundSize , -groundSize + j * groundSize * 2 , 1.0),
                point4(groundSize + i * groundSize * 2, -groundSize , -groundSize + j * groundSize * 2 , 1.0)
            };

            u = verticesNew[b] - verticesNew[a];
            v = verticesNew[c] - verticesNew[b];
            normal = normalize(cross(u, v));

            if ((abs(i) + abs(j)) % 2 == 0) {
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[a]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[b]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[c]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[a]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[c]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.4, 0.4, 0.4 }; points[Index] = verticesNew[d]; Index++;
            }
            else {
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[a]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[b]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[c]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[a]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[c]; Index++;
                normals[Index] = normal; colors[Index] = vec3{ 0.8, 0.8, 0.8 }; points[Index] = verticesNew[d]; Index++;
            }
        }   
    }
}

void ground() {

    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}

void triangle(const point4& a, const point4& b, const point4& c) {

    normals[Index] = vec3(a.x, a.y, a.z); colors[Index] = vec3{ 0.4, 1, 0.4 }; points[Index] = a;  Index++;
    normals[Index] = vec3(b.x, b.y, b.z); colors[Index] = vec3{ 0.4, 1, 0.4 }; points[Index] = b;  Index++;
    normals[Index] = vec3(c.x, c.y, c.z); colors[Index] = vec3{ 0.4, 1, 0.4 }; points[Index] = c;  Index++;
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
    vec4(0, 0.0, 1.0, 1.0),
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

    ground();
    tetrahedron(5);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), sizeof(colors), colors);

    GLuint program = InitShader("vshader56.glsl", "fshader56.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(normals)));


    point4 light_position(2.0, 3.0, -1.0, 1.0);
    color4 light_ambient(0.2, 0.2, 0.2, 1.0);
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);

    color4 material_ambient(0.633, 0.727811, 0.633, 1.0);
    color4 material_diffuse(0.07568, 0.61424, 0.07568, 1.0);
    color4 material_specular(0.0215, 0.1745, 0.0215, 1.0);
    float  material_shininess = 76.8;

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
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 1);
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 projection = Perspective(60.0, aspect, 0.05, 3000.0);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
    

    point4 at(rotateX, rotateY, 0.0, 3);  //ilk parametre sağ-sol, ikinci yukarı aşağı, üçüncü .. , dördüncü uzaklık.
    point4 eye(0.0, 1.0, 2.0, 2.0);
    vec4   up(0.0, 1.0, 0.0, 1.0);

    const vec3 viewer_pos(movY, 1.2, movX);
    mat4 model_view = (LookAt(eye, at, up) * Translate(-viewer_pos) * RotateX(0) * RotateY(0) * RotateZ(0));

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);


    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glutSwapBuffers();

}


void mouse(int button, int state, int x, int y) {

    if (state == GLUT_DOWN) {
        switch (button) {
        case GLUT_LEFT_BUTTON:    break;
        case GLUT_MIDDLE_BUTTON:   break;
        case GLUT_RIGHT_BUTTON:    break;
        }
    }
}

void timer(int id) {

    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 033: case 'q': case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case 'w': case 'W':
            movX -= 0.1;
            break;
        case 's': case 'S':
            movX += 0.1;
            break;
        case 'a': case 'A':
            movY -= 0.1;
            break;
        case 'd': case 'D':
            movY += 0.1;
            break;
        case '4':
            rotateX -= 0.5;
            break;
        case '6':
            rotateX += 0.5;
            break;
        case '8':
            rotateY += 0.5;
            break;
        case '2':
            rotateY -= 0.5;
            break;
    } 
}

void reshape(int width, int height) {

    glViewport(0, 0, width, height);
    aspect = GLfloat(width) / height;
    mat4 projection = Perspective(60.0, aspect, 0.05, 3000.0);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Homework 3");
    glewExperimental = GL_TRUE;
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutTimerFunc(20, timer, 0);
    glutMainLoop();
    return 0;
}
