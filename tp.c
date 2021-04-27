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

#include "maillage.h"
#include "calcul.h"

typedef struct  infos {
    int         nx;
    int         ny;
    double      angle;
}               infos;

void    charger_objet(char *nom) {
    FILE        *fobject = NULL;
    infos       infos;
    float       **tabValues;
    int         nbLines;
    int         posElem;

    char    *line = NULL;
    size_t	len = 0;
    ssize_t	read;
    char    **splitLine = NULL;

    if(!(fobject = fopen(nom, "r"))) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier-objet\n");
        exit(EXIT_FAILURE);
    }

    nbLines = 0;
    while ((read = getline(&line, &len, fobject)) != -1) {
        line = strtok(line, "\t\r\n ");
        char *splitLine[5] = {NULL};

        posElem = 0;
        while (line) {
            splitLine[posElem++] = line;

            if (posElem > 4) {
                fprintf(stderr, "Erreur, le fichier comporte plus d'argumets que demandé.\n");
                exit(EXIT_FAILURE);
            }
            line = strtok(NULL, "\t\r\n ");
        }

        if (nbLines == 0) {
            infos.nx = strtol(splitLine[0], NULL, 10);
            infos.ny = strtol(splitLine[1], NULL, 10);
            infos.angle = strtod(splitLine[2], NULL);

            if (!(tabValues = malloc(sizeof(float *) * ((infos.nx * infos.ny)) + 1))) {
                fprintf(stderr, "Erreur, malloc échoué.\n");
                exit(EXIT_FAILURE);
            }

            posElem = 0;
            while (posElem < ((infos.nx * infos.ny)) + 1) {
                if (!(tabValues[posElem++] = malloc(sizeof(float) * 5))) {
                    fprintf(stderr, "Erreur, malloc échoué.\n");
                    exit(EXIT_FAILURE);
                }
            }

        } else {
            posElem = 0;
            while (splitLine[posElem]) {
                tabValues[nbLines - 1][posElem] = atof(splitLine[posElem]);
                printf("%f\n", tabValues[nbLines - 1][posElem]);
                posElem++;
            }
        }

        printf("\n");
        nbLines += 1;
    }

} /* charger_objet */

void afficher_profil(float couleur[3])
{
} /* afficher_profil */


void afficher_maillage(float couleur[3])
{
} /* afficher_maillage */

void afficher_vitesses(float couleur[3])
{
} /* afficher_vitesses */

void afficher_candidates(float couleur[3],int nc, int ci[], int cj[])
{
} /* afficher_candidates */

void afficher_pointage(float couleur[3],double xp, double yp, int ic, int jc)
{
} /* afficher_pointage */

void afficher_ligne(float couleur[3], double x[], double y[], int n)
{
} /* afficher_ligne */

void cellules_candidates(int *nc, int ci[], int cj[])
{
} /* cellules_candidates */

void calculer_ligne(double step, double pe, double qe, int ci, int cj,
                    double sx[], double sy[], int *nns, int maxp)
{
} /* calculer_ligne */
