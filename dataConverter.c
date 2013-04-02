#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dataConverter.h"

/**
 * Converts numbers in one base system to another.
 * @return 0 if the program terminates properly; 1 if an error occurs.
 */
int main(int argc, char* argv[]) {
    if(argc == 2) {
        if(argv[1][0] == '-' && argv[1][1] == '?') {
            showHelp();
            return 0;
        }
        else {
            printerr("Unrecognized flag.");
            return 1;
        }
    }

    /* More error conditions. */
    if(argc != 4) {
        printerr("Invalid syntax.");
        return 1;
    }
    if(argv[1][0] != '-' || argv[3][0] != '-') {
        printerr("Unrecognized flag.");
        return 1;
    }

    Base src, dest;
    int len = countDigits(argv[2]);
    src = convertFlag(argv[1][1]);
    if(src < 0) {
        return 1;
    }
    else if((src == HEXADECIMAL && len > 4) || len > 8) {
        /* Too many digits. */
        fprintf(stderr, "Max length is 8 digits (4 for hexadecimal).\n");
        return 1;
    }

    dest = convertFlag(argv[3][1]);
    if(dest < 0) {
        return 1;
    }
    /* Check for the trivial case. */
    if(src == dest) {
        printf("%s\n", argv[2]);
        return 0;
    }

    /* Perform the conversion. */
    convert(argv[2], src, dest);
    return 0;
}

/**
 * Counts the number of digits in a string, excluding the decimal point and
 * the terminating character.
 * @param str The string to count.
 * @return The number of digits.
 */
int countDigits(char* str) {
    int i, count;
    for(i = 0, count = 0; i < strlen(str); i++) {
    	char c = *(str + i);
    	if(c != '.' && c != '\0') {
	        count++;
	    }
    }
    return count;
}
/**
 * Prints the help message to the screen.
 */
void showHelp(void) {
    printf("Proper usage: dataConverter [flag] [value] [flag]\nAvailable Fla");
    printf("gs:\n-h\tHexadecimal\n-d\tDecimal\n-o\tOctal\n-b\tBinary\n");
    printf("First flag indicates the base of value; second flag indicates");
    printf(" the new base to convert to.\n");
}

/**
 * Print a message to standard error.
 * @param msg The message to display.
 */
void printerr(char* msg) {
    fprintf(stderr, "%s Type dataConverter -? for help.\n", msg);
}

/**
 * Converts a number from one base system to another.
 * @param str The bit string to convert.
 * @param src The base system of the source.
 * @param dest The base system of the answer.
 */
void convert(char* str, Base src, Base dest) {
    float val;
    val = strToFloat(str, src);

    /* First, convert the integer part of the value. */
    intToStr((int) val, dest);

    /* If there's no fractional part, let's stop. */
    if(val == (int) val) {
	    printf("\n");
	    return;
    }

    /* Now convert the fractional part. Print a decimal point. */
    printf(".");
    val -= (int) val;
    while(val != (int) val) {
	    val *= getBaseValue(dest);
    }
    intToStr((int) val, dest);
    printf("\n");
}

/**
 * Converts a number into a char string representing that number in a different
 * base system, then prints that string out to standard output.
 * @param val An integer value.
 * @param b A number base - either decimal, hexadecimal, binary, or octal.
 */
void intToStr(int val, Base b) {
    /* Maximum size: 4 hexadecimal bits, which is 16 binary bits */
    char array[17];
    int i;

    /* Let's clear the buffer and terminate it. */
    for(i = 0; i < 16; i++) {
	    *(array + i) = '0';
    }
    *(array + 16) = '\0';

    int base;
    base = getBaseValue(b);

    /* Run the conversion to string. */
    i = 15;
    while(val != 0) {
	    char c = getChar(val % base, b);
	    *(array + i) = c;
	    val /= base;
	    i--;
    }

    /* Delete the extra 0s at the beginning of the string. */
    char* result;
    for(i = 0; i < 17; i++) {
        if(*(array + i) != '0') {
            result = array + i;
            break;
        }
    }
    printf("%s", result);
}

/**
 * Converts a number in a given base system into the computer's natural number
 * format. 
 * @param str The value to convert. Must consist only of characters and the
 * decimal pount (.).
 * @param b The base that the number string is in.
 * @return The numerical value of the number string.
 */
float strToFloat(char* str, Base b) {
   /* Find the length and the location of the decimal point. */
    const size_t len = strlen(str);
    char* offset;
    offset = strchr(str, '.');

    int i, j, base;
    base = getBaseValue(b);

    /* Use this to hold the final answer. */
    float sum = 0.0f;

    /* If there is no decimal point, treat this like an integer. */
    i = (offset == NULL) ? (len - 1) : (offset - str - 1);

    /* Moves from the most significant bit to the least, converting each bit
       into its true value.*/
    for(j = 0; j < len; i--, j++) {
        char c = str[j];
        if(c == '.') {
            /* The integral part has ended and the fraction begins. */
            i++;
            continue;
        }
        sum += (power(base, i) * getVal(c, b));
    }

    return sum;
}


/**
 * Converts a character in a given base system into its true value.
 * @param c The char to convert.
 * @param b The base system of c.
 * @return The value of the given character, or -1 if the char is invalid
 * in the given base.
 */
int getVal(char c, Base b) {
    c = toupper(c);
    if(!isValid(c, b)) {
        fprintf(stderr, "Character '%c' is not valid in the given base.\n", c);
        return -1;
    }

    int x;
    if(c >= 'A') {
        x = c - 'A' + 10;
    }
    else {
        x = c - '0';
    }
    return x;
}

/**
 * @return The integer value's representation in the given base.
 */
char getChar(int val, Base b) {
    if(val > getBaseValue(b)) {
        fprintf(stderr, "This value is invalid in the given base.\n");
        return 0;
    }

    if(val >= 10) {
        return ('A' + (val - 10));
    }
    else {
        return ('0' + val);
    }
}

/**
 * Converts a flag into the proper base.
 * @param c The input flag to convert.
 */
Base convertFlag(char c) {
    c = tolower(c);
    if(c == 'd') {
        return DECIMAL;
    }
    else if(c == 'h') {
        return HEXADECIMAL;
    }
    else if(c == 'o') {
        return OCTAL;
    }
    else if(c == 'b') {
        return BINARY;
    }
    else {
        fprintf(stderr, "Flag must be -d, -h, -o, or -b.\n");
        return -1;
    }
}

/**
 * @param b A base system. Either binary, octal, decimal, or hexadecimal.
 * @return The numerical value of the base. Returns 0 if the base is
 * unrecognized.
 */
int getBaseValue(Base b) {
    if(b == BINARY) {
        return 2;
    }
    else if(b == OCTAL) {
        return 8;
    }
    else if(b == DECIMAL) {
        return 10;
    }
    else if(b == HEXADECIMAL) {
        return 16;
    }
    else {
        printerr("Invalid base.");
        return 0;
    }
}

/**
 * Precondition: The given base is valid.
 * @param c A single character.
 * @param base A numerical base: base 2, 8, 10, or 16.
 * @return 1 if the character is valid in the given base; 0 otherwise.
 */
int isValid(char c, Base base) {
    int i;
    c = toupper(c);
    for(i = 0; i < strlen(VALS[base]); i++) {
        if(c == VALS[base][i]) {
            return 1;
        }
    }

    return 0;
}

/**
 * Calculates integer powers. This function can handle both positive and
 * negative exponents.
 * @param base The the base of the operation.
 * @param exponent The exponent to raise the base to.
 * @return The value of base ^ exponent.
 */
float power(int base, int exponent) {
    if(exponent == 0) {
        return 1.0f;
    }

    int i;
    float product = 1.0f;

    if(exponent > 0) {
        for(i = 0; i < exponent; i++) {
            product *= (float)base;
        }
    }
    else {
        for(i = 0; i > exponent; i--) {
            product *= (1.0f / (float)base);
        }
    }

    return product;
}
