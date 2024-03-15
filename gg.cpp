// rsa - crt
#include <gmpxx.h>
#include <iostream>

mpz_class RSA_decrypt_CRT(RSA_SK sk, mpz_class ct)
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