//ECDSA 전자서명 구현
#include <iostream>
#include <string>
#include <gmpxx.h>
#include <vector>

using namespace std;

struct Point{
    mpz_class x;
    mpz_class y;
};

struct EC{
    mpz_class a;
    mpz_class b;
    mpz_class p;
};

mpz_class mod_inv(mpz_class x, mpz_class mod)
{
    mpz_class r;
    if(mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t()) == 0)
        cout << "error: an inverse doesn't exist" << endl;
    return r;
}

mpz_class mod(mpz_class n, mpz_class p)
{
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t());
    return r;
}

void EC_add(EC E, Point *R, Point P, Point Q)
{
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;

    if(P.x == 0 && P.y == 0)
    {
        *R = Q;
    }
    else if(Q.x == 0 && Q.y == 0)
    {
        *R = P;
    }
    else
    {
        xtemp = Q.x - P.x;
        xtemp = mod(xtemp, E.p);
        xtemp = mod_inv(xtemp, E.p);

        if(xtemp == 0)
        {
            R->x = 0;
            R->y = 0;
        }
        else if(r < 0)
            cout << "error" << endl;
        else
        {
            ytemp = Q.y - P.y;
            ytemp = mod(ytemp, E.p);

            r = xtemp * ytemp;
            r = mod(r, E.p);

            Rx = (r*r - P.x - Q.x);
            Rx = mod(Ry, E.p);
            R->x = Rx;

            Ry = r * (P.x - Rx) - P.y;
            Ry = mod(Ry, E.p);
            R->y = Ry;
        }
    }
}