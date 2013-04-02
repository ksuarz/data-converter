/* Use this to represent the different base systems. */
typedef int Base;

/* Help/Errors */
void showHelp(void);
void printerr(char*);

/* Helper Methods */
int isValid(char, Base);
int getBaseValue(Base);
int getVal(char, Base);
char getChar(int, Base);
float power(int, int);
Base convertFlag(char);
int countDigits(char* str);

/* Conversion */
void convert(char*, Base, Base);
float strToFloat(char*, Base);
void intToStr(int, Base);

/* Constants */
static const Base HEXADECIMAL = 0;
static const Base DECIMAL = 1;
static const Base OCTAL = 2;
static const Base BINARY = 3;

static const char* VALS[] = {"0123456789ABCDEF", "0123456789", "01234567", "01"};
