
// For C++11 above 
#include <iostream> 
#include <cstdlib> 
using namespace std; 
   
int main() 
{ 
    const char *str1 = "42"; 
    const char *str2 = "3.14159"; 
    const char *str3 = "geek 34"; 
     
    int num1 = atoi(str1); 
    int num2 = atoi(str2); 
    int num3 = atoi(str3); 
    
    cout << "atoi(\"" << str1  
              << "\") is " << num1 << '\n'; 
    cout << "atoi(\"" << str2  
              << "\") is " << num2 << '\n'; 
    cout << "atoi(\"" << str3  
              << "\") is " << num3 << '\n'; 
     
   return 0; 
} 
