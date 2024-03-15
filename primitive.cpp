#include <iostream>
#include <vector>

using namespace std;

bool is_primitive(int base, int mod);
vector<int> primitive_root(int mod);

int main()
{
    for(int i : primitive_root(17))
        cout << i << ',';
    cout << endl;
}

bool is_primitive(int base, int mod) /*p가 원하는 크기의 소수가 맞는지*/
{
    int exp =2;
    int r= base * base;
    for(; r!=base; exp++)
    {
        r *= base;
        r %= mod;
    }
    return exp == mod;
}

vector<int> primitive_root(int mod) /*(p가 주어졌을 때) g가 p의 원시근이 맞는지*/
{
    vector<int> v;
    for(int base=2; base<mod; base++)
    {
        if(is_primitive(base,mod))
        v.push_back(base);
    }
    return v;
}