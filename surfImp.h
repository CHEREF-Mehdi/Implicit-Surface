#include <GL/glut.h>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace std;

#define WIDTH_GRILLE 200  //largeur du grille
#define HEIGHT_GRILLE 150 //hauteur du grille

const int NMAX = 100;
float T = 0.7, Ri = 100;

struct Point
{
    float x, y, d;
    bool isIn()
    {
        return d < T;
    }
    void set(float xx, float yy, float dd)
    {
        x = xx;
        y = yy;
        d = dd;
    }
};

Point P[NMAX];
Point Grid[WIDTH_GRILLE][HEIGHT_GRILLE];

struct Square
{
    Point A, B, C, D;
    void set(int i, int j)
    {
        A = Grid[i][j];
        B = Grid[i + 1][j];
        C = Grid[i + 1][j + 1];
        D = Grid[i][j + 1];
    }
};

Square carres[WIDTH_GRILLE - 1][HEIGHT_GRILLE - 1];

enum MenuEntry
{
    MENU_Default = 0,
    MENU_AddPoint = 1,
    MENU_INCREASE_Ri = 2,
    MENU_DECREASE_Ri = 3,
    MENU_INCREASE_T = 4,
    MENU_DECREASE_T = 5,
    MENU_EXIT = 6
};

enum SubMenuPotentielEntry
{
    SUB_MENU_Murakami = 1,
    SUB_MENU_Nishimura = 2,
    SUB_MENU_Wyvil = 3
};

enum SubMenuDensiteEntry
{
    SUB_MENU_Melange = 1,
    SUB_MENU_Union = 2,
    SUB_MENU_Sub = 3
};

/**************************** disatance ****************************/
//calcule la distance euclidienne entre deux points
float distance(Point P1, Point P2);

/**************************** afficher les points ****************************/
void drawPoint();

/**************************** fonctions de potentiel ****************************/
float Murakami(float r);
float Nishimura(float r);
float Wyvill(float r);
float valueOfPotentielFunc(Point p1, Point p2);

/**************************** fonctions de densité ****************************/
float Melange(Point point);
float Union(Point point);
float Sub(Point point);
float ValueOfDensiteFunc(Point point);

/**************************** Polygonalisation ****************************/
void SetUpEdges(Square c);
void MarchingSquares(float Ux, float Uy);

/**************************** fonction de display & reshape ****************************/
void display();
void reshape(int width, int height);

/**************************** fonctions souris ****************************/
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

/**************************** fonctions menu ****************************/
void menuFoncDensite(int item);   //sous menu fonction de densite
void menuFoncPotentiel(int item); //sous menu fonction de potentiel
void mainMenu(int item);          //menu principale
void createMenu();                // crée tout le menu