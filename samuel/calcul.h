/***********************************************************************/
/*                                                                     */
/*             Michel GRAVE (michel.grave@univ-lr.fr)                  */
/*                                                                     */
/*        IUP de Genie Informatique - Universite de La Rochelle        */
/*                                                                     */
/*            TP : Visualisation de champs de vecteurs                 */
/*                                                                     */
/*                  version 2 : OpenGL et glut                         */
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

#define ABS(x)    ( (x)<0?-(x):(x) )
#define MAX(x,y)    ( (x)<(y)?(y):(x) )
#define MIN(x,y)    ( (x)>(y)?(y):(x) )

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int dedans(double xa, double ya, double xb, double yb,                */
/*            double xc, double yc, double xd, double yd,            */
/*            double xr, double yr)                                  */
/*                                                                   */
/* Determine si le point (xr,yr) se trouve dans le quadrilatere      */
/* defini par les points (xa,ya) (xb,yb) (xc,yc) (xd,yd), (dans      */
/* cet ordre).                                                       */
/*-------------------------------------------------------------------*/
int dedans(double xa, double ya, double xb, double yb,
           double xc, double yc, double xd, double yd,
           double xr, double yr);


/*-------------------------------------------------------------------*/
/*                                                                   */
/* void localiser(double xp, double yp, int *celi,int *celj)         */
/*                                                                   */
/*-------------------------------------------------------------------*/
void localiser(double xp, double yp, int *celi, int *celj);


/*-------------------------------------------------------------------*/
/*                                                                   */
/* void pq2xy(int i,int j,double *xr,double *yr,double p,double q)   */
/*                                                                   */
/* calcule les coordonnes cartesiennes d'un point a partir de ses    */
/* coordonnees parametriques                                         */
/*                                                                   */
/*-------------------------------------------------------------------*/
void pq2xy(int i,int j,double *xr,double *yr,double p,double q);


/*-------------------------------------------------------------------*/
/*                                                                   */
/* void xy2pq(int i,int j,double xr,double yr,double *p,double *q)   */
/*                                                                   */
/*-------------------------------------------------------------------*/
void xy2pq(int i,int j,double xr,double yr,double *p,double *q);


/*-------------------------------------------------------------------*/
/*                                                                   */
/* int cellule_xy2pq(int i,int j,                                    */
/*             double *pt00, double *pt10, double *pt11,double *pt01,*/
/*             double *qt00, double *qt10, double *qt11,double *qt01)*/
/*                                                                   */
/*-------------------------------------------------------------------*/
int cellule_xy2pq(int i,int j,
                  double *pt00, double *pt10, double *pt11,double *pt01,
                  double *qt00, double *qt10, double *qt11,double *qt01);


/*-------------------------------------------------------------------*/
/*                                                                   */
/* int tracer_cellule(int i,int j, double step,                      */
/*                    double pe,double qe,double *ps,double *qs)     */
/*                    double sx[], double sy[], int *nns, int maxp)  */
/*                                                                   */
/*-------------------------------------------------------------------*/
int tracer_cellule(int i, int j, double step,
                    double pe, double qe,double *ps, double *qs,
                    double sx[], double sy[], int *nns, int maxp);
