/***
 *  Funciones para parsear String escritas y probadas en C para Linux
 * 
 * 
 * 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PARSE_CHAR                              ","
#define PARSE_ERROR                             (0)
#define CMD_FORMAT                             ("tag : value")
#define EXAMPLE_CMD                            "+SMSUB: \"CMD\",\"7\"␍␊"


// Value sera un valor numerico del 0 al 9

/***
 *              Values Table:
 *                      0           Error en parse
 *                      1           1  minuto
 *                      3           5  minutos
 *                      3           10 minutos
 *                      4           30 minutos
 *                      5           60 miuntos
 *                      6           Calibrar el sensor
 *                      6           x
 *                      7           x
 *                      8           x
 *                      9           x
 * 
 *       
*/






// SI error retorna 0
static int get_parse(char* string){

    int ret = PARSE_ERROR;

    printf("Dummy function:\r\n%s\r\n",string);

    char* token = strtok(string,PARSE_CHAR); // TAG
    printf("token: %s \r\n",token);
    token = strtok(NULL,PARSE_CHAR);           // VALUE
    token[0]= " ";
    token[2]=(char)0;
    printf("token: %s \r\n",&(token[1]));
    
    ret = atoi(&(token[1]));
    return ret;
}



char string[]=EXAMPLE_CMD;


int main(void){
    printf("Test parse \r\n");
    int value = get_parse(string);

    printf("Value : %d \r\n",value);

    return 0;
}