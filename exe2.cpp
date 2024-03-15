//CRT 이용한 RSA code
#include <iostream>
#include <string>
#include <gmpxx.h>

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
mpz_class RSA_en(RSA_PK pk, mpz_class msg);
mpz_class RSA_de_CRT(RSA_SK sk, mpz_class ct);

mpz_class RSA_sign(RSA_SK sk, mpz_class msg);
bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg);

mpz_class  urandomb(gmp_randstate_t state, unsigned int bit)
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

mpz_class mod(mpz_class n, mpz_class p)
{
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t());
    return r;
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

//암호화
mpz_class RSA_en(RSA_PK pk, mpz_class msg)
{
    mpz_class ct;
    ct = powm(msg, pk.e, pk.N);
    return ct;
}

//CRT 이용한 복호화
mpz_class RSA_de_CRT(RSA_SK sk, mpz_class ct)
{
    mpz_class m1, m2;
    mpz_class M1_inv, M2_inv;
    mpz_class msg;

    m1 = powm(ct, sk.d, sk.p);
    m2 = powm(ct, sk.d, sk.q);
    M1_inv = mod_inv(sk.q, sk.p);
    M2_inv = mod_inv(sk.p, sk.q);

    msg = (m1 * sk.q * M1_inv);
    msg = mod(msg, sk.N);
    msg += (m2 * sk.p * M2_inv);
    msg = mod(msg, sk.N);
    return msg;
}

//전자서명
mpz_class RSA_sign(RSA_SK sk, mpz_class msg)
{
    mpz_class sig;
    sig = powm(msg, sk.d, sk.N);
    return sig;
}

//검증하는거임
bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg)
{
    mpz_class r;
    r = powm(sig, pk.e, pk.N);
    if(cmp(r, msg) == 0)
        return true;
    else
        return false;
}

int main()
{
    mpz_class msg{"0x23423423"};
    mpz_class sig;
    RSA_PK pk;
    RSA_SK sk;
    mpz_class ct;
    mpz_class dec;
    bool ver;

    RSA_keygen(&pk, &sk, 2048);
    
    ct = RSA_en(pk, msg);
    dec = RSA_de_CRT(sk, ct);
    
    if(cmp(msg, dec) != 0)
        cout << "decryption failed!" << endl;
    else
        cout << "decryption success!" << endl;

    sig = RSA_sign(sk, msg);
    ver = RSA_verify(pk, sig, msg);

    if(ver == true)
        cout << "검증 success" << endl;
    else
        cout << " 검증 failed" << endl;

}