/*
 * File:   main.c
 * Author: Mathew
 *
 * Created on May 7, 2024, 10:09 AM
 */

/*
 * In this assignment, we practiced using the MCC and serial terminal.
 */

#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <time.h>
/*
    Main application
*/
int random_number(int min_num, int max_num) {
    int result =0;
    int low_num = 0;
    int hi_num = 0;
    
    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1;
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }
    
    while(1) {
        result = rand();
        if (result <= max_num) {
            if (result >= min_num) {
                return result;
            }
        }
    }
    
    //I added the above while loop in an effort to make my results more random
    //than the code below. However, it turns out the rand() function is actually
    //just not very random
    
    //result = (rand() % (hi_num-low_num)) + low_num;
    //return result;
}

int main(void)
{
    SYSTEM_Initialize();
    UART2_Initialize();
    srand(time(NULL));

    while(1)
    {
        printf("%d\r\n",random_number(1,100));
    }    
}