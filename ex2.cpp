//원시근 구하기
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int powm(int base, int exp, int mod)
{
    int r=1;
    int i;

    for(i=0; i<exp; i++)
    {
        r *= base;
        r %= mod;
    }
    return r;

}

bool is_primitive(int base, int mod)
{//여기서 mod는 소수여야 함
    int exp = 2;
    int r = base * base;
    for(; r != base; exp++)
    {
        r *= base;
        r %= mod;
    }
    return exp == mod;
}

vector<int> primitive_root(int mod)
{
    vector<int> v;
    for(int base=2; base<mod; base++)
    {
        if(is_primitive(base, mod))
            v.push_back(base);
    }
    return v;
}

int main()
{
    int i;
    printf("i: ");
    scanf("%d", &i);

    for(int i : primitive_root(i))
        cout << i << ", ";
    cout << endl;
}

/*
int main()
{
    int base;
    int exp;
    int mod;
    int res;

    printf("base: ");
    scanf("%d", &base);

    printf("exp: ");
    scanf("%d", &exp);

    printf("mod: ");
    scanf("%d", &mod);

    res = powm(base, exp, mod);
    cout << res << endl;

    return 0;
}
*/