/*
 * Autor: Adam Navarro Megías
 */


#include "calculadora.h"

// El indice de las operaciones va igual que lo definido
// como número de operacion en el archivo .x
// Se controla automático, no necesita indicarlo el usuario

void
calcprog_1(char *host, int operation, operands op, nums list)
{
	/* Se definen las siguientes estructuras, pero no se usarán todas, dependerá de el procedimiento*/
	CLIENT *clnt;
	int  *result_int;
	float *result_float;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCSIMPLEVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operation)
	{
	case 1: result_int = sum_1(&list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sum call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 2: result_int = sub_1(&op, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sub call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 3: result_int = mult_1(&list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "mult call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	case 4: result_float = div_1(&op, clnt);
			if (result_float == (float *) NULL) {
				clnt_perror (clnt, "div call failed");
			}
			printf("Result: %f\n", *result_float);
			break;

	
	default:perror("not a valid operation index");
			exit(1);
	}

	free(list.nums_val);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int 
check_help(char* arg1, char* arg2)
{
	if (strcmp(arg1, "help") == 0 || strcmp(arg1, "-h") == 0)
	{
		if (arg2 == NULL)
		{
			printf ("Use: program server_host\n");
			printf ("Modes: basic(operations +, -, *, /)\n       advanced(operations ^, sqrt, log, exp)\n");
			printf ("       sufix(mix basic operations in one string with sufix expression)\n       file(Use a file with a lot of operations)\n");
			printf ("       vector(operations +, -, *)\n       matrix(operations +, -, *)\n");
			printf ("Use: program server_host mode -h, to see how each model works\n");
			exit(1);
		}
		else
		if (strcmp(arg2, "basic") == 0)
		{
			printf ("Operaciones disponibles:\n");
			printf ("  + : Suma de múltiples operandos\n");
			printf ("  - : Resta de dos operandos\n");
			printf ("  * : Multiplicación de múltiples operandos\n");
			printf ("  / : División de dos operandos\n");
			printf ("\nEjemplos de sentencias:\n");
			printf ("+ 1 2 3 4\n");
			printf ("- 5 2\n");
			printf ("* 5 4 3 2 1\n");
			printf ("/ 6 2\n");
			exit (1);
		}
		else
		if (strcmp(arg2, "advanced"))
		{
			printf ("Not implemented:\n");
		}
		else
		if (strcmp(arg2, "sufix"))
		{
			printf ("Not implemented:\n");
		}	
		else
		if (strcmp(arg2, "file"))
		{
			printf ("Not implemented:\n");
		}
		else
		if (strcmp(arg2, "vector"))
		{
			printf ("Not implemented:\n");
		}
		else
		if (strcmp(arg2, "matrix"))
		{
			printf ("Not implemented:\n");
		}
		else
		{
			printf ("not a valid mode\n");
		}
		return 0; // Se ha introducido help
	}
	return 1; // No se ha introducido help
}

// Función recursiva par a verificar si se han introducido dígitos o hay caracteres
int is_number(const char *str) {

	// str comienza al principio de la cadena (la cadena será idealmente un número)
    // El número puede ser negativo, se salta el signo
    if (*str == '-') 
        str++;    

    // Bucle recursivo para verificar si cada carácter es dígito
    while (*str) {
        if (!isdigit(*str)) 
            return 0; // No es un número

        str++;
    }

    return 1; // Es un número válido
}


int
main (int argc, char *argv[])
{
	char *host;

	/* Definición de variables y estructuras */
	
	operands op = {0, 0};
	nums list = {0, NULL};
	
	if (argc < 2) {
		printf ("Use: %s server_host\n", argv[0]);
		printf ("Use: %s server_host -h, to see all modes\n", argv[0]);
		printf ("Use: %s server_host mode -h, to see how it works\n", argv[0]);
		exit(1);
	}
	
	if (argc == 3){
		int help = check_help(argv[2], NULL);
		if (help == 1)
		{
			printf ("Use: %s server_host\n", argv[0]);
			printf ("Use: %s server_host -h, to see all modes\n", argv[0]);
			printf ("Use: %s server_host mode -h, to see how it works\n", argv[0]);
			exit(1);
		}
			
		exit(1);	
	}		
	else
	if (argc == 4){
		int help = check_help(argv[3], argv[2]);
		if (help == 1)
		{
			printf ("Use: %s server_host\n", argv[0]);
			printf ("Use: %s server_host -h, to see all modes\n", argv[0]);
			printf ("Use: %s server_host mode -h, to see how it works\n", argv[0]);
			exit(1);
		}
		exit(1);
	}

	
	host = argv[1];

	// Leemos el modo de operación (solo lee la primera palabra)
	char *mode = (char *) malloc (15 * sizeof(char));
	printf ("Please input the mode to use: basic, advanced, sufix, file, vector, matrix:\n");
	scanf ("%14s", mode); // Lee un máximo de 14 caracteres y pone el último como \0

	// Verificamos que la entrada ha sido solo una palabra
	// Si por ejemplo hay un espacio o salto de linea o similar justo después de la palabra
	// no pasa nada. Nos sirve también para limpiar el buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') { // Se consumen palabras hasta ver si hay algo más que espacios
            printf("Error: Se ingresaron más de una palabra.\n");
            return 1;
        }
    }

	char input[100];
	// Usamos un switch para no tener que hacer una cadena de if enlazados
	switch (mode[0])
	{
	case 'b':
		{
		// Comprobamos que de verdad es el modo basic (solo hacemos un if y no 7)
		if (strcmp(mode, "basic") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		// Solicitamos la operacion seguida de los operandos
		printf ("Input the operation(- + / *) followed by the operands. (- && / accept 2 op,):\n");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0'; // Eliminamos el salto de línea, que luego da problemas

		// Extraemos la operación
		char *token = strtok(input, " ");
		if (strlen(token) != 1) {
			printf ("operation must be a single character: %s, tamaño: %ld\n", token, strlen(token));
			exit (1);
		}
		char operation = token[0];

		// Comprobamos la validez de la operación
		switch (operation)
		{
		case '+': case '-': case '*': case '/':
			break;
		default:
			perror("not a valid operation symbol (only +, -, *, /)");
			exit(1);
		}


		// list -> de números, struct definido en el .x
		list.nums_len = 0; // Reservamos memoria para 10 que es el máximo, si hay menos da igual
		list.nums_val = (int *) malloc (10 * sizeof(int));

		// Parseamos los tokens a operandos
		while ((token = strtok(NULL, " ")) != NULL) { // Extraer los operandos
			// Comprobamos aqui el límite de 10, porque en esta ejecución intentaría asignar 
			// nums_val[10] y daría segfault
			if (list.nums_len == 10) {
				printf("Too many operands, max 10\n");
				exit(1);
			}

			if (!is_number(token)) {
				printf("Found a non-number operand: %s\n", token);
				exit(1);
			}
			list.nums_val[list.nums_len++] = atoi(token);
		}

		// Comprobación de operandos general
		if (list.nums_len < 2) {
			printf ("Error: not enough operands!\n");
			exit (1);
		}

		// Comprobaciones finales para guardar datos en las estructuras pertinentes
		switch (operation)
		{
		// Para la resta y división simplemente asignamos los dos operandos controlando que solo hay 2
		case '-': case '/':	if (list.nums_len > 2) {
								printf ("Error: too many operands!\n");
								exit (1);
							}
							op.op1 = list.nums_val[0];
							op.op2 = list.nums_val[1];
							break;
		case '*': case '+': break;
		default:printf ("not a valid operation\n");
				exit (1);
		}
	
		switch (operation)
		{
			case '+': calcprog_1 (host, 1, op, list);
					break;
			case '-': calcprog_1 (host, 2, op, list);
					break;
			case '*': calcprog_1 (host, 3, op, list);
					break;
			case '/': calcprog_1 (host, 4, op, list);
					break;
		}

		break;
		}

	case 'a':
		if (strcmp(mode, "advanced") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf ("not implemented\n");
		exit (1);
		break;
	
	case 'f':
		if (strcmp(mode, "file") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf ("not implemented\n");
		exit (1);
		break;
	
	case 's':
		if (strcmp(mode, "sufix") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf ("not implemented\n");
		exit (1);
		break;	

	case 'v':
		if (strcmp(mode, "vector") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf ("not implemented\n");
		exit (1);
		break;	
	
	case 'm':
		if (strcmp(mode, "matrix") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf ("not implemented\n");
		exit (1);
		break;	

	default:
		perror("not a valid mode");
		exit(1);
	}

exit (0);
}
