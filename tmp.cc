#include "stdio.h"
#include <errno.h>
#include <string.h>
#include <vector>
#include <stdexcept>
#include <iostream>
int main(){
    for(int i = 0; i < 10; ++i){
        try
        {
            if(i == 5) throw("err");
        }
        catch(const char* e)
        {
            std::cout << e << '\n';
            continue;
        }
        std::cout<<i << '\n';
        
    }
}