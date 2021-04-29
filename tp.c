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

//sudo apt install libglu1-mesa-dev freeglut3-dev
//gcc -g -o tp *.c -lglut -lGL -lGLU -lm

#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

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
	glColor3f(couleur[0], couleur[1], couleur[2]);
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < nx; i++) {
        glVertex2f(x[i][0], y[i][0]);
    }
    glEnd();
} /* afficher_profil */


void afficher_maillage(float couleur[3]) {

	glColor3f(couleur[0], couleur[1], couleur[2]);

	glBegin(GL_LINES);
	for(int i = 0; i < nx; i++) {
		for(int j = 0; j < ny; j++ ) {
			
			if(i < nx-1){
				glVertex2f(x[i][j], y[i][j]);
				glVertex2f(x[i+1][j], y[i+1][j]);	
			}

			if(j < ny-1){
			glVertex2f(x[i][j], y[i][j]);	
				glVertex2f(x[i][j+1], y[i][j+1]);
			}

		}
	}
	glEnd();	
} 

void afficher_vitesses(float couleur[3]) {

	glColor3f(couleur[0], couleur[1], couleur[2]);
	
	//Calcul de la vitesse v = d/t (t sera sur l'axe des abscisses donc 
	//il serait plus correct de dire ici v = d/ligne)
	double d, vitesse = 0;

	glBegin(GL_LINES);
	for(int ligne = 0; ligne < nx; ligne++){
		for(int colonne = 0; colonne < ny; colonne++){
			
			//Trouver la distance entre deux vecteurs.
			//d = (produit scalaire) - (http://labomath.free.fr/faidherbe/premS/prod-scal/prodscal.pdf)
			d = sqrt(pow(u[ligne][colonne],2) + pow(v[ligne][colonne],2));

			vitesse = d/ligne;

			glVertex2f(x[ligne][colonne], y[ligne][colonne]);
			glVertex2f((x[ligne][colonne] + (u[ligne][colonne]*vitesse)), (y[ligne][colonne] + v[ligne][colonne]*(vitesse)));

		}

	}
	glEnd();


	
	

} /* afficher_vitesses */

void afficher_candidates(float couleur[3],int nc, int ci[], int cj[]) {
	glColor3f(couleur[0], couleur[1], couleur[2]);

	glBegin(GL_POLYGON);
	for(int index = 0; index < nc; index++){
		int candidate_x = ci[index]; //voir.c
		int candidate_y = cj[index]; //voir.c
		
		glVertex2f(x[candidate_x][candidate_y], y[candidate_x][candidate_y]);
		glVertex2f(x[candidate_x+1][candidate_y], y[candidate_x+1][candidate_y]);
		glVertex2f(x[candidate_x+1][candidate_y+1], y[candidate_x+1][candidate_y+1]);
		glVertex2f(x[candidate_x][candidate_y+1], y[candidate_x][candidate_y+1]);
	}
	glEnd();
} /* afficher_candidates */

void afficher_pointage(float couleur[3],double xp, double yp, int ic, int jc) {
    glColor3f(couleur[0], couleur[1], couleur[2]);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(xp,yp,0.0f);
    glEnd();
} /* afficher_pointage */

void afficher_ligne(float couleur[3], double x[], double y[], int n) {
    glColor3f(couleur[0], couleur[1], couleur[2]);

	glBegin(GL_LINES);
    for(int index = 0; index < n-1; index++) {
        
        glVertex2f(x[index], y[index]);
        glVertex2f(x[index+1], y[index+1]);
        
    }
	glEnd();
	
} /* afficher_ligne */

void cellules_candidates(int *nc, int ci[], int cj[]) {
	int candidateCellsCounter = 0;
	for(int i = 0; i < nx-1; i++) {

		for(int j = 0; j < ny-1; j++) {

			if(u[i][j]<=0 && u[i+1][j]<=0 && u[i+1][j+1]<=0 && u[i][j+1]<=0){
				continue;
			} else if(u[i][j]>0 && u[i+1][j]>0 && u[i+1][j+1]>0 && u[i][j+1]>0){
				continue;
			} else if(v[i][j]<=0 && v[i+1][j]<=0 && v[i+1][j+1]<=0 && v[i][j+1]<=0){
				continue;
			} else if(v[i][j]>0 && v[i+1][j]>0 && v[i+1][j+1]>0 && v[i][j+1]>0){
				continue;
			} else {
				ci[candidateCellsCounter] = i;
				cj[candidateCellsCounter] = j;
				candidateCellsCounter += 1;
			}
		}
	}
	*nc = candidateCellsCounter;

} /* cellules_candidates */

void calculer_ligne(double step, double pe, double qe, int ci, int cj,
                    double sx[], double sy[], int *nns, int maxp) {
} /* calculer_ligne */


