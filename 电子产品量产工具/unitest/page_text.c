

#include <stdlib.h>
#include "page_manager.h"

int main(int argc ,char** argv)
{
    AppRegisterPage();
    
    AppSelectPage("main")->PageHandler(NULL);
    return 0;
}

