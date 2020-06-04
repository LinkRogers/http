#include "httplib.h"
#include <iostream>
#include <fstream> 
#include <unistd.h>

using namespace std;

int main(void)
{

  int arr[2];
  arr[0] = 1;
  arr[1] = 2;
  int *pint = arr;

  cout << "pint+!: " << pint+1 << endl;
  cout << "arr+1: " << arr+1 << endl;
  cout << "&arr+1: " << &arr+1 << endl;

  cout << "sizeof(arr)" << sizeof(arr) << endl;
  cout << "sizeof(*arr)" << sizeof(*arr) << endl;
  cout << "sizeof(&arr)" << sizeof(&arr) << endl;

}
