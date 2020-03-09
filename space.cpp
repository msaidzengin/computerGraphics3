#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec4  point1;

const int NumVertices = 300000;

point4 points[NumVertices];
vec3   normals[NumVertices];
vec3   colors[NumVertices];

float groundSize = 0.3;

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

float movX = 3;
float movY = 0;
int rotate = 0;

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

void colorcube() {

    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}

void init() {

    colorcube();

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
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const vec3 viewer_pos(movY, 1.2, movX);
    mat4 model_view = ( Translate(-viewer_pos) * RotateX(Theta[1]) * RotateY(Theta[0]) * RotateZ(Theta[1]));

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);


    glutSwapBuffers();

}

void mouse(int button, int state, int x, int y) {

    if (state == GLUT_DOWN) {
        switch (button) {
        case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
        case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
        case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

void timer(int id) {

    if (rotate == 1) {
        Theta[Axis] -= 0.5;
        rotate = 0;
    }
    else if (rotate == -1) {
        Theta[Axis] += 0.5;
        rotate = 0;
    }
    else {
        Theta[Axis] += 0;
    }
    
    if (Theta[Axis] > 360.0) {
        Theta[Axis] -= 360.0;
    }

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
        case 'o': case 'O':
            rotate = 1;
            break;
        case 'p': case 'P':
            rotate = -1;
            break;
    } 
}

void reshape(int width, int height) {

    glViewport(0, 0, width, height);
    GLfloat aspect = GLfloat(width) / height;
    mat4 projection = Perspective(60.0, aspect, 0.05, 3000.0);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Color Cube");
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
