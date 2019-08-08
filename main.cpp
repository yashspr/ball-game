#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLfloat xball=0.0f, yball=0.0f; // initial position of the ball at the center of the screen
GLfloat xmax=1.0,ymax=1.0, xmin=-1.0,ymin=-1.0; // boundaries of the screen
GLfloat xpaddle=-0.15f; //position of the paddle. initially placed at the middle.
GLfloat paddlelength=0.3f;// length of the paddle
GLfloat xspeed=0.02f;
GLfloat yspeed=0.02f;

int millisec=30;
int score=0,save=0,fall=0;
float old_xspeed,old_yspeed;
bool pause=false;
int xyz;

void settings_menu(int id)
{
	switch(id)
	{
		case 1: pause=true; break;
		case 2: pause=false; break;
		case 3: exit(0); break; //quit
	}
}

void draw(int flag,float x,float y,float z)
{
	glRasterPos3f(x, y,z);
	char scoreDisplay[9];
	if(flag ==0 )
        sprintf(scoreDisplay, "Score: %d", score);
    else if(flag == 1)
        sprintf(scoreDisplay, "Lives: %d", 3-fall);
    else if(flag==2)
        sprintf(scoreDisplay, "Game Over");
	for(int c=0; c <strlen(scoreDisplay); c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreDisplay[c]);
	}
}

void display(void)
{
    if(pause==false)
    {

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        draw(0,0.5,0.9,0.0);
        draw(1,-0.9,0.9,0.0);

        // paddle drawing
        glBegin(GL_QUADS);
        if(fall==0) // used to control the color of the paddle
            glColor3f(1.0,1.0,0.0);
        else if(fall==1)
            glColor3f(1.0,0.5,0.0);
        else if(fall>=2)
            glColor3f(1.0,0.0,0.0);
        glVertex3f(xpaddle,-0.9,0.0); // top left point
        glVertex3f(xpaddle+paddlelength,-0.9,0.0); // top right point
        glVertex3f(xpaddle+paddlelength,-1.0,0.0); // bottom right point
        glVertex3f(xpaddle,-1.0,0.0); // bottom left point
        glEnd();

        // ball drawing
        glBegin(GL_QUADS);
        glColor3f(0.0,1.0,1.0); // green ball
        glVertex3f(xball,yball,0.0); // top left corner
        glVertex3f(xball+0.08,yball,0.0); // top right corner
        glVertex3f(xball+0.08,yball-0.08,0.0); // bottom right corner
        glVertex3f(xball,yball-0.08,0.0); // bottom left corner
        glEnd();

        xball+=xspeed;
        yball+=yspeed;

        if(xball>=xmax-0.08) // if ball touches right boundary
        {
            xball=xmax-0.08;
            xspeed=-xspeed;
        }
        else if(xball<=xmin) // if ball touches left boundary
        {
            xball=xmin;
            xspeed=-xspeed;
        }

        if(yball>=ymax) // if ball touches top boundary
        {
            yball=ymax;
            yspeed=-yspeed;
        }
        else if((yball-0.08<=ymin+0.1)) // if ball comes to the bottom of the screen near the paddle
        {
            //normal condition- if ball hits the paddle somewhere in between
            if((xpaddle<=xball+0.08)&&(xball<=xpaddle+paddlelength))
            {
                yball = ymin+0.1+0.082;
                yspeed=-yspeed;
                score++;
                save++;
                xspeed=xspeed + (xspeed/fabs(xspeed)) * 0.002;
                yspeed=yspeed - 0.002;
            }
        }

        if(yball-0.08<=ymin) // if ball touches the bottom of screen
        {
            paddlelength=paddlelength-0.07;
            fall++;
            if(fall==4){
                pause = true;
                draw(2,-0.2,0,0);
                glutPostRedisplay();
                glutDestroyMenu(xyz);
            }
            else {
                yspeed=-yspeed;
                xspeed=xspeed+0.0015;
                yspeed=yspeed+0.006;
            }
        }

        // prevent ball to slow down too much
        if(fabs(yspeed) < 0.008) // to prevent ball from just keep bouncing horizontally
            yspeed += (xspeed/fabs(xspeed)) * 0.02;

        if(fabs(xspeed) < 0.008) // to prevent ball from just bouncing vertically
            xspeed += (yspeed/fabs(yspeed)) * 0.02;

        if(save==10)
        {
            paddlelength=paddlelength+0.04;
            save=0;
        }

        glutSwapBuffers();
    }
}

void Keyboard(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        if(xpaddle>=-1.0)
            xpaddle-=0.1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        if(xpaddle+paddlelength<=1.0)
            xpaddle+=0.1;
    }
    if(key=='Q')
        exit(0);
}

// called every 30 millisecs to redraw the picture i.e call display function
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(millisec,Timer,0);
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640,600);
    glutInitWindowPosition(50,50);
    glutCreateWindow("BALL GAME");
    glutSpecialFunc(Keyboard);
    xyz = glutCreateMenu(settings_menu);
    glutAddMenuEntry("PAUSE",1);
    glutAddMenuEntry("RESUME",2);
    glutAddMenuEntry("QUIT GAME",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //glutPostRedisplay();
    glutDisplayFunc(display);
    glutTimerFunc(0,Timer,0);
    glClearColor(0.0,0.0,0.0,1.0);
    glutMainLoop();
    return 0;
}
