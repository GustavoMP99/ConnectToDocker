#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <comip.h>
#include <stdbool.h>//to add booleans
//There is a test file, to open write: test.txt

/*
 * Primer proyecto programado de Lenguajes de programación G51
 * Gustavo Méndez Poveda
 * Ultima modificación: 4/5/2020
 */

typedef enum {HOME,CEL, SPECIAL} Type;


struct specialPhone{
    char* sphoneNumber;
};
/**
 * The basic for the house number phone
 */
struct homePhone{
    char* hphoneNumber;
};

/**
 * The basic for the personal number phone
 */
struct celPhone{
    char* cphoneNumber;
};

/**
 * The struct for the list of phone numbers
 */
struct phoneList{
    void* phone;
    Type type;
    struct phoneList* sig;
} *head;

/**
 * Void to create the personal phone numbers
 * @param number1
 * @return
 */
void* newCelPhone(char* number1){
    struct celPhone* newCelPhone;
    newCelPhone= (struct celPhone*) malloc(sizeof(struct celPhone));
    newCelPhone->cphoneNumber = (char*) malloc(1);
    strcpy(newCelPhone->cphoneNumber,number1);

    return newCelPhone;
}
/**
 * Void to create the house phone numbers
 * @param number1
 * @return
 */
void* newHomePhone(char* number1){
    struct homePhone* newHomePhone;
    newHomePhone= (struct homePhone*) malloc(sizeof(struct homePhone));
    newHomePhone->hphoneNumber = (char*) malloc(1);
    strcpy(newHomePhone->hphoneNumber,number1);

    return newHomePhone;
}

/**
 * Void to create the special phone numbers
 * @param number1
 * @return
 */
void* newSpecialPhone(char* number1){
    struct specialPhone* newSpecialPhone;
    newSpecialPhone= (struct specialPhone*) malloc(sizeof(struct specialPhone));
    newSpecialPhone->sphoneNumber = (char*) malloc(1);
    strcpy(newSpecialPhone->sphoneNumber,number1);

    return newSpecialPhone;
}
/**
 * Void in charge of make the list of phone numbers and insert the rest of numbers
 * @param newPhone
 * @param type
 */
void insertNumber(void* newPhone, Type type){
    struct phoneList* nn = NULL;
    nn = (struct phoneList*) malloc(sizeof(struct phoneList));
    nn->phone= newPhone;
    nn->type= type;
    nn->sig=NULL;

    if (head == NULL)
        head=nn;
    else{
        nn->sig = head;
        head = nn;
    }
}


/**
 * Bool to compare a char array with a pattern
 * @param str
 * @param pattern
 * @return
 */
bool reg_matches( char *str,  char *pattern)
{
    regex_t re;
    int ret;

    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
        return false;

    ret = regexec(&re, str, (size_t) 0, NULL, 0);
    regfree(&re);

    if (ret == 0)
        return true;

    return false;
}

/**
 * Method to print the phones in the list
 */
void printPhones(){
    struct phoneList* temp = head;
    printf("********* Phone List *********\n");
    if (temp == NULL)
        printf("Empty List");
    else
        while(temp!=NULL){
            if (temp->type==0){
                printf("House Phone: %s\n",((struct homePhone*)temp->phone)->hphoneNumber);

            }
            else if (temp->type==1) {
                printf("Celphone: %s\n",((struct celPhone*)temp->phone)->cphoneNumber);

            }
            else{
                printf("Special phone: %s\n",((struct specialPhone*)temp->phone)->sphoneNumber);
            }
            temp = temp->sig;
        }
}

/**
 * Code in charge of search the phone numbers in the text
 * @param text
 */
void searchNumber(char* text){
    char * source = text;
    char * regexString = "(\\+506|00506|506)?[ -]*[ -]*([0-9][ -]*){8}";//Pattern
    size_t maxMatches = 1000;
    size_t maxGroups = 1;

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    unsigned int m;
    char * cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        return;
    }

    cursor = source;
    for (m = 0; m < maxMatches; m ++) {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break;
        unsigned int g;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t) -1)
                break;
            if (g == 0)
                offset = groupArray[g].rm_eo;
            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;

            char *temp = (char *) malloc(1);
            strcpy(temp, cursorCopy + groupArray[g].rm_so);
            if (reg_matches(temp, "(\\+506|00506|506)?[ -]*[ -]*[2]([0-9][ -]*){7}")) {//check the type of the phone
                insertNumber(newHomePhone(temp), HOME);
            } else {
                insertNumber(newCelPhone(temp), CEL);
            }
        }
        cursor += offset;
    }
    regfree(&regexCompiled);
}

void specialNumbers(char* text){
    char * source = text;
    char * regexString = "(\\+506|00506|506)?[ -]*[ -]*([0-9][ -]*){3}$";//Pattern
    size_t maxMatches = 1000;
    size_t maxGroups = 1;

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    unsigned int m;
    char * cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        return;
    }

    cursor = source;
    for (m = 0; m < maxMatches; m ++) {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break;
        unsigned int g;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t) -1)
                break;
            if (g == 0)
                offset = groupArray[g].rm_eo;
            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;

            char *temp = (char *) malloc(1);
            strcpy(temp, cursorCopy + groupArray[g].rm_so);
            insertNumber(newSpecialPhone(temp), SPECIAL);

        }
        cursor += offset;
    }
    printPhones();//print the phones before the program clean the memory
    regfree(&regexCompiled);
}

/**
 * Code to open the txt. file
 * @return
 */
char *txt(){
    FILE *fp;
    long lSize;
    char *buffer;

    char *fAdress=NULL;
    fAdress = (char*) malloc(1);
    fflush(stdin);
    printf("Type the file adress:");// format: "c:\\temp\\test.txt"
    scanf("%[^\n]", fAdress);

    fp = fopen ( fAdress , "rb" );
    if( !fp )
        perror(fAdress), exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

/* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer )
        fclose(fp),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    searchNumber(buffer);//search the  numbers in the text
    specialNumbers(buffer);
    fclose(fp);//close the file
    free(buffer);//free the space
    return buffer;
}



/**
 * Project main
 * @return
 */
int main() {
    txt();//
    return 0;
}
