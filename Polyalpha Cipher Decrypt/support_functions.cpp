
#include "support_functions.h"
#include "dictionary1.h"
#include "dictionary2.h"
#include "threeLetters.h"
#include "cctype"
#include <iostream>

int verify_segment_in_dict1(char *proposed_cipher, int offset)
{

    size_t segment_length;
    int i;

    if (proposed_cipher == NULL)
        return 0;

    segment_length = strlen(proposed_cipher);
    
    if (offset < 0)
        return 0;

    if (offset + segment_length > dictionary1_longest)
        return 0;
   
    for (i = 0; i < dictionary1_length; i++) {
        if (strncmp(DICTIONARY1[i]+offset, proposed_cipher, segment_length) == 0)
                return 1;

    }

    return 0;

}

int findPlaintextInDict1(char *plaintext)
{
    
    size_t plaintext_length;
    int i,j, offset;
    
    if (plaintext == NULL)
        return 0;
    
    plaintext_length = strlen(plaintext);
    
    if (plaintext_length < 1)
        return 0;
    
    offset = plaintext[0] - 'a';
    if (offset == -1)
        return 0;
    
    
    for (i = dictionary1_index[offset]; i < dictionary1_index_end[offset]; i++) {
        for (j = 0; j < dictionary1_longest; j++) {
            
            if (j == plaintext_length)
                return 1;
            
            if (DICTIONARY1[i][j] != plaintext[j])
                break;
            
            
        }
    }
    return 0;
    
}


int findPlaintextInDict2(char *plaintext)
{
    
    int i;
    size_t plaintext_length;
    char word_buffer[dictionary2_longest];
    
    if (plaintext == NULL)
        return 0;
    
    plaintext_length = strlen(plaintext);
    
    if (plaintext_length == 0)
        return 1;
    
    if (plaintext_length < dictionary2_shortest)
        return findWordInDict2(plaintext);
    
    for (i = dictionary2_longest; i > dictionary2_shortest; i--) {
        
        strncpy(word_buffer, plaintext, i);
        word_buffer[i] = '\0';
        
        if (findWordInDict2(word_buffer)) {
            return findPlaintextInDict2(plaintext + i);
        }
    }
    return 0;
}

int findWordInDict2(char *plaintext)
{
    
    size_t wordlength;
    int i, offset;
    int j;
    
    if (plaintext == NULL)
        return 0;
    
    wordlength = strlen(plaintext);
    
    
    if (wordlength < 1)
        return 0;
    
    offset = plaintext[0] - 'a';
    
    if (offset == -1)
        return 0;
    
    for (i = D2_ALPHA_INDEX[offset]; i < D2_ALPHA_INDEX[offset+1]; i++) {
        for (j = 0; j < wordlength; j++) {
            if (DICTIONARY2[i][j] != plaintext[j])
                break;
            else if (j == wordlength - 1)
                return 1;
        }
    }
    
    return 0;
    
}

char *breakCipherDict1(int keylength, char *ciphertext)
{

    char *plaintext = NULL;
    char plaintext_buffer[fixed_buffer_size + 1];
    char ciphertext_buffer[fixed_buffer_size + 1];
    char *key;
    int i=0;

    if (keylength < 0 || keylength > 40)
        return NULL;

    if (ciphertext == NULL)
        return NULL;

    ciphertext_buffer[fixed_buffer_size] = '\0';
    plaintext_buffer[fixed_buffer_size] = '\0';

    do{
        /* by doing this, we obtain a piece of the key */
        strncpy(ciphertext_buffer, ciphertext, fixed_buffer_size);
        strncpy(plaintext_buffer, DICTIONARY1[i], fixed_buffer_size);
        key = subtractBuffers(ciphertext_buffer, plaintext_buffer);

        /* we obtain another piece of the plaintext with our guessed key... */
        strncpy(ciphertext_buffer, ciphertext+keylength, fixed_buffer_size);
        plaintext = subtractBuffers(ciphertext_buffer, key);

        if (verify_segment_in_dict1(plaintext, keylength)) {
            delete plaintext;
            delete key;
            plaintext = new char[strlen(ciphertext) + 1];
            strncpy(plaintext, DICTIONARY1[i], dictionary1_longest + 1);
            break;
            
        } else {
            delete plaintext;
            delete key;
            plaintext = NULL;
            i++;
        }
    }while(i < dictionary1_length);

    return plaintext;
}

char *breakCipherDict2(int keylength,
        char *ciphertext)
{
    
    int offset = 0;
    int result;
    char *plaintext;
    char *plaintext_buffer, *shift_buffer, *ciphertext_buffer;
    int i;
    char actual_offset;

    if (ciphertext == NULL)
        return NULL;

	plaintext = new char[strlen(ciphertext) + 1];
	plaintext_buffer = new char[keylength + 1];
	ciphertext_buffer = new char[keylength + 1];

    result = breakAttemptDict2(ciphertext, keylength, offset,
           plaintext); 

    if (result == 0) {
        delete plaintext;
        delete ciphertext_buffer;
        delete plaintext_buffer;
        return NULL;
    } else{

        /* we obtain our shift key and propagate it to the rest of the ciphertext */
        strncpy(plaintext_buffer, plaintext, keylength);
        strncpy(ciphertext_buffer, ciphertext, keylength);
        plaintext_buffer[keylength] = '\0';
        ciphertext_buffer[keylength] = '\0';
        shift_buffer = subtractBuffers(ciphertext_buffer, plaintext_buffer);

        /* we apply the shift key here to the rest of the ciphertext */ 
        for (i = 2 * keylength; i < strlen(ciphertext); i++) {
            actual_offset = shift_buffer[i%keylength] - 'a' + 1; 
            plaintext[i] = ciphertext[i] - actual_offset;
            if (plaintext[i] < 'a')
               plaintext[i] += ('z' - 'a' + 1); 

        }

        /* wrap up, we are done... */
        delete shift_buffer;
        delete plaintext_buffer;
        delete ciphertext_buffer;
        plaintext[i] = '\0';

        return plaintext;

    }

}

int breakAttemptDict2(char *ciphertext, int keylength,
        int offset, char *plaintext)
{

    int i;
    char word_buffer[dictionary2_longest+1];
    char *shift_buffer, *threeLetterPart;
    char verification_buffer[201];
    size_t wordlen;
    int result;

    if (offset >= keylength)
        return 1;

    for (i = 0; i < dictionary2_length; i++) {

        // gets portion of the key from cipher
        wordlen = strlen(DICTIONARY2[i]);
        strncpy(word_buffer, ciphertext + offset, wordlen);
        word_buffer[wordlen] = '\0';
        shift_buffer = subtractBuffers(word_buffer, DICTIONARY2[i]);

        strncpy(word_buffer, ciphertext + offset + keylength, wordlen);
        threeLetterPart = subtractBuffers(word_buffer, shift_buffer);

        if (verifyThreeLetterSegment(threeLetterPart) == 0) {
            
            delete threeLetterPart;
            delete shift_buffer;
            continue; 

        } else {
            
            if (offset + wordlen > keylength)
                wordlen = keylength - offset + 1;

            strncpy(plaintext + offset, DICTIONARY2[i], wordlen);
            strncpy(plaintext + offset + keylength, threeLetterPart, wordlen);
            delete shift_buffer;
            delete threeLetterPart;

            result = breakAttemptDict2(ciphertext, keylength, offset + wordlen, plaintext);
            plaintext[2 * keylength] = '\0';

            strncpy(verification_buffer, plaintext + keylength, 201);
            if (verifyThreeLetterSegment(verification_buffer) > 0 && result == 1)
                return 1;

        }
    }
    return 0;
}

int verifyPlaintext(char *plaintext)
{
    int result;

    result = findPlaintextInDict1(plaintext);
    if (!result)
        result += findPlaintextInDict2(plaintext);

    return result;
}


char *subtractBuffers(char *buffer1, char *buffer2)
{
    
    char *result;
    char startover = 'z' - 'a' + 1;
    char temp;
    int i;

    if (buffer1 == NULL || buffer2 == NULL)
        return NULL;
    if (strlen(buffer1) != strlen(buffer2))
       return NULL;
	result = new char[strlen(buffer1) + 1];
    if (result == NULL)
        return NULL;
    for (i = 0; i < strlen(buffer1); i++) {
        if (!islower(buffer1[i]) || !islower(buffer2[i])){
            delete result;
            return NULL; 
        }
        temp = buffer2[i] - 'a' + 1;
        result[i] = buffer1[i] - temp;
        if (!islower(result[i]))
            result[i] += startover;
    }
    result[i] = '\0';
    return result;
}

int isValidThreeLetter(char *threeLetter)
{
    int i;
    int offset;
    
    if (threeLetter == NULL)
        return 0;
    if (strlen(threeLetter) != 3)
        return 0;
    offset = threeLetter[0] - 'a';
    if (threeLetterOffset[offset] == -1)
        return 0;
    for (i = threeLetterOffset[offset]; i <= threeLetterOffsetEnd[offset]; i++)
        if (threeLetter[1] == threeLetterList[i][1] && threeLetter[2] == threeLetterList[i][2])
           return 1;
    return 0;
}


int verifyThreeLetterSegment(char *segment) 
{

    size_t length;
    char threeLetter[4];
    int i;
    if (segment == NULL)
        return 0;
    length = strlen(segment);
    if (length < 3)
        return 0;
    threeLetter[3] = '\0';
    for (i = 0; i < length -2; i++){
        threeLetter[0] = segment[i];
        threeLetter[1] = segment[i+1];
        threeLetter[2] = segment[i+2];
        if(isValidThreeLetter(threeLetter) == 0){
            return 0;
        }
    }
    return 1;
}



