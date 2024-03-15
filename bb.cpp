//ecdsa
#include <iostream>
#include <string>
#include <gmpxx.h>

using namespace std;

struct EC{
    mpz_class a;
    mpz_class b;
    mpz_class p;
};

struct Point{
    mpz_class x;
    mpz_class y;
};

struct ECDSA_PK{
    EC E;
    mpz_class n;
    Point G;
    Point Q;
};

struct ECDSA_SK{
    EC E;
    mpz_class n;
    Point G;
    Point Q;
    mpz_class d;
};

struct ECDSA_SIG{
    mpz_class S1;
    mpz_class S2;
};

mpz_class mod(mpz_class n, mpz_class p)
{
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t());
    return r;
}

mpz_class mod_inv(mpz_class x, mpz_class mod)
{
    mpz_class r;
    mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class mod)
{
    mpz_class r;
    mpz_urandomm(r.get_mpz_t(), state, mod.get_mpz_t());
    return r;
}

int cmp(mpz_class a, mpz_class b)
{
    int r;
    r = mpz_cmp(a.get_mpz_t(), b.get_mpz_t());
    return r;
}

void EC_double(EC E, Point *R, Point P)
{
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;
    mpz_class r;

    ytemp = 2 * P.y;
    ytemp = mod(ytemp, E.p);
    ytemp = mod_inv(ytemp, E.p);

    if(ytemp == 0)
    {
        R->x = 0;
        R->y = 0;
    }
    else if(ytemp < 0)
        cout << "error" << endl;
    else
    {
        xtemp = (3 * P.x * P.x) + E.a;
        xtemp = mod(xtemp, E.p);

        r = xtemp * ytemp;
        r = mod(r, E.p);

        Rx = r*r - (2 * P.x);
        Rx = mod(Rx, E.p);
        R->x = Rx;

        Ry = r * (P.x - Rx) - P.y;
        Ry = mod(Ry, E.p);
        R->y = Ry;
    }
}

void EC_add(EC E, Point *R, Point P, Point Q)
{
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;
    mpz_class r;

    if(P.x == 0 && P.y == 0)
    {
        *R = Q;
    }
    else if(Q.x == 0 && Q.y)
    {
        *R = P;
    }
    else if(P.x == Q.x && P.y == Q.y)
    {
        EC_double(E, R, P);
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
        else if(xtemp < 0)
            cout << "error" << endl;
        else
        {
            ytemp = Q.y - P.y;
            ytemp = mod(ytemp, E.p);

            r = xtemp * ytemp;
            r = mod(r, E.p);

            Rx = (r*r - P.x - Q.x);
            Rx = mod(Rx, E.p);
            R->x = Rx;

            Ry = r*(P.x - Rx) - P.y;
            Ry = mod(Ry, E.p);
            R->y = Ry;
        }
    }
}

void bits_inv(vector<bool> *bits, mpz_class r)
{
    for(; r>0; r/=2)
    {
        (*bits).push_back(r % 2 == 1);
    }
} // r을 비트 단위로 변환 후 역순으로 저장

void EC_mult(EC E, Point *R, Point P, mpz_class r)
{
    vector<bool> bits;
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

void ECDSA_keygen(ECDSA_PK *pk, ECDSA_SK *sk)
{
    pk->E = {2, 2, 17};
    sk->E = pk->E;
    pk->G = {5, 1};
    sk->G = pk->G;
    pk->n = 19;
    sk->n = pk->n;

    gmp_randstate_t state;
    unsigned long seed;
    seed = time(NULL);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    sk->d = urandomm(state, sk->n);
    if(sk->d == 0)
        sk->d = urandomm(state, sk->n);
    EC_mult(sk->E, &sk->Q, sk->G, sk->d);

    pk->Q = sk->Q;
}

ECDSA_SIG ECDSA_sign(ECDSA_SK sk, mpz_class msg)
{
    unsigned long seed;
    gmp_randstate_t state;
    mpz_class k, inv_k;
    mpz_class h_m;
    int nBit, mBit;
    Point R;
    ECDSA_SIG sig;

    nBit = mpz_sizeinbase(sk.n.get_mpz_t(), 2);
    mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
    h_m = msg >> max(mBit - nBit, 0);

    gmp_randinit_mt(state);
    do{
        seed = time(NULL);
        gmp_randseed_ui(state, seed);
        k = urandomm(state, sk.n);
        EC_mult(sk.E, &R, sk.G, k);
    }while(mod(R.x, sk.n) == 0);

    sig.S1 = mod(R.x, sk.n);

    inv_k = mod_inv(k, sk.n);

    if(inv_k == 0)
    {
        cout << "no inverse k" << endl;
        ECDSA_sign(sk, msg);
    }

    sig.S2 = h_m + (sig.S1 * sk.d);
    sig.S2 = mod(sig.S2, sk.n);
    sig.S2 = inv_k * sig.S2;
    sig.S2 = mod(sig.S2, sk.n);

    if((sig.S2 || mod_inv(sig.S2, sk.n)) == 0)
    {
        cout << "repeat SIGN" << endl;
        return ECDSA_sign(sk, msg);
    }
    else
        return sig;
}

bool ECDSA_verify(ECDSA_PK pk, ECDSA_SIG sig, mpz_class msg)
{
    mpz_class r;
    mpz_class c;
    mpz_class h_m;
    int nBit, mBit;
    Point R;
    Point P, Q;
    mpz_class u1, u2;

    nBit = mpz_sizeinbase(pk.n.get_mpz_t(), 2);
    mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
    h_m = msg >> max(mBit - nBit, 0);

    r = mod_inv(sig.S2, pk.n);
    c = mod(r, pk.n);

    u1 = mod(h_m * c, pk.n);
    u2 = mod(sig.S1 * c, pk.n);

    EC_mult(pk.E, &P, pk.G, u1);
    EC_mult(pk.E, &Q, pk.Q, u2);
    EC_add(pk.E, &R, P, Q);

    if(R.x == 0 && R.y == 0)
    {
        cout << "point of infinity" << endl;
        return false;
    }
    else if(cmp(sig.S1, mod(R.x, pk.n)) == 0)
        return true;
    else
        return false;
}

int main()
{
    ECDSA_PK pk;
    ECDSA_SK sk;
    ECDSA_SIG sig;
    mpz_class msg{"0x10"};
    bool rop;

    ECDSA_keygen(&pk, &sk);

    sig = ECDSA_sign(sk, msg);

    rop = ECDSA_verify(pk, sig, msg);

    if(rop == true)
        cout << "true" << endl;
    else if(rop == false)
        cout << "false" << endl;
    else
        cout << "error" << endl;

    return 0;
}