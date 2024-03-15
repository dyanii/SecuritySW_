#include <iostream>

using namespace std;

int powm(int base, int exp, int mod);

int main()
{
    int A, B;
    A = powm(19, 12, 29);
    B = powm(19, 23, 29);

    int C, D;
    C = powm(A, 23, 29);
    D = powm(B, 12, 29);
    if(C == D)
    {
        cout << "동일한 키가 생성됨" << endl;
    } else{
        cout << "동일한 키가 생성되지 않음" << endl;
    }
}

int powm(int base, int exp, int mod)
{
    int r = 1;
    int i;

    for(i=0; i<exp; i++)
    {
        r *= base; //base가 g임
        r %= mod;
    }
    return r;
}