// #include "polyalpha_decrypt.h"
// #include "input_control.h"
#include "support_functions.h"
#include <string>
#include <cstring>
#include <iostream>


int verifyCiphertext(char *cipher_buffer)
{
    int i;
    
    if (cipher_buffer == NULL)
        return 0;
    
    for (i = 0; i < 151; i++) { // 151 is maximum cipher length
        if (cipher_buffer[i] == 0){
            
            if (i >= 50 && i <= 150)
                return 1;
            return 0;
        }
        
        if (islower(cipher_buffer[i]) == 0)
            return 0;
    }
    return 0;
}

int verifyKeylength(int key)
{
    
    if (key < 1 || key > 40)
        return 0;
    return 1;
}


void inputKeylength(int *keylength)
{
    int verified, key;
    if (keylength == NULL)
        return;
    do{
        std::cout<<"Enter Key Length:";
        std::cin>>key;
        verified = verifyKeylength(key);
        if (!verified)
            printf("Grrrr!! The keylength should be between 1 and 40!");
    }while(!verified);
    *keylength = key;
    return;
}

void inputCiphertext(char *ciphertext)
{
    int verified;
    do{
        
        std::cout<<"Enter a ciphertext \n";
        std::cin>>ciphertext;
        verified = verifyCiphertext(ciphertext);
        if (!verified)
            std::cout<<"Ooops !! The ciphertext should be between 50 and 150!";
        
    }while(!verified);
    
    return;
}


int main()
{
    int keylength;
    char ciphertext[250];
    char *plaintext;
//    char *(* break_cipher[ALGORITHM_NUMBER])(int keylength, char *ciphertext) = {
//        // break_polyalpha_assuming,
//        break_polyalpha_no_stretching_dict1,
//        break_polyalpha_nostretching_dict2_wrapper,
//    };
//    int i = 0;
    
    inputKeylength(&keylength);
    inputCiphertext(ciphertext);
    
    plaintext = breakCipherDict1(keylength, ciphertext);
    if (!plaintext){
        plaintext = breakCipherDict2(keylength, ciphertext);
        if (plaintext) {
            std::cout<<"Found plaintext from dict 2";
            std::cout<<"\n"<<plaintext<<"\n";
            delete(plaintext);
        } else {
            std::cout<<"Plaintext not found";
        }

    } else {
        std::cout<<"Found plaintext from dict 1";
        std::cout<<"\n"<<plaintext<<"\n";
        delete(plaintext);
    }
    
//    do{
//        
//        // plaintext = break_cipher[i](keylength, ciphertext);
//        if (plaintext) {
//            std::cout<<"Found plaintext";
//            std::cout<<"\n"<<plaintext;
//            break;
//        }
//        i++;
//    }while(i < ALGORITHM_NUMBER);
//    if (!plaintext)
//        std::cout<<"Plaintext not found";
//    else
//        delete(plaintext);
    return 0;
}


