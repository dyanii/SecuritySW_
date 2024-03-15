//RSA 암호 구현
#include <iostream>
#include <gmpxx.h>
#include <string>

using namespace std;

struct RSA_PK{
    mpz_class e;
    mpz_class N;
};

struct RSA_SK{
    mpz_class e;
    mpz_class d;
    mpz_class N;
    mpz_class p;
    mpz_class q;
};

void RSA_KeyGen(struct RSA_PK *pk, struct RSA_SK *sk, int key_size);
mpz_class RSA_encrypt(struct RSA_PK pk, mpz_class msg);
mpz_class RSA_decrypt(struct RSA_SK sk, mpz_class ct);

mpz_class urandomb(gmp_randstate_t state, unsigned int bit)
{
    mpz_class r;
    mpz_urandomb(r.get_mpz_t(), state, bit);
    return r;
}

mpz_class nextprime(mpz_class n)
{
    mpz_class r;
    mpz_nextprime(r.get_mpz_t(), n.get_mpz_t());
    return r;
}

mpz_class random_prime(unsigned int bit)
{
    mpz_class r, p;
    unsigned long seed;
    
    seed = time(NULL);

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    r = urandomb(state, bit);
    p = nextprime(r);

    if(mpz_sizeinbase(p.get_mpz_t(), 2) != bit)
        return random_prime(bit);
    else
        return p;
}

mpz_class mod_inv(mpz_class x, mpz_class y)
{
    mpz_class r;
    mpz_invert(r.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
    return r;
}

int cmp(mpz_class x, mpz_class y)
{
    int r;
    r = mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
    return r;
}

void RSA_KeyGen(RSA_PK *pk, RSA_SK *sk, int key_size)
{
    mpz_class ell;

    sk->p = random_prime(key_size / 2);
    sk->q = random_prime(key_size / 2);
    while(cmp(sk->p, sk->q) == 0)
        sk->q = random_prime(key_size / 2);
    sk->N = sk->p * sk->q;
    pk->N = sk->N;

    ell = lcm(sk->p - 1, sk->q -1);

    pk->e = 0x10001;

    if(gcd(pk->e, ell) != 1)
        pk->e = nextprime(pk->e);

    sk->e = pk->e;
    sk->d = mod_inv(pk->e, ell);
}

mpz_class lcm(mpz_class a, mpz_class b)
{
    mpz_class r;
    mpz_lcm(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return r;
}

mpz_class gcd(mpz_class a, mpz_class b)
{
    mpz_class r;
    mpz_gcd(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return r;
}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class RSA_encrypt(RSA_PK pk, mpz_class msg)
{
    mpz_class ct;
    ct = powm(msg, pk.e, pk.N);
    return ct;
}

mpz_class RSA_decrypt(RSA_SK sk, mpz_class ct)
{
    mpz_class msg;
    msg = powm(ct, sk.d, sk.N);
    return msg;
}

//메인함수
int main()
{
    int n = 2048; //key size
    RSA_PK pk;
    RSA_SK sk;

    mpz_class msg{"0x123456789"};
    mpz_class ct;

    RSA_KeyGen(&pk, &sk, n);

    cout << "메시지는 " << msg << endl;

    ct = RSA_encrypt(pk, msg);
    cout << "암호문은 " << ct << endl;

    msg = RSA_decrypt(sk, ct);
    cout << "복호화된 메시지는 " << msg << endl;

    return 0;
}
// gcc 1번문제.cpp -lstdc++ -lgmpxx -lgmp -o 1번문제