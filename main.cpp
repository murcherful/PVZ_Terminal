#include <iostream>
#include <ctime>
#include <cstdlib>
#include "MainControl.h"

using namespace std;

int main()
{
	std::srand(std::time(0));
    MainControl mc(100);
    mc.start();
    return 0;
}

