#include <iostream>

using namespace std;

int main(){
    int arr1[]={0,1,2,3};

    for(int i=0; i<4; i++){
        cout << arr1[i] << '\n';
    }

    for(int num : arr1){
        cout << num << '\n';
    }
}