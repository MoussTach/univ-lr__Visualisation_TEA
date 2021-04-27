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

//DECOUTY Hugo JUBERT Baptiste

//sudo apt install libglu1-mesa-dev freeglut3-dev
//gcc -g -o tp *.c -lglut -lGL -lGLU -lm

#include <stdio.h>
#include <stdbool.h>  
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "maillage.h"
#include "calcul.h"

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int *splitInDoubleArray(char *line, char *delimiter, 	     */
/*			int *_n_ints)                                */
/*                                                                   */
/*-------------------------------------------------------------------*/

double *splitInDoubleArray(char *line, char *delimiter, int *_n_ints){
    double *_dynamic = (double*)malloc(sizeof(double));
    char *currentSplited = strtok(line, delimiter);
    double val;
	while(currentSplited != NULL){
	        val = atof(currentSplited);
	        (*_n_ints)++;
	
        	_dynamic = (double *)realloc(_dynamic, sizeof(double)*(*_n_ints));
	        _dynamic[(*_n_ints)-1] = val;

		//printf("'%s', f:%lf, exp:%e\n", currentSplited, val, val);
		currentSplited = strtok(NULL, delimiter);
	}
	return _dynamic;
}

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int *splitInIntArray(char *line, char *delimiter, 		     */
/*			int *_n_ints)                                */
/*                                                                   */
/* Permet de charger les valeurs du fichier aile dans les tableaux   */
/*-------------------------------------------------------------------*/

int *splitInIntArray(char *line, char *delimiter, int *_n_ints){
    int *_dynamic = (int*)malloc(sizeof(int));
    char *currentSplited = strtok(line, delimiter);
    int val;
	while(currentSplited != NULL){
        val = atoi(currentSplited);
        (*_n_ints)++;
        _dynamic = (int *)realloc(_dynamic, sizeof(int)*(*_n_ints));
        _dynamic[(*_n_ints)-1] = val;
		//printf("'%s', i:%d\n", currentSplited, val);
		currentSplited = strtok(NULL, delimiter);
	}
	return _dynamic;
}

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void charger_objet(char *nom)                                     */
/*                                                                   */
/*-------------------------------------------------------------------*/
void charger_objet(char *nom)
{
    FILE *fobject = fopen(nom, "r");
    if(!fobject){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier-objet\n");
        exit(EXIT_FAILURE);
    }
    char currentChar = 0, *currentLine = (char*)malloc(sizeof(char)*BUFSIZ);
    int currCharIndex = 0, nbLignes = 0, splitted_qtty = 0;
    int i = 0, j = 0;

    while(currentChar!=EOF){
        while(currentChar!='\n'){
            currentChar = fgetc(fobject);
            if(currentChar == EOF)
                break;
            if(currentChar == '\t')
                currentLine[currCharIndex] = ' ';
            if(currentChar == '\r')
                currentLine[currCharIndex] = '\0';
            else
                currentLine[currCharIndex] = currentChar;
            currCharIndex++;
            //printf("current: %d, %c\n", currentChar, currentChar);
        }
        if(currentLine[currCharIndex-1] == '\n')
            currentLine[currCharIndex-1] = '\0';
        if(currCharIndex != 0){
            if(currCharIndex == 1)
                printf("LINE:%s\n", currentLine);
            splitted_qtty = 0;
            if(nbLignes==0){
                printf("LINE:%s\n", currentLine);
                int *first = splitInIntArray(currentLine, " ", &splitted_qtty);
                nx = (int)first[0]/1;
                ny = (int)first[1]/1;
                Angle = (double)first[2]/1.;
		printf("first2 : %d - angle %lf\n", first[2], Angle);
                /*for(int k=0 ; k<splitted_qtty ; k++){
                    printf("%d\t", first[k]);
                }*/
                //printf("\n");
            } else{
//	        printf("LINE:%s\n", currentLine);
                double *object = splitInDoubleArray(currentLine, " ", &splitted_qtty);
                /*for(int k=0 ; k<splitted_qtty ; k++){
                    printf("%lf|%e\t", object [k], object[k]);
                }*/
                
                //printf("i:%d, j:%d\n", i, j);
                x[i][j] = object[0];
                y[i][j] = object[1];
                u[i][j] = object[2];
                v[i][j] = object[3];
		if(i == 0 && j == 0){
			xmin = x[i][j]; xmax = xmin;
			ymin = y[i][j]; ymax = ymin;
			umin = u[i][j]; umax = umin;
			vmin = v[i][j]; vmax = vmin;
		}

		xmin = MIN(xmin, x[i][j]);
		ymin = MIN(ymin, y[i][j]);
		umin = MIN(umin, u[i][j]);
		vmin = MIN(vmin, v[i][j]);

		xmax = MAX(xmax, x[i][j]);
		ymax = MAX(ymax, y[i][j]);
		umax = MAX(umax, u[i][j]);
		vmax = MAX(vmax, v[i][j]);


		if(j+1<ny){
                    j++;
                }
                else{
                    i++;
                    j=0;
                }
/*		printf("result:\n");
                for(int i=0 ; i<splitted_qtty ; i++){
                    printf("%lf|%e\t", object[i], object[i]);
                }
	            printf("\n");
*/
		
                
            }
            currentLine = (char *)realloc(currentLine,sizeof(char)*BUFSIZ);
            currCharIndex = 0;
            if(currentChar!=EOF)
                currentChar = 0;
            nbLignes++;
        }
        else
            break;
    }
    free(currentLine);
    printf("Total de lignes dans le fichier:%d\n", nbLignes);
    fclose(fobject);
    printf("xmin: %lf|%e, xmax: %lf|%e, ymin: %lf|%e, ymax:%lf|%e\n", xmin,xmin, xmax,xmax, ymin,ymin, ymax,ymax);
    printf("umin: %lf|%e, umax: %lf|%e, vmin: %lf|%e, vmax:%lf|%e\n", umin,umin, umax,umax, vmin,vmin, vmax,vmax);
} /* charger_objet */


/*-------------------------------------------------------------------*/
/*                                                                   */
/* void afficher_profil(float couleur[3])                            */
/*                                                                   */
/* Permet d'afficher le profil de l'aile dans une couleur grise      */
/*-------------------------------------------------------------------*/
void afficher_profil(float couleur[3])
{
    glColor3f(couleur[0], couleur[1], couleur[2]);
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < nx-1; i++) {
        glVertex2f(x[i][0], y[i][0]);
    }
    glEnd();

} /* afficher_profil */

/*-------------------------------------------------------------------*/
/*                                                                   */
/*void afficher_maillage(float couleur[3])                           */
/*                                                                   */
/* Permet d'afficher le maillage autour de l'aile dans une couleur   */
/*           grise                                                   */
/*-------------------------------------------------------------------*/
void afficher_maillage(float couleur[3])
{
    glColor3f(couleur[0], couleur[1], couleur[2]);

    for(int i = 0; i < nx-1; i++) {
        for(int j = 0; j < ny; j++) {
            glBegin(GL_LINES);
            glVertex2f(x[i][j], y[i][j]);
            glVertex2f(x[i+1][j], y[i+1][j]);
            glEnd();
        }
    }


    for(int i = 0; i < nx; i++) {
        for(int j = 0; j < ny-1; j++) {
            glBegin(GL_LINES);
            glVertex2f(x[i][j], y[i][j]);
            glVertex2f(x[i][j+1], y[i][j+1]);
            glEnd();
        }
    }
} /* afficher_maillage */

/*-------------------------------------------------------------------*/
/*                                                                   */
/*void afficher_vitesses(float couleur[3])                           */
/*                                                                   */
/* Permet d'afficher les vecteurs vitesse sur le maillage dans une   */
/*    couleur rouge                                                  */
/*-------------------------------------------------------------------*/
void afficher_vitesses(float couleur[3])
{
    glColor3f(couleur[0], couleur[1], couleur[2]);

    double d=0, t=0, vi = 0;

    for(int i = 0; i < nx; i++) {
        t = i;
        for(int j = 0; j < ny-1; j++) {
            // Pythagore pour trouver la distance entre les deux vecteurs
            d = sqrt(pow(u[i][j], 2) + pow(v[i][j], 2));
            // Calcul de la vitesse vitesse = distance/temps
            vi = d/t;

            glBegin(GL_LINES);
            glVertex2f(x[i][j], y[i][j]);
            glVertex2f(x[i][j] + u[i][j] * vi, y[i][j] + v[i][j] * vi);
            glEnd();
        }
    }
} /* afficher_vitesses */

/*---------------------------------------------------------------------*/
/*                                                                     */
/*void afficher_candidates(float couleur[3],int nc, int ci[], int cj[])*/
/*                                                                     */
/* Permet d'afficher les cellules candidates (changement de vitesse    */
/* sur 1 des 4 côtés de la cellule) dans une couleur bleu              */
/*                                                                     */
/*---------------------------------------------------------------------*/
void afficher_candidates(float couleur[3],int nc, int ci[], int cj[])
{
    glColor3f(couleur[0], couleur[1], couleur[2]);
    for(int i=0; i<nc ; i++){
        glBegin(GL_POLYGON);
	glVertex2f(x[ci[i]][cj[i]], y[ci[i]][cj[i]]);
	glVertex2f(x[ci[i]+1][cj[i]], y[ci[i]+1][cj[i]]);
	glVertex2f(x[ci[i]+1][cj[i]+1], y[ci[i]+1][cj[i]+1]);
	glVertex2f(x[ci[i]][cj[i]+1], y[ci[i]][cj[i]+1]);
	glEnd();
    }
    
} /* afficher_candidates */

/*-------------------------------------------------------------------*/
/*                                                                   */
/*void afficher_pointage(float couleur[3],double xp,                 */
/*                       double yp, int ic, int jc)                  */
/*                                                                   */
/* Permet d'afficher en un point vert, le clic de l'utilisateur      */
/*                                                                   */
/*-------------------------------------------------------------------*/ 
void afficher_pointage(float couleur[3],double xp, double yp, int ic, int jc)
{
    glColor3f(couleur[0], couleur[1], couleur[2]);
    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(couleur[0], couleur[1], couleur[2]);
    glVertex3f(xp,yp,0);
    glEnd();
} /* afficher_pointage */


/*--------------------------------------------------------------------*/
/*                                                                    */
/*void afficher_ligne(float couleur[3], double x[], double y[], int n)*/
/*                                                                    */
/* Permet d'afficher les streamlines en bleu ou rouge                 */
/*                                                                    */
/*--------------------------------------------------------------------*/ 
void afficher_ligne(float couleur[3], double x[], double y[], int n)
{
    glColor3f(couleur[0], couleur[1], couleur[2]);

    for(int i = 0; i < n-1; i++) {
        glBegin(GL_LINES);
        glVertex2f(x[i], y[i]);
        glVertex2f(x[i+1], y[i+1]);
        glEnd();
    }
} /* afficher_ligne */

/*-------------------------------------------------------------------*/
/*                                                                   */
/*void cellules_candidates(int *nc, int ci[], int cj[])              */
/*                                                                   */
/* Permet de déterminer les cellules candidates pour par la suite les*/
/*        afficher                                                   */
/*-------------------------------------------------------------------*/ 
void cellules_candidates(int *nc, int ci[], int cj[])
{
    int cpt=0;
    for(int i = 1; i < nx-1; i++) {
        for(int j = 0; j < ny-1; j++) {  
	    bool candidate = true;         
	    if(((u[i][j]<=0 && u[i+1][j]<=0 && u[i+1][j+1]<=0 && u[i][j+1]<=0)||
		(u[i][j]>0 && u[i+1][j]>0 && u[i+1][j+1]>0 && u[i][j+1]>0))||
		((v[i][j]<=0 && v[i+1][j]<=0 && v[i+1][j+1]<=0 && v[i][j+1]<=0)||
		(v[i][j]>0 && v[i+1][j]>0 && v[i+1][j+1]>0 && v[i][j+1]>0))){
		
		candidate = false;
	    }

	    if(candidate==true){
		ci[cpt]=i;
		cj[cpt]=j;
		cpt=cpt+1;
	    }

        }
    }
    *nc=cpt+1;
    
} /* cellules_candidates */


/*-------------------------------------------------------------------*/
/*                                                                   */
/*void calculer_ligne(double step, double pe, double qe, int ci,     */
/*              int cj, double sx[], double sy[], int *nns, int maxp)*/
/*                                                                   */
/* Permet de déterminer les streamlines pour par la suite les        */
/*        afficher                                                   */
/*-------------------------------------------------------------------*/ 
void calculer_ligne(double step, double pe, double qe, int ci, int cj,
                    double sx[], double sy[], int *nns, int maxp)
{

     double ps, qs, xIntermediaire, yIntermediaire;
     int flag=1;
     	printf("ci:%i, cj:%i\n",ci, cj);
	     while((*nns < maxp) && flag){
		//printf("pe:%f, qe:%f\n",pe,qe);
		flag = tracer_cellule(ci, cj, step, pe, qe, &ps, &qs, sx, sy, nns, maxp);
		switch(flag){
			case 1 : {
					cj--;
					break;
				}
			case 2:	{
					cj++;
					break;
				}
			case 3: {
					ci--;
					break;
				}
			case 4: {
					ci++;
					break;
				}
			default:
				break;
		}

		if(ci>nx-1 || cj>ny-1){
			flag=0;
		}else{
			xIntermediaire = (x[ci+1][cj]-x[ci][cj])/2+x[ci][cj];
			yIntermediaire = (y[ci][cj+1]-y[ci][cj])/2+y[ci][cj];
			//printf("ci:%i, cj:%i; x:%f, y:%f, pe: %f, qe:%f\n",ci, cj,xIntermediaire,yIntermediaire,pe,qe);
			xy2pq(ci,cj,xIntermediaire,yIntermediaire,&pe,&qe);
			//printf("ci:%i, cj:%i; x:%f, y:%f, pe: %f, qe:%f\n",ci, cj,xIntermediaire,yIntermediaire,pe,qe);
		}
	     }  
     
} /* calculer_ligne */
