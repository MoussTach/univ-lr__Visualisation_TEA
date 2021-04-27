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

#include <math.h>
#include "maillage.h"
#include "calcul.h"

#define dp(p,q,xa,xb,xc,xd) \
  ((1.0-(q)) * ((xd) - (xa)) + (q) * ((xc) - (xb)))
#define dq(p,q,xa,xb,xc,xd) \
  ((1.0-(p)) * ((xb) - (xa)) + (p) * ((xc) - (xd)))

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int dedans(double xa, double ya, double xb, double yb,            */
/*            double xc, double yc, double xd, double yd,            */
/*            double xr, double yr)                                  */
/*                                                                   */
/* Determine si le point (xr,yr) se trouve dans le quadrilatere      */
/* defini par les points (xa,ya) (xb,yb) (xc,yc) (xd,yd), (dans      */
/* cet ordre).                                                       */
/*-------------------------------------------------------------------*/
int dedans(double xa, double ya, double xb, double yb,
           double xc, double yc, double xd, double yd,
           double xr, double yr)
{
  double vx[5], vy[5];
  double v,s,vp;
  int i;
  vx[0] = xa - xr;
  vx[1] = xb - xr;
  vx[2] = xc - xr;
  vx[3] = xd - xr;
  vy[0] = ya - yr;
  vy[1] = yb - yr;
  vy[2] = yc - yr;
  vy[3] = yd- yr;
  for (i=0; i<4; i++)
    {
    s = sqrt(vx[i]*vx[i]+vy[i]*vy[i]);
    vx[i] = vx[i] / s;
    vy[i] = vy[i] / s;
    };
  vx[4] = vx[0];
  vy[4] = vy[0];
  v = 0;
  for (i=0; i<4; i++)
    {
    vp = acos((vx[i]*vx[i+1]+vy[i]*vy[i+1]) );
    v += vp;
    };
  if ((6.20 < ABS(v)) && (ABS(v) <6.32))
    {
    return 1;
    };
  return 0;
} /* dedans */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void localiser(double xp, double yp, int *celi,int *celj)           */
/*                                                                   */
/*-------------------------------------------------------------------*/
void localiser(double xp, double yp, int *celi, int *celj)
{
  int i,j;
  *celi = *celj = -1;
  for(i=0; i<nx-1; i++)
    for(j=0; j<ny-1; j++)
      {
      if (dedans(x[i  ][j  ],y[i  ][j  ],x[i+1][j  ],y[i+1][j  ],
                 x[i+1][j+1],y[i+1][j+1],x[i  ][j+1],y[i  ][j+1],
                 xp,yp)) 
         {
         *celi = i;
         *celj = j;
         return;
         };
      };
} /* localiser */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void pq2xy(int i,int j,double *xr,double *yr,double p,double q)   */
/*                                                                   */
/* calcule les coordonnes cartesiennes d'un point a partir de ses    */
/* coordonnees parametriques                                         */
/*                                                                   */
/*-------------------------------------------------------------------*/
void pq2xy(int i,int j,double *xr,double *yr,double p,double q)
{
double ump, umq;
ump = 1.0 -p;  umq = 1.0 - q;
*xr = ump * (umq * x[i  ][j] + q * x [i  ][j+1]) 
    +   p * (umq * x[i+1][j] + q * x [i+1][j+1]);
*yr = ump * (umq * y[i  ][j] + q * y [i  ][j+1]) 
    +   p * (umq * y[i+1][j] + q * y [i+1][j+1]);
} /* pq2xy */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void xypq(double xr, double yr,double *p, double *q,              */
/*           double x00,double y00, double x01, double y01,          */
/*           double x11,double y11, double x10, double y10,          */
/*           double pmin,double pmax,double qmin, double qmax)       */
/*                                                                   */
/* Calcule les coordonnees parametriques d'un point dans une cellule */
/* a partir de ses coordonnees cartesiennes. Le calcul se fait de    */
/* maniere recursive, en decoupant la cellule en 4, jusqu'a ce       */
/* que l'on ait trouve une cellule suffisament petite encadrant le   */
/* point. Le resultat obtenu n'est donc qu'une approximation.        */
/*                                                                   */
/*-------------------------------------------------------------------*/
void xypq(double x, double y,double *p, double *q, 
          double x00,double y00, double x01, double y01, 
          double x11,double y11, double x10, double y10,
          double pmin,double pmax,double qmin, double qmax)
{
double xa, ya, xb, yb, xc, yc, xd, yd, xe, ye;
xa = (x01 + x00) / 2.0;
ya = (y01 + y00) / 2.0;
xb = (x11 + x01) / 2.0;
yb = (y11 + y01) / 2.0;
xc = (x10 + x11) / 2.0;
yc = (y10 + y11) / 2.0;
xd = (x00 + x10) / 2.0;
yd = (y00 + y10) / 2.0;
xe = (xa + xb + xc + xd) / 4.0;
ye = (ya + yb + yc + yd) / 4.0;
*p = (pmin + pmax) / 2.0;
*q = (qmin + qmax) / 2.0;
if ((pmax-pmin) < 0.01)
  {
  *p = (pmin + pmax) / 2.0;
  *q = (qmin + qmax) / 2.0;
  return;
  };
if (dedans(x00,y00,xa,ya,xe,ye,xd,yd,x,y))
  {
  xypq(x,y,p,q,x00,y00,xa,ya,xe,ye,xd,yd,pmin,(pmin+pmax)/ 2.0,qmin,(qmin+qmax)/ 2.0);
  }
else
  {
  if (dedans(xa,ya,x01,y01,xb,yb,xe,ye,x,y))
    {
    xypq(x,y,p,q,xa,ya,x01,y01,xb,yb,xe,ye,pmin,(pmin+pmax)/ 2.0,(qmin+qmax)/ 2.0,qmax);
    }
  else
    {
    if (dedans(xe,ye,xb,yb,x11,y11,xc,yc,x,y))
      {
      xypq(x,y,p,q,xe,ye,xb,yb,x11,y11,xc,yc,(pmin+pmax)/2.0,pmax,(qmin+qmax)/2.0,qmax);
      }
    else
      {
      if (dedans(xd,yd,xe,ye,xc,yc,x10,y10,x,y))
        {
        xypq(x,y,p,q,xd,yd,xe,ye,xc,yc,x10,y10,(pmin+pmax)/2.0,pmax,qmin,(qmin+qmax)/2.0);
        }
      };
    };
  };
} /* xypq */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* void xy2pq(int i,int j,double xr,double yr,double *p,double *q)   */
/*                                                                   */
/*-------------------------------------------------------------------*/
void xy2pq(int i,int j,double xr,double yr,double *p,double *q)
{
xypq(xr,yr,p,q,x[i  ][j  ],y[i  ][j  ], x[i  ][j+1],y[i  ][j+1],
               x[i+1][j+1],y[i+1][j+1], x[i+1][j  ],y[i+1][j  ],
     0.0,1.0,0.0,1.0);
} /* xy2pq */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int cellule_xy2pq(int i,int j,                                    */
/*                 double *pt00, double *pt10, double *pt11,double *pt01,*/
/*                 double *qt00, double *qt10, double *qt11,double *qt01)*/
/*                                                                   */
/*-------------------------------------------------------------------*/
int cellule_xy2pq(int i,int j,                                    
                  double *pt00, double *pt10, double *pt11,double *pt01,
                  double *qt00, double *qt10, double *qt11,double *qt01)
{
double det;
double xp,yp,xq,yq;
/* pt00, qt00 */
xp = dp( 0.0, 0.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
xq = dq( 0.0, 0.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
yp = dp( 0.0, 0.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
yq = dq( 0.0, 0.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
det = xp*yq - xq*yp;
if (ABS(det) < 0.000000001) return 0;
*pt00 = (  u[i  ][j  ] * yq - v[i  ][j  ] * xq)/det;
*qt00 = (- u[i  ][j  ] * yp + v[i  ][j  ] * xp)/det;

/* pt01, qt01 */
xp = dp( 0.0, 1.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
xq = dq( 0.0, 1.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
yp = dp( 0.0, 1.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
yq = dq( 0.0,1.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
det = xp*yq - xq*yp;
if (ABS(det) < 0.000000001) return 0;
*pt01 = (  u[i  ][j+1] * yq - v[i  ][j+1] * xq)/det;
*qt01 = (- u[i  ][j+1] * yp + v[i  ][j+1] * xp)/det;
/* pt11, qt11 */
xp = dp(1.0,1.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
xq = dq(1.0,1.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
yp = dp(1.0,1.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
yq = dq(1.0,1.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
det = xp*yq - xq*yp;
if (ABS(det) < 0.000000001) return 0;
*pt11 = (  u[i+1][j+1] * yq - v[i+1][j+1] * xq)/det;
*qt11 = (- u[i+1][j+1] * yp + v[i+1][j+1] * xp)/det;
/* upq10, vpq10 */
xp = dp(1.0,0.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
xq = dq(1.0,0.0,x[i  ][j  ],x[i  ][j+1],x[i+1][j+1],x[i+1][j  ]);
yp = dp(1.0,0.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
yq = dq(1.0,0.0,y[i  ][j  ],y[i  ][j+1],y[i+1][j+1],y[i+1][j  ]);
det = xp*yq - xq*yp;
if (ABS(det) < 0.00000000001) return 0;
*pt10 = (  u[i+1][j  ] * yq - v[i+1][j  ] * xq)/det;
*qt10 = (- u[i+1][j  ] * yp + v[i+1][j  ] * xp)/det;
return 1;
} /* cellule_xy2pq */

/*-------------------------------------------------------------------*/
/*                                                                   */
/* int tracer_cellule(int i,int j, double step,                      */
/*                    double pe,double qe,double *ps,double *qs,     */
/*                    double sx[], double sy[], int *nns, int maxp)  */
/*                                                                   */
/*-------------------------------------------------------------------*/
int tracer_cellule(int i, int j, double step,
                    double pe, double qe,double *ps, double *qs,
                    double sx[], double sy[], int *nns, int maxp)
{
double pt00,pt10,pt01,pt11,qt00,qt10,qt01,qt11;
double p,q,pp,qp;
double px,py;
double pt,qt;
int flag, sortie;
int ns;
ns= *nns;
sortie = 0;
if (!cellule_xy2pq(i,j,&pt00,&pt10,&pt11,&pt01,&qt00,&qt10,&qt11,&qt01))
  return sortie;
p = pe;
q = qe;
pq2xy(i,j,&px,&py,p,q);
sx[ns] = px;
sy[ns] = py;
ns ++;
flag = 1;
pp = p;
qp = q;
pq2xy(i,j,&px,&py,p,q);
sx[ns] = px;
sy[ns] = py;
while ((ns < maxp) && flag )
  {
  pt = (1.0-p)*((1.0-q)*pt00+q*pt01) + p*((1.0-q)*pt10+q*pt11);
  qt = (1.0-p)*((1.0-q)*qt00+q*qt01) + p*((1.0-q)*qt10+q*qt11);
  p = p + pt * step;
  q = q + qt * step;
  ns ++;
  if (p <= 0.0)
    {
    flag = 0;
    q = qp + ((q - qp) * (0.0 - pp)) / ( p - pp);
    p = 0.0;
    sortie = 1;
    };
  if (p >= 1.0)
    {
    flag = 0;
    q = qp + ((q - qp) * (1.0 - pp)) / ( p - pp);
    p = 1.0;
    sortie = 2;
    };
  if (q <= 0.0)
    {
    p = pp + ((p - pp) * (0.0 - qp)) / ( q - qp);
    q = 0.0;
    flag = 0;
    sortie = 3;
    };
  if (q >= 1.0)
    {
    p = pp + ((p - pp) * (1.0 - qp)) / ( q - qp);
    q = 1.0;
    flag = 0;
    sortie = 4;
    };
  pq2xy(i,j,&px,&py,p,q);
  sx[ns] = px;
  sy[ns] = py;
  pp = p;
  qp = q;
  };
*ps = p;
*qs = q;
*nns = ns;
return sortie;
} /* tracer_cellule*/
