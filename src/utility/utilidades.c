#include "utilidades.h"  /* Include the header (not strictly necessary here) */

int combinaciones(int n, int r)    /* Function definition */
{
    int ncr;
    ncr=fact(n)/(fact(r)*fact(n-r));
    return ncr;
}

int fact(int n)
{
    int i,f=1;
    for(i=1;i<=n;i++)
    {
        f=f*i;
    }
    return f;
}