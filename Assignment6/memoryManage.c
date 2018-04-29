
/* 
 * @File:   memoryManage.c
 * @Author: Chris Hyde
 *
 * @Created: April 27
 */

#include <stdio.h>
#include <stdlib.h>

/*
 *@to_hex
 */
void to_hex(int decimal){
    int remainder, temp; 
    int j = 0; // index of the hex array
    char hex[7]; // 32 bit hex should only have 8 characters
    
    temp = decimal; //temp is the holder for the decimal
    for(int i = 7; i >= 0; i--){
        remainder = decimal % 16;
        if(remainder < 10){
            hex[j++] = 48 + remainder;
        }else{
            hex[j++] = 55 + remainder;
        }
        decimal /= 16;
    }
    
    //Print the hex array 
    printf("\nDecimal %d as a hexadecimal: 0x", temp);
    for(j; j>=0; j--){
        printf("%c", hex[j]);
    }
    printf("\n");
}
/*
 * @to_binary
 */
void to_binary(int decimal){
    
    int index = 31; //the number of bits in the binary to represent
    
    printf("\nDecimal %d as binary:", decimal);
    
    //loop through each index and print 1 or 0
    for(index; index >= 0; index--){
        if((1 << index) & decimal){
            printf("1");
        }else{
            printf("0");
        }
        
         if(index != 31 && index % 4 == 0){
           printf(" "); //print space every 4 index to represent every 4 bit integer
        }
        
    }
    printf("\n");
}

/*
 * @modify_input
 */
void modify_input(int decimal){
    int new_decimal;
    int mask = -65536; 
    int hex = 2047; // decimal representation of 0x07ff
    
    //shift left 16 bits and mask out last 16 bits then or in 0x07ff
     new_decimal = (((decimal << 16) & mask) | hex); 
    
    //print new_decimal as a decimal, binary, and hex
    printf("\nDecimal %d was modified to a decimal of: %d\n", decimal, new_decimal);
    to_binary(new_decimal);
    to_hex(new_decimal); //if done correctly will always end in 7ff. 
    
}

/*
 * @user_input
 */
int user_input(){
    int user_decimal;
    int bool = 0;
    int i = 0;
    char temp[100];
    char* c;

    while (bool != 1){
        //ask user to input a number between 0 and 4095
        printf("Please enter a number between 0 and 4095: ");
        scanf("%d", &user_decimal);
        
        //assign any none integers to c
            while ((temp[i] = getchar()) != '\n'){
                c += temp[i];
                i++;
            }
        
        //check to see if c isn't just next line
        if (c != "\n") {
            printf("\nERROR: Invalid Entry - %s\n", temp);
            c = "\n"; //rest c
            
        } else {
            if (user_decimal > 4095 || user_decimal < 0) { //make sure user input is a valid number
                printf("\nERROR: Invalid Entry - %d\n\n", user_decimal);
            } else {
                bool = 1;
            }
        }
        
    }
    return user_decimal;
}

/*
 *@Main 
 */
int main(int argc, char** argv) {
    int input = user_input();
    
    to_binary(input); //print 32 bit binary to terminal
    to_hex(input); // print 32 bit hex to terminal
    
    
    modify_input(input);

    return (EXIT_SUCCESS);
}

