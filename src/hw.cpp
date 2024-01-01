#include <iostream>
#include <fstream>
#include "./headers/cmd.h"

int main()
{
    Command cmnd;
    cmnd.displayHelp();
    cmnd.inputCommand();

    return 0;
}
