#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <thread>
#include <vector>
#include "stb_image.h"

#define PI 3.14159265359
#define GLEW_STATIC
using namespace std;

GLFWwindow* window;

int wwindowsize = 1200;
int hwindowsize = 700;
int xmouse, ymouse;

void Circle(int sze) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        glVertex3d(cos(delta) * sze / 2, sin(delta) * sze / 2, 0);
    }
    glEnd();
}

void Square(int sze) {
    glBegin(GL_QUADS);
    glVertex2d(-sze/2, -sze/2);
    glVertex2d(sze / 2, -sze / 2);
    glVertex2d(-sze / 2, sze / 2);
    glVertex2d(-sze / 2, sze / 2);
    glEnd();
}

const int drawsize = 25;
struct Point {
    double red, green, blue;
    int xpos, ypos;
    int sze;
    int id;
} p;
vector<Point> v;
int cnt = 0;



void reshape() {
    glfwGetWindowSize(window, &wwindowsize, &hwindowsize);
    glViewport(0, 0, wwindowsize, hwindowsize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, wwindowsize, hwindowsize, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    for (int i = v.size() - 2; i >= 0; --i) {
        glPushMatrix();
        glTranslated(v[i + 1].xpos, v[i + 1].ypos, 0);
        glColor3d(v[i + 1].red, v[i + 1].green, v[i + 1].blue);
        Circle(v[i + 1].sze);
        glPopMatrix();
        if ((abs(v[i + 1].xpos - v[i].xpos) >= drawsize / 2 || abs(v[i + 1].ypos - v[i].ypos) >= drawsize * 3 / 4) && v[i + 1].id == v[i].id) {
            glLineWidth(drawsize); 
            glPushMatrix();
            glBegin(GL_LINES);
            glColor3d(v[i + 1].red, v[i + 1].green, v[i + 1].blue);
            glVertex2d(v[i + 1].xpos, v[i + 1].ypos);
            glColor3d(v[i].red, v[i].green, v[i].blue);
            glVertex2d(v[i].xpos, v[i].ypos);
            glEnd();
        }
    }
}

void nkey(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) {
        exit(0);
    }

}
bool press = false;
void motion(GLFWwindow* window, double x, double y) {
    if (press) {
        p.xpos = x;
        p.ypos = y;
        p.red = 0;
        p.green = 0.2;
        p.blue = 0.8;
        p.sze = drawsize;
        p.id = cnt;
        v.push_back(p);
    }
    xmouse = x;
    ymouse = y;
}
void mouse(GLFWwindow* window, int button, int actions, int moods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (actions == GLFW_PRESS) {
            press = true;
            cnt++;
            p.xpos = xmouse;
            p.ypos = ymouse;
            p.red = 0;
            p.green = 0.2;
            p.blue = 0.8;
            p.sze = drawsize;
            p.id = cnt;
            v.push_back(p);
        }
        else {
            press = false;
        }
    }

}



int main() {
    if (!glfwInit()) return -1;  
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    

    window = glfwCreateWindow(wwindowsize, hwindowsize, "", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    reshape();
    
    glfwSetKeyCallback(window, nkey);
    glfwSetCursorPosCallback(window, motion);
    glfwSetMouseButtonCallback(window, mouse);
    glfwMakeContextCurrent(window);
    

    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    glfwSetCursor(window, cursor);
    

    GLFWimage images[1];
    images[0].pixels = stbi_load("icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        reshape();
        display();
        
        
        




        //glFlush();
        glfwPollEvents();
        glfwSwapBuffers(window);
        this_thread::sleep_for(chrono::microseconds(1000 / 45));
    }

    glfwTerminate();
    return 0;
}