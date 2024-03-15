//타원곡선(ecdh) mpz_class로 구현하기(nyc code)
#include <iostream>
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

mpz_class mod_inv(mpz_class x, mpz_class mod);
int cmp(mpz_class x, mpz_class y);
mpz_class mod(mpz_class n, mpz_class p);

void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void bits_inv(vector<mpz_class> *bits, mpz_class r);
void EC_mult(EC E, Point *R, Point P, mpz_class r);
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);

int main() {
  struct EC secp256k1;
  struct Point A,B, pubA, pubB, sA, sB;
  mpz_class da, db;
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  secp256k1 = {0, 7, mpz_class{"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}};
  // A = 1G
  A = {mpz_class{"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
  mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};
  // B = 2G
  B = {mpz_class{"0xC6047F9441ED7D6D3045406E95C07CD85C778E4B8CEF3CA7ABAC09B95C709EE5"},
  mpz_class{"0x1AE168FEA63DC339A3C58419466CEAEEF7F632653266D0E1236431A950CFE52A"}};
    
  // private key 랜덤 값으로 생성
  da=urandomm(state, secp256k1.p);
  db=urandomm(state, secp256k1.p);
  
  //public key 생성
  EC_mult(secp256k1, &pubA, A, da);
  EC_mult(secp256k1, &pubB, A, db);

  cout << pubA.x << endl;
  cout << pubB.x << endl;

  // 키 교환하고 다시 shared key 생성
  EC_mult(secp256k1, &sA, pubB, da);
  EC_mult(secp256k1, &sB, pubA, db);
  cout << sA.x << endl;
  cout << sB.x << endl;

  // 공유된 키가 똑같은지 비교하는 과정
  if ((cmp(sA.x, sB.x) ==0) && (cmp(sA.y, sB.y) ==0)){
    cout << "success!" << endl; }
  else
    cout << "failed!" << endl;
 
  return 0;
}

void EC_mult(EC E, Point *R, Point P, mpz_class r){
    vector<mpz_class> bits;
    int n;
    bits_inv(&bits, r);
    n = bits.size();
    *R=P;
    for(; n>1; n--){
        EC_double(E, R, *R);
        if(bits.at(n-2)==1)
            EC_add(E, R, *R, P);
    }
}
void bits_inv(vector<mpz_class> *bits, mpz_class r){
    for(; r>0; r/=2){
        (*bits).push_back(r%2);
    }
}
void EC_add(EC E, Point *R, Point P, Point Q){
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;
    
    if(P.x ==0 && P.y==0){
      *R=Q;
    }
    else if (Q.x==0 && Q.y==0){
      *R=P;
    }
    xtemp = Q.x - P.x;
    xtemp = mod(xtemp, E.p);
    xtemp = mod_inv(xtemp, E.p);
    ytemp = Q.y - P.y;
    ytemp = mod(ytemp, E.p);
    r = xtemp*ytemp;
    r = mod(r, E.p);
    Rx = (r*r - P.x - Q.x);
    Rx = mod(Rx, E.p);
    R->x = Rx;
    Ry = r*(P.x - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R->y = Ry;
}
mpz_class mod_inv(mpz_class x, mpz_class mod){
  mpz_class r;
  if(mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t()) == 0)
    cout<<"error: an inverse doesn't exist"<<endl;
  return r;
}
    
mpz_class mod (mpz_class n, mpz_class p){
  mpz_class r;
  mpz_mod(r.get_mpz_t(),n.get_mpz_t(), p.get_mpz_t());
  return r;
}
int cmp(mpz_class x, mpz_class y){
  int r;
  r=mpz_cmp(x.get_mpz_t(),y.get_mpz_t());
  return r;
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
  Rx = r*r - (2 * P.x);
  Rx = mod(Rx, E.p);
  R->x = Rx;
  Ry = r*(P.x - Rx) - P.y;
  Ry = mod(Ry, E.p);
  R->y = Ry;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n){
  mpz_class r;
  mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
  return r;
}