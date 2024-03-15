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

mpz_class urandomm(gmp_randstate_t state, const mpz_class n);

mpz_class mod_inv(mpz_class x, mpz_class mod);
int cmp(mpz_class x, mpz_class y);
mpz_class mod(mpz_class n, mpz_class p);
mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);

void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void bits_inv(vector<int> &bits, int r);
void EC_mult(EC E, Point *R, Point P, int r);

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n)
{
    mpz_class r;
    mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
    return r;
}

void bits_inv(vector<int> *bits, int r)
{
    for(; r>0; r/=2)
    {
        (*bits).push_back(r % 2);
    }
}

mpz_class mod_inv(mpz_class x, mpz_class mod)
{
    mpz_class r;
    if (mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t())== 0)
        cout << "error: an inverse doesn't exist" << endl;
    return r;
}

int cmp(mpz_class x, mpz_class y)
{
    int r;
    r = mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
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
        xtemp = Q.x- P.x;
        xtemp = mod(xtemp, E.p);
        xtemp = mod_inv(xtemp, E.p);
        ytemp = Q.y - P.y;
        ytemp = mod(ytemp, E.p);
        r = xtemp * ytemp;
        r = mod(r, E.p);

        Rx = (r*r - P.x - Q.x);
        Rx = mod(Rx, E.p);
        R->x = Rx;

        Ry = r*(P.x- Rx) - P.y;
        Ry = mod(Ry, E.p);
        R->y = Ry;
    }

}

void EC_double(EC E, Point *R, Point P)
{
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;

    ytemp = 2 * P.y;
    ytemp = mod(ytemp, E.p);
    ytemp = mod_inv(ytemp, E.p);
    xtemp = (3 * P.x * P.x) + E.a;
    xtemp = mod(xtemp, E.p);
    r = xtemp * ytemp;
    r = mod(r, E.p);

    Rx = r*r - (2*P.x);
    Rx = mod(Rx, E.p);
    R->x = Rx;

    Ry = r*(P.x - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R->y = Ry;
}

void EC_mult(EC E, Point *R, Point P, int r)
{
    vector<int> bits;
    int n;

    bits_inv(&bits, r);
    n = bits.size();

    *R = P;

    for(; n>1; n--)
    {
        EC_double(E, R, *R);
        if(bits.at(n-2) == 1)
            EC_add(E, R, *R, P);
    }
}

int main(){
    EC secp256k1;
    secp256k1 = {0, 7, mpz_class{"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}};

    Point A, B, C, R;

    //생성원임
    A = {mpz_class{"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
        mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};
    B = {mpz_class{"0xC6047F9441ED7D6D3045406E95C07CD85C778E4B8CEF3CA7ABAC09B95C709EE5"},
        mpz_class{"0x1AE168FEA63DC339A3C58419466CEAEEF7F632653266D0E1236431A950CFE52A"}};
    C = {mpz_class{"0xF9308A019258C31049344F85F89D5229B531C845836F99B08601F113BCE036F9"},
        mpz_class{"0x388F7B0F632DE8140FE337E62A37F3566500A99934C2231B6CB9FD7584B8E672"}};
    
    mpz_class xA;
    mpz_class xB;
    mpz_class pubA;
    mpz_class pubB;

    gmp_randstate_t state;
    gmp_randinit_mt(state);

    xA = urandomm(state, secp256k1.p);
    xB = urandomm(state, secp256k1.p);
    

    EC_mult(secp256k1, &R, A, 3);

    if((cmp(R.x, C.x) == 0) && (cmp(R.y, C.y) == 0))
    {
        cout << "KeyGen: (" << R.x << ", " << R.y << ")" << endl;
        cout << "success!" << endl;
    }
    else
        cout << "failed! try again" << endl;
        
    return 0;
}