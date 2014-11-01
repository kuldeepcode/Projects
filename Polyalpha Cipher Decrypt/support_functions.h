#include <stdlib.h>

char *breakCipherDict1(int keylength, char *ciphertext);
char *breakCipherDict2(int keylength, char *ciphertext);
int breakAttemptDict2(char *ciphertext, int keylength, int offset, char *plaintext);
int verifyPlaintext(char *plaintext);
int findPlaintextInDict1(char *plaintext);
int findPlaintextInDict2(char *plaintext);
int findWordInDict2(char *plaintext);
char *subtractBuffers(char *buffer1, char *buffer2);
int isValidThreeLetter(char *threeLetter);
int verifyThreeLetterSegment(char *chunk);
