/*
 * Autor: Adam Navarro Megías
 */

#include <ctype.h>
#include "calculadora.h"

// El indice de las operaciones va igual que lo definido
// como número de operacion en el archivo .x
// Se controla automático, no necesita indicarlo el usuario

void
calcprog_1(char *host, int operation, nums *list)
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
	case 1: result_int = sum_1(list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sum call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 2: result_int = sub_1(list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sub call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 3: result_int = mult_1(list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "mult call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	case 4: result_float = div_1(list, clnt);
			if (result_float == (float *) NULL) {
				clnt_perror (clnt, "div call failed");
			}
			printf("Result: %f\n", *result_float);
			break;

	
	default:perror("not a valid operation index");
			exit(1);
	}

	free(list->nums_val);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


void
calcadvancedprog_1(char *host, int operation, oper *oper, char *string)
{
	/* Se definen las siguientes estructuras, pero no se usarán todas, dependerá de el procedimiento*/
	CLIENT *clnt;
	int  *result_int;
	float *result_float;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCADVANCEDPROG, CALCADVANVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operation)
	{
	case 1: // SUFIX
			result_float = sufix_1(&string, clnt); 
			if (result_float == (int *) NULL) {
				clnt_perror (clnt, "sufix call failed");
			}
			printf("Result: %f\n", *result_float);
			break;

	case 2: // POWER
			result_int = power_1(oper, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "pow call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	case 3: // SQRT
			result_float = sqrt_1(oper, clnt); 
			if (result_float == (int *) NULL) {
				clnt_perror (clnt, "sqrt call failed");
			}
			printf("Result: %f\n", *result_float);
			break;

	case 4: // MOD
			printf("El primer valor es: %f y el segundo %f", oper->base, oper->operator);
			result_int = mod_1(oper, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "mod call failed");
			}
			printf("Result mod : %d\n", *result_int);
			break;

	case 5: // GCD
			result_int = gcd_1(oper, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "gcd call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	case 6: // FIBONACCI		
			int value = (int)oper->base;
			int * value_pointer = &value;
			*value_pointer = value;

			result_int = fibonacci_1(value_pointer, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "fibonacci call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	default:perror("not a valid operation index");
			exit(1);
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}



void
calcvectormatrixprog_1(char *host, int operation, matrix_group *mg, matrix *m)
{
	/* Se definen las siguientes estructuras, pero no se usarán todas, dependerá de el procedimiento*/
	CLIENT *clnt;
	matrix *result;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCVECTORMATRIXPROG, CALCVMVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operation)
	{
	case 1: break;
	case 2: break;
	case 3: break;
	case 4: break;
	case 5: break;

	case 6: result = sum_matrix_1(mg, clnt);
			break;
	case 7: result = sub_matrix_1(mg, clnt);
			break;
	case 8: result = mul_matrix_1(mg, clnt);
			break;

	default:perror("not a valid operation index");
			exit(1);
	}

	printf("Matriz resultante: \n\n");
	for (int i = 0; i < result->matrix_len; i++){
		for (int j = 0; j < result->matrix_val[0].nums_len; j++){
			printf("%d ", result->matrix_val[i].nums_val[j]);
		}
		printf("\n");
	}

	xdr_free(xdr_nums, result->matrix_val);

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
			printf ("Operaciones disponibles:\n");
			printf ("  sufix : Conjunto de operaciones básicas en la misma ejecución, pero en  sufijo\n");
			printf ("  ^ : Una base elevada a un exponente (multiplicada por si misma tantas veces)\n");
			printf ("  sqrt : La raíz con exponente indicado (si es raíz cuadrada es el número^2 que da la base)\n");
			printf ("  mod : Operación módulo, que devuelve el resto entero de la división\n");
			printf ("  gcd : Máximo común divisor de dos números\n");
			printf ("  fibonacci : El número \"n\" de la secuencia de fibonacci\n");
			exit (1);
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
			printf ("Operaciones disponibles:\n");
			printf ("  + : Suma de dos matrices de igual dimensión\n");
			printf ("  - : Resta de dos matrices de igual dimensión\n");
			printf ("  * : Multiplicación de dos matrices compatibles\n");
			exit (1);
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
	static int has_decimal_point = 0;
	// str comienza al principio de la cadena (la cadena será idealmente un número)
    // El número puede ser negativo, se salta el signo
    if (*str == '-')
        str++;    
	if (*str == '\0') { // No puede estar vacía despues de saltar el signo
		return 0;
	}	
	
    // Bucle recursivo para verificar si cada carácter es dígito
    while (*str) {
		if (*str == '.' && has_decimal_point) {
			return 0; // Tendría más de un punto
		}
		else 
		if (*str == '.') 
			has_decimal_point = 1;
		else
        if (!isdigit((unsigned char)*str)) 
            return 0; // No es un número

        str++;
    }

    return 1; // Es un número válido
}


// Función recursiva par a verificar si se han introducido dígitos o hay caracteres, solo verifica int
int is_number_int(const char *str) {
	// str comienza al principio de la cadena (la cadena será idealmente un número)
    // El número puede ser negativo, se salta el signo
    if (*str == '-')
        str++;    
	if (*str == '\0') { // No puede estar vacía despues de saltar el signo
		return 0;
	}	
	if (!str)
		return 1; // Es nulo

    // Bucle recursivo para verificar si cada carácter es dígito
    while (*str) {
        if (!isdigit((unsigned char)*str)) 
            return 0; // No es un número
        str++;
    }

    return 1; // Es un número válido
}


// Función para parsear datos para la calculadora avanzada
void parse_data_advancecalc(char *token1, char *token2, oper *oper)
{
	if (token1==NULL || token2==NULL)
	{
		printf("Not enough operands!\n");
		exit(1);
	}
	else
	if (strtok(NULL, " ") != NULL)
	{
		printf("Too many operands!\n");
		exit(1);
	}

	char *endptr; // Puntero en el que strtof pondrá el último carácter convertido, que debería de ser \0
	oper->base = strtof (token1, &endptr);
	if (*endptr != '\0') 
	{
		printf("Error: strtof(1) failed\n");
		exit(1);
	}
	oper->operator = strtof (token2, &endptr);
	if (*endptr != '\0') 
	{
		printf("Error: strtof(2) failed\n");
		exit(1);
	}
	if (oper->operator < 0 || !is_number(token1) || !is_number(token2)) {
		printf("Error: not a number or negative exponent: %f ^ %f\n", oper->base, oper->operator);
		exit(1);
	}
}


int
main (int argc, char *argv[])
{
	char *host;

	/* Definición de variables y estructuras */
	
	nums list = {0, NULL};
	oper oper = {0.0, 0.0};
	char *string = NULL;

	matrix_group matrix_g;
	matrix_g.m1.matrix_len = 0;
	matrix_g.m1.matrix_val = NULL;
	matrix_g.m2.matrix_len = 0;
	matrix_g.m2.matrix_len = NULL;

	matrix matrix;
	matrix.matrix_len = NULL;
	matrix.matrix_val = NULL;


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
	printf ("Please input the mode to use: basic, advanced, *file-NOT_IMPLEMENTED*, *vector-NOT_IMPLEMENTED*, matrix:\n");
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
							break;
		case '*': case '+': break;
		default:printf ("not a valid operation\n");
				exit (1);
		}
		
	
		switch (operation)
		{
			case '+': calcprog_1 (host, 1,&list);
					break;
			case '-': calcprog_1 (host, 2,&list);
					break;
			case '*': calcprog_1 (host, 3,&list);
					break;
			case '/': calcprog_1 (host, 4,&list);
					break;
		}

		
		break; // Termina el modo basico------------------------------------------------------------
		}

	case 'a':
		{
		if (strcmp(mode, "advanced") != 0) {
			perror("not a valid mode");
			exit(1);
		}
		printf("Options:\n");
		printf("^ base exponent -> power\n");
		printf("sqrt base root_exponent -> square root\n");
		printf("sufix a b c * + D + -> the characters are numbers, this is an operation in sufix\n");
		printf("mod divider dividend -> Module operation (gives the remainder)\n");
		printf("gdc a b -> Greatest common divisor between \"a\" and \"b\"\n");
		printf("fibonacci n -> Fibonacci number at position \"n\"\n");

		printf ("\nInput your choice. (Only one):\n");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0'; // Eliminamos el salto de línea, que luego da problemas

		// Extraemos la operación
		char *token = strtok(input, " ");
	

		if (strcmp(token, "sufix") == 0 ) // Modo avanzado para cadena sufija
		{	
			token = strtok(NULL, "\0");
			// Llamar al programa con &input como string (debería de tener el resto de la cadena)
			// Será el servidor el encargado de verificar que está correcta
			calcadvancedprog_1(host, 1, &oper, token);
		}	
		else
		if (strcmp(token, "fibonacci") == 0 )
		{
			token = strtok(NULL, " ");
			if (!is_number(token)) {
				printf("Error: not a number\n");
				exit(1);
			}
			else
			{
				char *endptr; // Puntero en el que strtof pondrá el último carácter convertido, que debería de ser \0
				oper.base = strtof (token, &endptr);
				if (*endptr != '\0') 
				{
					printf("Error: strtof(0) failed\n");
					exit(1);
				} // Extraemos n de la cadena (no estamos verificando que haya más)
				calcadvancedprog_1 (host, 6, &oper, input); // Llamar al procedimiento con n
			}
		}
		else
		{
			char * token1 = strtok(NULL, " ");
			char * token2 = strtok(NULL, " ");
			switch (token[0])
			{
			case '^': 	if (strlen(token) == 1){
							parse_data_advancecalc(token1, token2, &oper);
							calcadvancedprog_1 (host, 2, &oper, input);
						}		
						else
							printf ("not a valid operation\n");
						break;
			case 's': 	if (strcmp(token, "sqrt") == 0){
							parse_data_advancecalc(token1, token2, &oper);
							calcadvancedprog_1 (host, 3, &oper, input);
						}		
						else
							printf ("not a valid operation\n");
						break;
			case 'm': 	if (strcmp(token, "mod") == 0){
							parse_data_advancecalc(token1, token2, &oper);
							calcadvancedprog_1 (host, 4, &oper, input);
						}		
						else
							printf ("not a valid operation\n");
						break;
			case 'g': 	if (strcmp(token, "gdc") == 0){
							parse_data_advancecalc(token1, token2, &oper);
							calcadvancedprog_1 (host, 5, &oper, input);
						}		
						else
							printf ("not a valid operation\n");		
						break;				
			default:
				break;
			}
		}
		break; // Termina el modo avanzado----------------------------------------------------------
		}


	case 'f':
		if (strcmp(mode, "file") != 0) {
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
		{
		if (strcmp(mode, "matrix") != 0) {
			perror("not a valid mode");
			exit(1);
		}

		// Solicitamos la operacion
		printf("Press enter (\\n)");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {
			// Descartar el carácter
		}	

		char operation;
		printf ("Input the operation(- + *):\n");
		scanf ("%c", &operation); // Lee un máximo de 1 caracter y pone el último como \0
		
		while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') { // Se consumen palabras hasta ver si hay algo más que espacios
            printf("Error: Se ingresaron más de una palabra.\n");
            return 1;
        }
    }
		// Comprobamos la validez de la operación
		switch (operation)
		{
		case '+': case '-': case '*': case '/':
			break;
		default:
			perror("not a valid operation symbol (only +, -, *, /)");
			exit(1);
		}
		
		char * token;
		int row_m1 		= 0;
		int column_m1 	= 0;
		int row_m2 	 	= 0;
		int column_m2 	= 0;
		

		// Recibimos por entrada los datos de las matrices
		printf("Input the number of rows and columns of both matrixes -> row_m1 column_m1 row_m2 column_m2: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0'; // Eliminamos el salto de línea, que luego da problemas


		if ( is_number_int(token=strtok(input, " ")) ){
			row_m1 = atoi(token);
			if ( is_number_int(token=strtok(NULL, " ")) ){
				column_m1 = atoi(token);
				if ( is_number_int(token=strtok(NULL, " ")) ){
					row_m2 = atoi(token);
					if ( is_number_int(token=strtok(NULL, " ")) ){
						column_m2 = atoi(token);
					}
					else{
						printf("Invalid matrix size numbers");
						exit(1);
					}
				}
				else{
					printf("Invalid matrix size numbers");
					exit(1);
				}
			}
			else{
				printf("Invalid matrix size numbers");
				exit(1);
			}
		}
		else{
			printf("Invalid matrix size numbers");
			exit(1);
		}
		
		// Reservamos la memoria para ambas matrices
		matrix_g.m1.matrix_len = row_m1;
		matrix_g.m1.matrix_val = (nums *)malloc(row_m1 * sizeof(nums));
		for (int i = 0; i < row_m1; i++) {
			matrix_g.m1.matrix_val[i].nums_len = column_m1;
			matrix_g.m1.matrix_val[i].nums_val = (int *)malloc(column_m1 * sizeof(int));
		}

		matrix_g.m2.matrix_len = row_m2;
		matrix_g.m2.matrix_val = (nums *)malloc(row_m2 * sizeof(nums));
		for (int i = 0; i < row_m2; i++) {
			matrix_g.m2.matrix_val[i].nums_len = column_m2;
			matrix_g.m2.matrix_val[i].nums_val = (int *)malloc(column_m2 * sizeof(int));
		}


		// Introducimos los datos a las matrices por la consola
		printf("Input the first matrix (one line for each row): \n");

		for (int i = 0; i < matrix_g.m1.matrix_len; i++) {
			fgets(input, sizeof(input), stdin);
			input[strcspn(input, "\n")] = '\0';
	
			token = strtok(input, " ");
			for (int j = 0; j < matrix_g.m1.matrix_val[i].nums_len; j++) {
				if (token == NULL) {
					printf("Not enough numbers!\n");
					exit(1);
				}
				if (!is_number_int(token)) {
					printf("Not a number!\n");
					exit(1);
				}
				matrix_g.m1.matrix_val[i].nums_val[j] = atoi(token);
				token = strtok(NULL, " ");
			}
	
			if (token != NULL) {
				printf("More numbers than expected!\n");
				exit(1);
			}
		}

		printf("\n\n");

		
		printf("Input the second matrix (one line for each row): \n");

		for (int i = 0; i < matrix_g.m2.matrix_len; i++) {
			fgets(input, sizeof(input), stdin);
			input[strcspn(input, "\n")] = '\0';
	
			token = strtok(input, " ");
			for (int j = 0; j < matrix_g.m2.matrix_val[i].nums_len; j++) {
				if (token == NULL) {
					printf("Not enough numbers!\n");
					exit(1);
				}
				if (!is_number_int(token)) {
					printf("Not a number!\n");
					exit(1);
				}
				matrix_g.m2.matrix_val[i].nums_val[j] = atoi(token);
				token = strtok(NULL, " ");
			}
	
			if (token != NULL) {
				printf("More numbers than expected!\n");
				exit(1);
			}
		}

		printf("\n\n");

		printf("\n\n");
		
		switch (operation)
		{
			case '+': 
					calcvectormatrixprog_1(host, 6, &matrix_g, &matrix);
					break;
			case '-': 
					calcvectormatrixprog_1 (host, 7, &matrix_g, &matrix);
					break;
			case '*': 
					calcvectormatrixprog_1 (host, 8, &matrix_g, &matrix);
					break;
		}
		
		break;	
		} // Termina el modo matrix-----------------------------------------------------------------

	default:
		perror("not a valid mode");
		exit(1);
	}


exit (0);
}
