/***********************************************************************/
/*                                                                     */
/*             Michel GRAVE (michel.grave@univ-lr.fr)                  */
/*                                                                     */
/*        IUP de Genie Informatique - Universite de La Rochelle        */
/*                                                                     */
/*            TP : Visualisation de champs de vecteurs                 */
/*                                                                     */
/*                   version 3 : OpenGL et glut                        */
/*                                                                     */
/*                         Novembre 2001                               */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*                          AVERTISSEMENT                              */
/*                                                                     */
/* Ce logiciel a ete realise a des fins d'enseignement, et un certain  */
/* nombre de fonctions ont ete soit simplifiees soit volontairement    */
/* modifiees. De plus, il n'a fait l'objet d'aucune certification ou   */
/* validation formelle. Il n'est donc en aucune maniere destine a etre */
/* exploite, partiellement ou integralement a d'autres fins que des    */
/* travaux pratiques.                                                  */
/*                                                                     */
/***********************************************************************/

// gcc -g -o tp tp.c voir.c calcul.c -lglut -lGL -lGLU -lm

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define MAIN
#include "maillage.h"
#include "calcul.h"


/* variable caracterisant la fenetre d'affichage */
    int vx1, vx2, vy1, vy2;
    double xcentre, ycentre;

/* Indicateurs pour controler l'affichage ou non de certains elements */
    int f_maillage = 0 , f_vitesses = 0 , f_cellules = 0 ;

/* Nombre et Indices des cellules candidates trouvees */
    int nc = 0;
    int ci[1000], cj[1000];

/* Coordonnees du dernier point designe */
    double xpnt=0.0, ypnt=0.0;

/* Indice de la cellule de travail courante */
    int cci = -1, ccj = -1;

/* Structures de donnees decrivant les lignes de courant */
    #define NMXPTS 4000
    double cx1[NMXPTS], cy1[NMXPTS];
    double cx2[NMXPTS], cy2[NMXPTS];
    double cx3[NMXPTS], cy3[NMXPTS];
    double cx4[NMXPTS], cy4[NMXPTS];
    int n1 = 0, n2 = 0, n3 = 0, n4 = 0;

/* Ensemble de couleurs predefinies */
    float bleu[3]   = {0.0,0.0,1.0};
    float fond [3]  = {0.9,0.7,0.5};
    float gris[3]   = {0.4,0.4,0.4};
    float noir[3]   = {0.0,0.0,0.0};
    float rouge[3]  = {1.0,0.0,0.0};
    float vert[3]   = {0.0,0.6,0.0};

/*----------- Prototypes des fonctions de visualisation -------------*/

void charger_objet(char *nom);

void afficher_profil(float couleur[3]);

void afficher_maillage(float couleur[3]);

void afficher_vitesses(float couleur[3]);

void afficher_candidates(float couleur[3],int nc,int ci[],int cj[]);

void afficher_pointage(float couleur[3],double xp,double yp,int ic,int jc);

void afficher_ligne(float couleur[3],double x[],double y[],int n);

void cellules_candidates(int *nc,int ci[],int cj[]);

void calculer_ligne(double step,double pe,double qe,int ci,int cj,
                    double sx[],double sy[],int *nns,int maxp);

/*-------------------------------------------------------------------*/


/*-------------------------------------------------------------------*/
/*                                                                   */
/* void initialiser_graphique(char *titre)                           */
/*                                                                   */
/*-------------------------------------------------------------------*/
void initialiser_graphique(char *titre)
{
   vx1 =   0;
   vx2 = 500;
   vy1 =   0;
   vy2 = 500;
   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowPosition (vx1, vy1);
   glutInitWindowSize (vx2, vy2); 
   glutCreateWindow (titre);

} /* initialiser_graphique */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void v2d_reshape(int w, int h)                                    */
/*                                                                   */
/*-------------------------------------------------------------------*/
void v2d_reshape(int w, int h)
{
   vx1 = 0;
   vx2 = w;
   vy1 = 0;
   vy2 = h;
   xcentre = (float)(vx2+vx1)/2.0;
   ycentre = (float)(vy2+vy1)/2.0;
   glViewport(0, 0, w, h);
} /* v2d_reshape */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void afficher_objet(void)                                         */
/*                                                                   */
/*-------------------------------------------------------------------*/

void afficher_objet(void)
{
  glClearColor(fond[0],fond[1],fond[2],0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0,-1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(Angle, 0.0, 0.0, 1.0);

  afficher_profil(gris);

  if (f_maillage)
    afficher_maillage(gris);

  if (f_vitesses)
    afficher_vitesses(rouge); 
  
  if (f_cellules)
    afficher_candidates(bleu,nc,ci,cj);
  
  afficher_pointage(vert,xpnt,ypnt,cci,ccj);
  
  afficher_ligne(bleu ,cx1,cy1,n1);
  afficher_ligne(rouge,cx2,cy2,n2);
  afficher_ligne(bleu ,cx3,cy3,n3);
  afficher_ligne(rouge,cx4,cy4,n4);

  glFlush();

  glutSwapBuffers();

} /* afficher_objet */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void pointage(int button, int state, int mx, int my)              */
/*                                                                   */
/*-------------------------------------------------------------------*/
void pointage(int button, int state, int mx, int my)
{
  GLdouble x,y; 
  double c,s;
  double p, q;
    if (state == GLUT_DOWN) {
    x = mx - xcentre;
    y = (vy2 - my) - ycentre;
    x = x / (vx2-vx1);
    y = y / (vy2-vy1);
    x = x * 2;
    y = y * 2;
    c =   cosf((3.1415927 *Angle)/180.0);
    s = - sinf((3.1415927 *Angle)/180.0);
    xpnt =   x*c - y*s;
    ypnt =   x*s + y*c;
    localiser(xpnt,ypnt,&cci,&ccj);
      
    xy2pq(cci,ccj,xpnt,ypnt,&p,&q);
    n1 = n2 = n3 = n4 = 0;
    
    calculer_ligne( 0.0005,p,q,cci,ccj,cx1,cy1,&n1,NMXPTS-1);
    calculer_ligne( 0.0050,p,q,cci,ccj,cx2,cy2,&n2,NMXPTS-1);
    calculer_ligne(-0.0005,p,q,cci,ccj,cx3,cy3,&n3,NMXPTS-1);
    calculer_ligne(-0.0050,p,q,cci,ccj,cx4,cy4,&n4,NMXPTS-1);

    glutPostRedisplay();
    };
} /* pointage */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void clavier(unsigned char key, int x, int y)                     */
/*                                                                   */
/*-------------------------------------------------------------------*/
void clavier(unsigned char touche, int x, int y)
{
   switch (touche) {
      case 27:
               exit(0);
               break;
      case 'm':
               f_maillage = 1 - f_maillage;
               break;
      case 'v':
               f_vitesses = 1 - f_vitesses;
               break;
      case 'c':
               f_cellules = 1 - f_cellules;
               if (f_cellules) cellules_candidates(&nc,ci,cj);
               break;
   }
    glutPostRedisplay();

}

/*-------------------------------------------------------------------*/
/*                                                                   */
/* main(argc,argv)                                                   */
/*                                                                   */
/*-------------------------------------------------------------------*/
int main(int argc,char *argv[])
{ 
   glutInit(&argc, argv);
   if (argc > 1)
     charger_objet(argv[1]);
   else
     charger_objet("aile");
   initialiser_graphique("VUE-2D");
   glutDisplayFunc(afficher_objet); 
   glutReshapeFunc(v2d_reshape);
 
   glutKeyboardFunc(clavier);
   glutMouseFunc(pointage);
   glutMainLoop();
   return 0;
  
} /* main */
