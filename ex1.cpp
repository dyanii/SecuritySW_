//시저암호 구현하기
#include <iostream>
#include <string>

using namespace std;

string caesar_en(string message, int key)
{
    for(char &c : message) c += key;
    return message;
}

string caesar_de(string message, int key)
{
    for(char &c : message) c -= key;
    return message;
}

int main()
{
    string m = "Caesar's cipher test";
    cout << m << endl;
    cout << (m = caesar_en(m, 3)) << endl;
    cout << caesar_de(m, 3) << endl;
}