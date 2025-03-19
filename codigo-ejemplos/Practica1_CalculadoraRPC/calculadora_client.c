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
	nums  		num_list = list;
	operands  	operand_args = op;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCSIMPLEVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operation)
	{
	case 1: result_int = sum_1(&num_list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sum call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 2: result_int = sub_1(&operand_args, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "sub call failed");
			}
			printf("Result: %d\n", *result_int);
			break;
	case 3: result_int = mult_1(&num_list, clnt);
			if (result_int == (int *) NULL) {
				clnt_perror (clnt, "mult call failed");
			}
			printf("Result: %d\n", *result_int);
			break;

	case 4: result_float = div_1(&operand_args, clnt);
			if (result_float == (float *) NULL) {
				clnt_perror (clnt, "div call failed");
			}
			printf("Result: %f\n", *result_float);
			break;
	
	default:perror("not a valid operation index");
			exit(1);
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	operands op;
	nums list;

	
	if (argc < 2) {
		printf ("Uso: %s server_host\n", argv[0]);
		printf ("use: %s server_host -h, to see all modes", argv[0]);
		printf ("use: %s server_host mode -h, to see how it works", argv[0]);
		exit(1);
	}


	if (argc == 2 && strcmp(argv[1], "-h") == 0)
	{
		printf ("Operaciones disponibles:\n");
		printf ("  + : Suma de múltiples operandos\n");
		printf ("  - : Resta de dos operandos\n");
		printf ("  \\* : Multiplicación de múltiples operandos\n");
		printf ("  / : División de dos operandos\n");
		printf ("\nEjemplos:\n");
		printf ("  %s localhost + 1 2 3 4\n", argv[0]);
		printf ("  %s localhost - 5 2\n", argv[0]);
		printf ("  %s localhost \\* 5 4 3 2 1\n", argv[0]);
		printf ("  %s localhost + 1 2 3 4\n", argv[0]);
		exit (1);
	}
	else
	if (argc == 3 && strcmp(argv[1], "-h") == 0 && strcmp(argv[2], "basic") == 0)
	{
		printf ("Operaciones disponibles para modo básico:\n");
		printf ("  + : Suma de múltiples operandos\n");
		printf ("  - : Resta de dos operandos\n");
		printf ("  \\* : Multiplicación de múltiples operandos\n");
		printf ("  / : División de dos operandos\n");
		printf ("\nEjemplos:\n");
		printf ("+ 1 2 3 4\n", argv[0]);
		printf ("- 5 2\n", argv[0]);
		printf ("\\* 5 4 3 2 1\n", argv[0]);
		printf ("+ 1 2 3 4\n", argv[0]);
		exit (1);
	}
	else
	
	
	
	host = argv[1];
	char *operation = argv[2];
	if (strlen(operation) != 1) {
		printf ("operation must be a single character: %s, tamaño: %d\n", operation, strlen(operation));
		exit (1);
	}

	/*
	 * Como usan distintas estructuras para la división/resta y suma/multiplicación
	 * se debe hacer un switch para asignar los valores a las estructuras correspondientes
	 */
	switch (operation[0])
	{
	// Para la resta y división simplemente asignamos los dos operandos controlando que solo hay 2
	case '-': case '/':	if (argc > 5) {
							printf ("usage: %s server_host ('-' or '/')", argv[0]);
							printf (" operand1 operand2. Error: too many operands!\n");
							exit (1);
						}
						op.op1 = atoi(argv[3]);
						op.op2 = atoi(argv[4]);
						break;
	// Para la suma y multiplicación asignamos memoria para el listado controlando la cantidad de operandos
	case '*': case '+': list.nums_len = argc - 3;
						list.nums_val = (int *) malloc (list.nums_len * sizeof(int));
						for (int i = 0; i < list.nums_len; i++) {
							list.nums_val[i] = atoi(argv[i + 3]);
						}
						break;
	default:printf ("not a valid operation\n");
			exit (1);
	}

	switch (operation[0])
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
exit (0);
}
