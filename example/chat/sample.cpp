#include <iostream>
#include <cstring>
using namespace std;

class my_string
{
public:
    my_string(char* s) : str_(s)
    {
    }
    char* func()
    {
        cout << "in normal function\n"; 
        return str_;
    }
     char* func() const {
        cout << "in const function\n"; 
        return str_;
    }

private:
    char* str_;
};
int main()
{
    
    const string str = (new my_string("abc"))->func();
    my_string s1( "111");
    const my_string s2("222");
    s1.func();
    s2.func();
    
    return 0;
}