/***********************************************************************/
/*                                                                     */
/*             Michel GRAVE (michel.grave@univ-lr.fr)                  */
/*                                                                     */
/*        IUP de Genie Informatique - Universite de La Rochelle        */
/*                                                                     */
/*            TP : Visualisation de champs de vecteurs                 */
/*                                                                     */
/*                  version 3 : OpenGL et glut                         */
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


#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "calcul.h"
#include "maillage.h"

void    charger_objet(char *nom) {
    FILE    *fobject = NULL;
    int     nbLines;
    int     nbElem;
    int     nxInc;
    int     nyInc;

    char    *line = NULL;
    size_t	len = 0;
    ssize_t	read;
    char    **splitLine = NULL;

    if(!(fobject = fopen(nom, "r"))) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier-objet\n");
        exit(EXIT_FAILURE);
    }

    nbLines = 0;
    nxInc = 0;
    nyInc = 0;

    xmin, xmax, ymin, ymax = 0;
    umin, umax, vmin, vmax = 0;
    while ((read = getline(&line, &len, fobject)) != -1) {
        line = strtok(line, "\t\r\n ");
        char *splitLine[5] = {NULL};

        nbElem = 0;
        while (line) {
            splitLine[nbElem++] = line;

            if (nbElem > 4) {
                fprintf(stderr, "Erreur, le fichier comporte plus d'argumets que demandé.\n");
                exit(EXIT_FAILURE);
            }
            line = strtok(NULL, "\t\r\n ");
        }

        if (nbLines == 0) {
            nx = strtol(splitLine[0], NULL, 10);
            ny = strtol(splitLine[1], NULL, 10);
            Angle = strtod(splitLine[2], NULL);

        } else {
            x[nxInc][nyInc] = strtod(splitLine[0], NULL);
            y[nxInc][nyInc] = strtod(splitLine[1], NULL);
            u[nxInc][nyInc] = strtod(splitLine[2], NULL);
            v[nxInc][nyInc] = strtod(splitLine[3], NULL);

            xmax = MAX(x[nxInc][nyInc], xmax);
            ymax = MAX(y[nxInc][nyInc], ymax);
            umax = MAX(u[nxInc][nyInc], umax);
            vmax = MAX(v[nxInc][nyInc], vmax);

            xmin = MIN(x[nxInc][nyInc], xmin);
            ymin = MIN(y[nxInc][nyInc], ymin);
            umin = MIN(u[nxInc][nyInc], umin);
            vmin = MIN(v[nxInc][nyInc], vmin);

            nyInc += 1;
            if (ny == nyInc) {
                nxInc += 1;
                nyInc = 0;
            }
        }
        nbLines += 1;
    }

    printf("nombre de lignes -> %d (%d x %d + 1)\n", nbLines, nx, ny);
    if ((nx * ny) + 1 != nbLines) {
        printf("\tFichier inconsistent /!\\\n");
    }
    printf("xmax: %e\t| ymax: %e\t| umax: %e\t| vmax: %e\n", xmax, ymax, umax, vmax);
    printf("xmin: %e\t| ymin: %e\t| umin: %e\t| vmin: %e\n", xmin, ymin, umin, vmin);

    fclose(fobject);
} /* charger_objet */

void afficher_profil(float couleur[3]) {
} /* afficher_profil */


void afficher_maillage(float couleur[3]) {
} /* afficher_maillage */

void afficher_vitesses(float couleur[3]) {
} /* afficher_vitesses */

void afficher_candidates(float couleur[3],int nc, int ci[], int cj[]) {
} /* afficher_candidates */

void afficher_pointage(float couleur[3],double xp, double yp, int ic, int jc) {
    glColor3f(couleur[0], couleur[1], couleur[2]);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(xp,yp,0.0f);
    glEnd();
} /* afficher_pointage */

void afficher_ligne(float couleur[3], double x[], double y[], int n) {
    glColor3f(couleur[0], couleur[1], couleur[2]);

    for(unsigned int index = 0; 0<n-1; index++) {
        glBegin(GL_LINES);
        glVertex2f(x[index], x[index + 1]);
        glVertex2f(y[index], y[index + 1]);
        glEnd();
    }
} /* afficher_ligne */

void cellules_candidates(int *nc, int ci[], int cj[]) {
} /* cellules_candidates */

void calculer_ligne(double step, double pe, double qe, int ci, int cj,
                    double sx[], double sy[], int *nns, int maxp) {
} /* calculer_ligne */
