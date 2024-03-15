//RSA 키 생성 구현하기
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

void RSA_keygen(RSA_PK *pk, RSA_SK *sk, int key_size);
mpz_class random_prime(unsigned int bit);
mpz_class lcm(mpz_class a, mpz_class b);
mpz_class nextprime(mpz_class n);
mpz_class mod_inv(mpz_class x, mpz_class mod);
mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);
int cmp(mpz_class A, mpz_class B);
mpz_class urandomb(gmp_randstate_t state, unsigned int bit);
mpz_class gcd(mpz_class a, mpz_class b);

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

mpz_class mod_inv(mpz_class x, mpz_class mod)
{
    mpz_class r;
    mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}

int cmp(mpz_class A, mpz_class B)
{
    int r;
    r = mpz_cmp(A.get_mpz_t(), B.get_mpz_t());
    return r;
}

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
    gmp_randstate_t state;

    seed = time(NULL);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    r = urandomb(state, bit);
    p = nextprime(r);

    if(mpz_sizeinbase(p.get_mpz_t(), 2) != bit)
        return random_prime(bit);
    else
        return p;
}

void RSA_keygen(RSA_PK *pk, RSA_SK *sk, int key_size)
{
    mpz_class ell;

    sk->p = random_prime(key_size / 2);
    sk->q = random_prime(key_size / 2);
    while(cmp(sk->p, sk->q) == 0)
        sk->q = random_prime(key_size / 2);
    sk->N = sk->p * sk->q;
    pk->N = sk->N;

    ell = lcm(sk->p - 1, sk->q - 1);
    pk->e = 0x10001;

    if(gcd(pk->e, ell) != 1)
        nextprime(pk->e);

    sk->e = pk->e;
    sk->d = mod_inv(pk->e, ell);
}

int main()
{
    /*mpz_class x;*/
    int i; //key size
    RSA_PK pk;
    RSA_SK sk;

    printf("key size: ");
    scanf("%d", &i);

    /*
    x = random_prime(i);
    cout << x << endl;
    cout << mpz_probab_prime_p(x.get_mpz_t(), 50) << endl;
    */

    RSA_keygen(&pk, &sk, i);
    cout << "두 개의 난수: " << sk.p << ", " << sk.q << endl;
    cout << "공개키: " << pk.e << endl;
    cout << "비밀키: " << sk.d << endl;

    return 0;
}