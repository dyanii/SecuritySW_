//타원곡선(ecdh) int 구현하기
#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct Point{
    int x;
    int y;
};

struct EC{
    int a;
    int b;
    int p;
};

int mod_inv(int x, int mod);
void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point*R, Point P);
void bits_inv(vector<int> *bits, int r);
void EC_mult(EC E, Point *R, Point P, int r);

int mod_inv(int x, int mod)
{
    int r;

    if(x==0)
        return 0;
    else if(mod<=0)
        return -1;
    else
    {
        while(x<0){
            x += mod;
        }

        for(r=0; r<mod; r++)
        {
            if((r*x)%mod == 1)
                return r;
        }
        return -1;
    }
}

void EC_add(EC E, Point *R, Point P, Point Q)
{
    int r;//r이 감마임

    if(P.x == 0 && P.y == 0)
    {
        *R = Q; //P가 infinity일때
    }
    else if(Q.x == 0 && Q.y == 0)
    {
        *R = P; //Q가 infinity일때
    }
    else
    {
        r = mod_inv((Q.x - P.x), E.p);
        if(r == 0) //역원이 없을 때
        {
            R->x = 0;
            R->y = 0;
        }
        else if(r<0)
        {
            cout << "error1" << endl;
        }
        else
        {
            //감마값 계산(모드도 취해줘야함)
            r *= (Q.y - P.y);
            r %= E.p;

            R->x = (r*r - P.x -Q.x);
            while(R->x < 0)
            {
                R->x += E.p;
            }
            R->x %= E.p;
        
            R->y = r*(P.x - R->x) - P.y;
            while(R->y < 0)
            {
                R->y += E.p;
            }
            R->y %= E.p;
        }
    }
}

void EC_double(EC E, Point*R, Point P)
{
    int r;

    r = mod_inv((2*P.y), E.p);

    if(r == 0)
    {
        R->x = 0;
        R->y = 0;
        //cout << "Point of infinity" << endl;
    }
    else if(r < 0)
        cout << "error2" << endl;
    else
    {
        r *= (3*P.x*P.x + E.a);
        r %= E.p;

        R->x = r*r - 2*P.x;
        while(R->x < 0)
        {
            R->x += E.p;
        }
        R->x %= E.p;

        R->y = r*(P.x - R->x) - P.y;
        while(R->y < 0)
        {
            R->y += E.p;
        }
        R->y %= E.p;
    }
}

void bits_inv(vector<int> *bits, int r)
{
    for(; r > 0; r /= 2)
    {
        (*bits).push_back(r % 2);
    }
}

void EC_mult(EC E, Point *R, Point P, int r)
{
    vector<int> bits;
    int n;

    if(P.x == 0 && P.y == 0)
    {
        R->x = 0;
        R->y = 0;
        cout << "point of infinity" << endl;
    }
    else if(r <= 0)
        cout << "error3" << endl;
    else
    {
        bits_inv(&bits, r);
        n = bits.size();

        *R = P;

        for(; n > 1; n--)
        {
            EC_double(E, R, *R);
            if(bits.at(n-2) == 1)
                EC_add(E, R, *R, P);
        }
    }
}

int main()
{
    Point A, B;
    Point R;
    EC E;
    Point C;

    printf("A의 좌표를 입력하시오: ");
    scanf("%d %d", &A.x, &A.y);

    //A={1,4};
    B={3,1};
    C={1,1};
    E.a = 2; E.b = 3; E.p = 5;

    EC_add(E, &R, A, B);
    cout << "A+B = (" << R.x << ", " << R.y << ")" << endl;

    EC_double(E, &R, A);
    cout << "2A = (" << R.x << ", " << R.y << ")" << endl;

    EC_mult(E, &R, C, 7);
    cout << "7C = (" << R.x << ", " << R.y << ")" << endl;

    EC_mult(E, &R, C, 9);
    cout << "9C = (" << R.x << ", " << R.y << ")" << endl;

}