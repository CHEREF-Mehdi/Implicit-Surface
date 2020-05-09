#include "surfImp.h"

int N = 0;
int foncDensite = 1, foncPotentiel = 1;

int mp = -1, addPoint = 0;
bool isSelecting = false;

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Surfaces implicites");
    glEnable(GL_DEPTH_TEST);

    createMenu();

    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutPostRedisplay();
    glutSwapBuffers();
    glutMainLoop();

    return 0;
}

/**************************** disatance ****************************/
float distance(Point P1, Point P2)
{
    return sqrt(pow(P2.x - P1.x, 2) + pow(P2.y - P1.y, 2));
}

/**************************** afficher les points ****************************/
void DrawPoint()
{
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glInitNames();
    glPushName(1);
    for (int i = 0; i < N; i++)
    {
        glLoadName(i);
        glBegin(GL_POINTS);
        glVertex2f(P[i].x, P[i].y);
        glEnd();
    }
}

/**************************** fonctions de potentiel ****************************/
float Murakami(float r)
{
    return (r >= 0 && r < Ri) ? pow(1 - pow((r / Ri), 2), 2) : 0.0;
}

float Nishimura(float r)
{
    if (r >= 0 && r < Ri / 3.0)
        return 1 - 3 * pow(r / Ri, 2);
    else if (r >= Ri / 3.0 && r < Ri)
        return 3 / 2 * pow(1 - (r / Ri), 2);
    return 0.0;
}

float Wyvill(float r)
{
    if (r >= 0 && r < Ri)
    {
        float X1 = -4.0 / 9.0, X2 = (17.0 / 9.0), X3 = (-22.0 / 9.0);
        X1 = X1 * pow(r / Ri, 6);
        X2 = X2 * pow(r / Ri, 4);
        X3 = X3 * pow(r / Ri, 2);
        return X1 + X2 + X3 + 1;
    }
    return 0.0;
}

float valueOfPotentielFunc(Point p1, Point p2)
{
    if (foncPotentiel == 1)
        return Murakami(distance(p1, p2));

    else if (foncPotentiel == 2)
        return Nishimura(distance(p1, p2));

    return Wyvill(distance(p1, p2));
}

/**************************** fonctions de densité ****************************/

float Melange(Point point)
{
    float V = 0;
    for (int i = 0; i < N; i++)
        V = V + valueOfPotentielFunc(point, P[i]);
    return V;
}

float Union(Point point)
{
    float V = 0;
    for (int i = 0; i < N; i++)
        V = std::max(V, valueOfPotentielFunc(point, P[i]));
    return V;
}

float Sub(Point point)
{
    float V = WIDTH_GRILLE;
    for (int i = 0; i < N; i++)
        V = std::min(V, valueOfPotentielFunc(point, P[i]));
    return V;
}

float ValueOfDensiteFunc(Point point)
{
    if (foncDensite == 1) //Melange
        return Melange(point);
    else if (foncDensite == 2) //Union
        return Union(point);
    //Intersection
    return Sub(point);
}

/**************************** Polygonalisation ****************************/
void SetUpEdges(Square c)
{

    Point P1, P2, P3, P4;
    float distance;
    //cas 0 do nothing
    if (!c.A.isIn() && !c.B.isIn() && !c.C.isIn() && c.D.isIn()) //cas 1
    {
        P1.x = c.A.x;
        distance = (T - c.D.d) / (c.A.d - c.D.d);
        P1.y = (1 - distance) * c.D.y + distance * c.A.y;

        distance = (T - c.D.d) / (c.C.d - c.D.d);
        P2.x = (1 - distance) * c.D.x + distance * c.C.x;
        P2.y = c.D.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (!c.A.isIn() && !c.B.isIn() && c.C.isIn() && !c.D.isIn()) //cas 2
    {
        distance = (T - c.C.d) / (c.D.d - c.C.d);
        P1.x = (1 - distance) * c.C.x + distance * c.D.x;
        P1.y = c.C.y;

        P2.x = c.B.x;
        distance = (T - c.C.d) / (c.B.d - c.C.d);
        P2.y = (1 - distance) * c.C.y + distance * c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (!c.A.isIn() && !c.B.isIn() && c.C.isIn() && c.D.isIn()) //cas 3
    {
        P1.x = c.A.x;
        distance = (T - c.D.d) / (c.A.d - c.D.d);
        P1.y = (1 - distance) * c.D.y + distance * c.A.y;

        P2.x = c.B.x;
        distance = (T - c.C.d) / (c.B.d - c.C.d);
        P2.y = (1 - distance) * c.C.y + distance * c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (!c.A.isIn() && c.B.isIn() && !c.C.isIn() && !c.D.isIn()) //cas 4
    {
        distance = (T - c.B.d) / (c.A.d - c.B.d);
        P1.x = (1 - distance) * c.B.x + distance * c.A.x;
        P1.y = c.B.y;

        P2.x = c.B.x;
        distance = (T - c.B.d) / (c.C.d - c.B.d);
        P2.y = (1 - distance) * c.B.y + distance * c.C.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (!c.A.isIn() && c.B.isIn() && !c.C.isIn() && c.D.isIn()) //cas 5
    {
        P1.x = c.A.x;
        distance = (T - c.D.d) / (c.A.d - c.D.d);
        P1.y = (1 - distance) * c.D.y + distance * c.A.y;

        distance = (T - c.D.d) / (c.C.d - c.D.d);
        P2.x = (1 - distance) * c.D.x + distance * c.C.x;
        P2.y = c.D.y;

        distance = (T - c.B.d) / (c.A.d - c.B.d);
        P3.x = (1 - distance) * c.B.x + distance * c.A.x;
        P3.y = c.B.y;

        P4.x = c.B.x;
        distance = (T - c.B.d) / (c.C.d - c.B.d);
        P4.y = (1 - distance) * c.B.y + distance * c.C.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glVertex2f(P3.x, P3.y);
        glVertex2f(P4.x, P4.y);
        glEnd();
    }
    else if (!c.A.isIn() && c.B.isIn() && c.C.isIn() && !c.D.isIn()) //cas 6
    {
        distance = (T - c.C.d) / (c.D.d - c.C.d);
        P1.x = (1 - distance) * c.C.x + distance * c.D.x;
        P1.y = c.C.y;

        distance = (T - c.B.d) / (c.A.d - c.B.d);
        P2.x = (1 - distance) * c.B.x + distance * c.A.x;
        P2.y = c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (!c.A.isIn() && c.B.isIn() && c.C.isIn() && c.D.isIn()) //cas 7
    {
        P1.x = c.A.x;
        distance = (T - c.D.d) / (c.A.d - c.D.d);
        P1.y = (1 - distance) * c.D.y + distance * c.A.y;

        distance = (T - c.B.d) / (c.A.d - c.B.d);
        P2.x = (1 - distance) * c.B.x + distance * c.A.x;
        P2.y = c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && !c.B.isIn() && !c.C.isIn() && !c.D.isIn()) //cas 8
    {
        P1.x = c.A.x;
        distance = (T - c.A.d) / (c.D.d - c.A.d);
        P1.y = (1 - distance) * c.A.y + distance * c.D.y;

        distance = (T - c.A.d) / (c.B.d - c.A.d);
        P2.x = (1 - distance) * c.A.x + distance * c.B.x;
        P2.y = c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && !c.B.isIn() && !c.C.isIn() && c.D.isIn()) //cas 9
    {
        distance = (T - c.D.d) / (c.C.d - c.D.d);
        P1.x = (1 - distance) * c.D.x + distance * c.C.x;
        P1.y = c.D.y;

        distance = (T - c.A.d) / (c.B.d - c.A.d);
        P2.x = (1 - distance) * c.A.x + distance * c.B.x;
        P2.y = c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && !c.B.isIn() && c.C.isIn() && !c.D.isIn()) //cas 10
    {
        P1.x = c.A.x;
        distance = (T - c.A.d) / (c.D.d - c.A.d);
        P1.y = (1 - distance) * c.A.y + distance * c.D.y;

        distance = (T - c.A.d) / (c.B.d - c.A.d);
        P2.x = (1 - distance) * c.A.x + distance * c.B.x;
        P2.y = c.B.y;

        distance = (T - c.C.d) / (c.D.d - c.C.d);
        P3.x = (1 - distance) * c.C.x + distance * c.D.x;
        P3.y = c.C.y;

        P4.x = c.B.x;
        distance = (T - c.C.d) / (c.B.d - c.C.d);
        P4.y = (1 - distance) * c.C.y + distance * c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glVertex2f(P3.x, P3.y);
        glVertex2f(P4.x, P4.y);
        glEnd();
    }
    else if (c.A.isIn() && !c.B.isIn() && c.C.isIn() && c.D.isIn()) //cas 11
    {
        distance = (T - c.A.d) / (c.B.d - c.A.d);
        P1.x = (1 - distance) * c.A.x + distance * c.B.x;
        P1.y = c.B.y;

        P2.x = c.B.x;
        distance = (T - c.C.d) / (c.B.d - c.C.d);
        P2.y = (1 - distance) * c.C.y + distance * c.B.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && c.B.isIn() && !c.C.isIn() && !c.D.isIn()) //cas 12
    {
        P1.x = c.A.x;
        distance = (T - c.A.d) / (c.D.d - c.A.d);
        P1.y = (1 - distance) * c.A.y + distance * c.D.y;

        P2.x = c.B.x;
        distance = (T - c.B.d) / (c.C.d - c.B.d);
        P2.y = (1 - distance) * c.B.y + distance * c.C.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && c.B.isIn() && !c.C.isIn() && c.D.isIn()) //cas 13
    {
        distance = (T - c.D.d) / (c.C.d - c.D.d);
        P1.x = (1 - distance) * c.D.x + distance * c.C.x;
        P1.y = c.D.y;

        P2.x = c.B.x;
        distance = (T - c.B.d) / (c.C.d - c.B.d);
        P2.y = (1 - distance) * c.B.y + distance * c.C.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    else if (c.A.isIn() && c.B.isIn() && c.C.isIn() && !c.D.isIn()) //cas 14
    {
        P1.x = c.A.x;
        distance = (T - c.A.d) / (c.D.d - c.A.d);
        P1.y = (1 - distance) * c.A.y + distance * c.D.y;

        distance = (T - c.C.d) / (c.D.d - c.C.d);
        P2.x = (1 - distance) * c.C.x + distance * c.D.x;
        P2.y = c.C.y;

        glBegin(GL_LINES);
        glVertex2f(P1.x, P1.y);
        glVertex2f(P2.x, P2.y);
        glEnd();
    }
    //cas 15 do nothing
}

void MarchingSquares(float Ux, float Uy)
{
    for (int i = 0; i < WIDTH_GRILLE; i++)
        for (int j = 0; j < HEIGHT_GRILLE; j++)
            Grid[i][j].set(Ux * i, Uy * j, ValueOfDensiteFunc(Grid[i][j]));

    for (int i = 0; i < WIDTH_GRILLE - 1; i++)
        for (int j = 0; j < HEIGHT_GRILLE - 1; j++)
        {
            carres[i][j].set(i, j);
            SetUpEdges(carres[i][j]);
        }
}

/**************************** fonction de display & reshape ****************************/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float Ux = viewport[2] / WIDTH_GRILLE;
    float Uy = viewport[3] / HEIGHT_GRILLE;

    glPointSize(5.0);

    MarchingSquares(Ux, Uy);

    DrawPoint();

    glColor3f(0.0, 1.0, 1.0);
    glPointSize(3);

    glutPostRedisplay();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**************************** fonctions souris ****************************/
void Mouse(int button, int state, int x, int y)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //deplacer des point
    if (button == GLUT_LEFT_BUTTON && addPoint == 0)
    {
        if (state == GLUT_DOWN)
        {
            GLuint *selectBuf = new GLuint[200];
            GLuint *ptr;
            GLint hits;

            glSelectBuffer(200, selectBuf);
            glRenderMode(GL_SELECT);

            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPickMatrix(x - viewport[0], -viewport[1] + viewport[3] - y, 5.0, 5.0, viewport);
            glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);

            DrawPoint();

            glPopMatrix();
            glFlush();

            hits = glRenderMode(GL_RENDER);
            if (hits)
            {
                ptr = (GLuint *)selectBuf;
                ptr += 3;
                mp = *ptr;
            }
        }

        if (state == GLUT_UP)
            mp = -1;

        reshape(viewport[2], viewport[3]);
        glutPostRedisplay();
    }

    //ajouter des point
    if (button == GLUT_LEFT_BUTTON && addPoint == 1 && state == GLUT_UP)
    {
        if (isSelecting)
            isSelecting = false;
        else
        {
            glColor3f(1.0, 1.0, 0.0);
            glPointSize(6.0);
            glPushName(1);

            P[N].x = x - viewport[0];
            P[N].y = -viewport[1] + viewport[3] - y;

            glLoadName(N);
            glBegin(GL_POINTS);
            glVertex2f(P[N].x, P[N].y);
            glEnd();

            N++;

            glutPostRedisplay();
            addPoint = 0;
            glutSetCursor(GLUT_CURSOR_UP_DOWN);
        }
    }
}

void Motion(int x, int y)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (addPoint == 0 && (mp != -1))
    {
        P[mp].x = x - viewport[0];
        P[mp].y = viewport[3] - y - viewport[1];
        glutPostRedisplay();
    }
}

/**************************** fonctions menu ****************************/
void menuFoncDensite(int item)
{
    switch (item)
    {
    case SUB_MENU_Melange:
        isSelecting = true;
        foncDensite = 1;
        break;
    case SUB_MENU_Union:
        isSelecting = true;
        foncDensite = 2;
        break;
    case SUB_MENU_Sub:
        isSelecting = true;
        foncDensite = 3;
        break;
    }
}

void menuFoncPotentiel(int item)
{
    switch (item)
    {
    case SUB_MENU_Murakami:
        isSelecting = true;
        foncPotentiel = 1;
        break;
    case SUB_MENU_Nishimura:
        isSelecting = true;
        foncPotentiel = 2;
        break;
    case SUB_MENU_Wyvil:
        foncPotentiel = 3;
        isSelecting = true;
        break;
    }
}

void mainMenu(int item)
{
    switch (item)
    {
    case MENU_AddPoint:
        isSelecting = true;
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
        addPoint = 1;
        break;
    case MENU_INCREASE_Ri:
        isSelecting = true;
        Ri = Ri + 5;
        break;
    case MENU_DECREASE_Ri:
        isSelecting = true;
        Ri = Ri + 5;
        break;
    case MENU_INCREASE_T:
        isSelecting = true;
        if (T < 0.9)
            T = T + 0.1;
        break;
    case MENU_DECREASE_T:
        isSelecting = true;
        if (T > 0.1)
            T = T - 0.1;
        break;
    case MENU_EXIT:
        isSelecting = true;
        exit(0);
        break;
    }
}

void createMenu()
{
    int menuPotentiel = glutCreateMenu(menuFoncPotentiel);
    glutAddMenuEntry("Murakami", SUB_MENU_Murakami);
    glutAddMenuEntry("Nishimura", SUB_MENU_Nishimura);
    glutAddMenuEntry("Wyvill", SUB_MENU_Wyvil);

    int menuDensite = glutCreateMenu(menuFoncDensite);
    glutAddMenuEntry("Merge", SUB_MENU_Melange);
    glutAddMenuEntry("Union", SUB_MENU_Union);
    glutAddMenuEntry("Intersection", SUB_MENU_Sub);

    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Add point ", MENU_AddPoint);
    glutAddSubMenu("Potential functions ", menuPotentiel);
    glutAddSubMenu("Density functions ", menuDensite);
    glutAddMenuEntry("Increase Ri (+5) ", MENU_INCREASE_Ri);
    glutAddMenuEntry("Decrease Ri (-5) ", MENU_DECREASE_Ri);
    glutAddMenuEntry("Increase T (+0.1) ", MENU_INCREASE_T);
    glutAddMenuEntry("Decrease T (-0.1) ", MENU_DECREASE_T);
    glutAddMenuEntry("Exit ", MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}