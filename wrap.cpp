#include <iostream>
#include <gmpxx.h>

mpz_class nextprime(mpz_class n) /*n보다 큰 최초의 소수를 return*/
{
    mpz_class r;
    mpz_nextprime(r.get_mpz_t(), n.get_mpz_t());
    return r;
}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}