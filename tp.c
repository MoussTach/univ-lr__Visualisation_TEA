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

#include "maillage.h"
#include "calcul.h"


void    charger_objet(char *nom) {
    FILE        *fobject = NULL;
    int         **tabValues;
    long long   **tabValues;
    int         nbLines;

    char    *line = NULL;
    char    *lineptr = NULL;
    int 	index = 0;
    size_t	len = 0;
    ssize_t	read;

    if(!(fobject = fopen(nom, "r"))) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier-objet\n");
        exit(EXIT_FAILURE);
    }

    nbLines = 0;
    while ((read = getline(&line, &len, fobject)) != -1) {
        if (nbLines == 0) {

        }
        printf("%s\n", line);
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
