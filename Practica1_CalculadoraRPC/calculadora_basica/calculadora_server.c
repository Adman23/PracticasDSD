/*
 * Autor: Adam Navarro Megías
 */

#include <ctype.h>
#include "calculadora.h"
#include "../calculadora_avanzada/calculadora_avanzada.h"

#define MAX 10

/*-----------------------------SIMPLECALC------------------------------------------*/
int *
sum_1_svc(nums *list, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	for (int i = 0; i < list->nums_len; i++) {
		result += list->nums_val[i];
	}

	return &result;
}

int *
sub_1_svc(nums *list, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	result = list->nums_val[0] - list->nums_val[1];
	
	return &result;
}

int *
mult_1_svc(nums *list, struct svc_req *rqstp)
{
	static int  result;
	result = 1;


	for(int i = 0; i < list->nums_len; i++) {
		result *= list->nums_val[i];
	}

	return &result;
}

float *
div_1_svc(nums *list, struct svc_req *rqstp)
{
	static float  result;
	result = 0;

	if (list->nums_val[1] == 0) {
		printf("Error: División por cero\n");
		result = 0;
		return &result;
	}

	result = (float)list->nums_val[0] / (float)list->nums_val[1];

	return &result;
}
/*---------------------------------------------------------------------------------*/





/*------------------------------ADVANCALC------------------------------------------*/
// Estructura stack creada para las operaciones en sufijo, solo la usa el server
typedef struct stack{
    float elements[MAX];
    int top;
} stack;
void init_stack(stack *s){
	s->top = -1;
}
int is_empty(stack *s){
	return s->top == -1;
}
int push(stack *s, int element){
	if (s->top == MAX - 1){ 
		return -1;	// Si la pila está llena
	}
	s->elements[++s->top] = element;
	return 0;	// Si se ha insertado correctamente
}
float pop(stack *s){
	if (is_empty(s)){
		return 1;	// Si la pila está vacía
	}
	return s->elements[s->top--];
}
float top(stack *s){
	if (is_empty(s)){
		return 1;	// Si la pila está vacía
	}
	return s->elements[s->top];
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
        if (!isdigit(*str))
		{
            return 0; // No es un número
		}
        str++;
    }
	return 1; // Si es un numero
}

float *
sufix_1_svc(char **string, struct svc_req *rqstp)
{
	static float result;
	result = 0;
	stack s;
	init_stack(&s);
	char * endptr; // Puntero para strtof que apunta donde termina la cadena que transforma

	// Hacemos bucle extrayendo todos los tokens de la cadena
	// Cada número que se encuentre se manda a la pila, cada operación se hace con los dos números superiores
	// Si se da el caso de encontrar operación y no haber números en la pila suficientes --> Error
	// Si se da el caso de tener la pila con números y terminar la cadena --> Error
	// Si se da el caso de meter en la pila pero está llena --> Error (Este es error por la implementación, mayor tamaño daría margen a operaciones más grandes -> max 10)
	char *token = strtok(*string, " ");
	while (token != NULL)
	{
		if (is_number(token)){
			if (push(&s, strtof(token, &endptr)) == -1){
				printf("Error: Pila llena\n");
				result = -1;
				return &result;
			}
			if (*endptr != '\0')
			{
				printf("Error: strtof(server) failed\n");
				result = -1;
				return &result;
			}
		}
		else{
			if (s.top >= 1)
			{
				int op2 = pop(&s);
				int op1 = pop(&s);
				switch (token[0]){
					case '+': push(&s, op1 + op2); break;
					case '-': push(&s, op1 - op2); break;
					case '*': push(&s, op1 * op2); break;
					case '/': 	if (op2 != 0)
									push(&s, op1 / op2);
								else{
									printf("Error: División por cero\n");
									result = -1;
									return &result;
								}
								break;
					default: printf("Operación no válida\n"); break;
				}
			}
			else{
				printf("Error: Pila vacía y se ha encontrado operación\n");
				result = -1;
				return &result;
			}
		}
		token = strtok(NULL, " ");
	}
	// Si todo ha salido correcto solo debe haber un elemento en la pila que será el resultado
	result = s.elements[0];
	return &result;
}

int *
power_1_svc(oper *op, struct svc_req *rqstp)
{
	static int result;
	result = 1;

	if (op->operator < 0){
		printf("Error: Exponente negativo\n");
		result = -1;
		return &result;
	}

	// result = pow(op->base, op->operator); Podría hacer esto pero no tiene gracia

	for (int i = 0; i < op->operator; i++){ // No hay mucha diferencia pero te ahorras una llamada
		result *= op->base;
	}

	return &result;
}

#include <math.h>
float *
sqrt_1_svc(oper *op, struct svc_req *rqstp)
{
	static float result;
	result = 0;

	if (op->base < 0){
		printf("Error: Raíz cuadrada de número negativo\n");
		result = -1;
		return &result;
	}

	result = (float)sqrt((double)op->base);
	return &result;
}

int *
mod_1_svc(oper *op, struct svc_req *rqstp)
{
	static int result;
	result = 0;

	if (op->operator == 0){
		printf("Error: División por cero\n");
		result = -1;
		return &result;
	}

	int op1 = (int)op->base;
	int op2 = (int)op->operator;

	result = op1 % op2;

	return &result;
}

int *
gcd_1_svc(oper *op, struct svc_req *rqstp)
{
	static int result;
	result = 0;

	int a = (int)op->base;
	int b = (int)op->operator;
	result = ((a < b) ? a : b); // Cogemos el mínimo de los dos números
	
	// Encontramos el mayor divisor del número mínimo que sea el máximo común divisor de los dos
	while (result > 0){
		if (a % result == 0 && b % result == 0){
			break;
		}
		result--;
	}

	// Si no lo ha encontrado será 0 y es que no tendrán
	return &result;
}

int *
fibonacci_1_svc(int *n, struct svc_req *rqstp)
{
	static int result;
	result = 0;	

	if (*n < 0){
		printf("Error: Número negativo\n");
		result = -1;
		return &result;
	}


	// Caso de 0 o 1 pues es 1 
	if (*n == 0){
		result = 0;

		return &result;
	}
	else
	if (*n == 1){
		result = 1;

		return &result;
	}
	else // Cualquier otro caso aplicamos la secuencia de fibonacci result(n) = result(n-1) + result(n-2) (0, 1, 1, 2, 3, 5, 8, 13, 21, 34...)
	{

	perror("step5");
		int a = 0;
		int b = 1;
		int c;
		for (int i = 2; i <= *n; i++){
			c = a + b;
			a = b;
			b = c;
		}
		result = c;

	perror("step6");
	}


	return &result;
}







/*---------------------------------------------------------------------------------*/






/*------------------------------VECTMATRIXCALC-------------------------------------*/
/*---------------------------------------------------------------------------------*/


nums *
sum_vector_1_svc(matrix * m, struct svc_req *rqstp)
{
	static nums result = {0,NULL}; 
	return &result;
}

nums *
sub_vector_1_svc(matrix * m, struct svc_req *rqstp)
{
	static nums result = {0,NULL}; 
	return &result;
}

nums *
cross_vector_1_svc(matrix * m, struct svc_req *rqstp)
{
	static nums result = {0,NULL}; 
	return &result;
}

nums *
mul_vector_1_svc(nums * v, struct svc_req *rqstp)
{
	static nums result = {0,NULL}; 
	return &result;
}

float *
mod_vector_1_svc(nums * v, struct svc_req *rqstp)
{
	static float result; 
	result = 0;
	return &result;
}

// Las matrices tienen que ser de igual dimensión
matrix *
sum_matrix_1_svc(matrix_group *mg, struct svc_req *rqstp)
{
	static matrix result;
	xdr_free((xdrproc_t)xdr_nums, &(result.matrix_val));
	result.matrix_val = NULL;
	result.matrix_len = 0;
	

	// Asignamos memoria pertinente para esta operación
	result.matrix_len = mg->m1.matrix_len;
	result.matrix_val = (nums *)malloc(mg->m1.matrix_len * sizeof(nums));
	for (int i = 0; i < mg->m1.matrix_len; i++) {
		result.matrix_val[i].nums_len = mg->m1.matrix_val[i].nums_len;
		result.matrix_val[i].nums_val = (int *)malloc(result.matrix_val[i].nums_len * sizeof(int));
	}

	char tmp[50];

	sprintf(tmp," %d ", mg->m1.matrix_val[0].nums_val[0]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m1.matrix_val[0].nums_val[1]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m1.matrix_val[1].nums_val[0]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m1.matrix_val[1].nums_val[1]);

	sprintf(tmp," %d ", mg->m2.matrix_val[0].nums_val[0]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m2.matrix_val[0].nums_val[1]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m2.matrix_val[1].nums_val[0]);
	perror(tmp);
	sprintf(tmp," %d ", mg->m2.matrix_val[1].nums_val[1]);
	


	// Verificamos que las dimensiones de las matrices son iguales
	// Comprobamos que tengan el mismo número de filas y luego que las primeras filas tengan el mismo número de columnas
	// Asumimos que las matrices son matrices, es decir, las filas tienen la misma longitud en la misma matriz
	if (mg->m1.matrix_len != mg->m2.matrix_len || mg->m1.matrix_val[0].nums_len != mg->m2.matrix_val[0].nums_len)
	{
		perror("Matrices no tienen las mismas dimensiones");
	}
	else
	{
		// Sumamos las matrices dentro del grupo
		for (int i = 0; i < mg->m1.matrix_len; i++)
			for (int j = 0; j <  mg->m1.matrix_val[i].nums_len; j++)
				result.matrix_val[i].nums_val[j] = mg->m1.matrix_val[i].nums_val[j] + mg->m2.matrix_val[i].nums_val[j];
		
	}


	return &result;
}


matrix *
sub_matrix_1_svc(matrix_group *mg, struct svc_req *rqstp)
{
	static matrix result;
	result.matrix_val = NULL;
	result.matrix_len = 0;
	
	if (result.matrix_val != NULL)
		xdr_free((xdrproc_t)xdr_nums, &(result.matrix_val)); // Liberamos memoria de otras ejecuciones de la operacion

	// Asignamos memoria pertinente para esta operación
	result.matrix_len = mg->m1.matrix_len;
	result.matrix_val = (nums *)malloc(mg->m1.matrix_len * sizeof(nums));
	for (int i = 0; i < mg->m1.matrix_len; i++) {
		result.matrix_val[i].nums_len = mg->m1.matrix_val[i].nums_len;
		result.matrix_val[i].nums_val = (int *)malloc(result.matrix_val[i].nums_len * sizeof(int));
	}
	
	// Verificamos que las dimensiones de las matrices son iguales
	// Comprobamos que tengan el mismo número de filas y luego que las primeras filas tengan el mismo número de columnas
	// Asumimos que las matrices son matrices, es decir, las filas tienen la misma longitud en la misma matriz
	if (mg->m1.matrix_len != mg->m2.matrix_len || mg->m1.matrix_val[0].nums_len != mg->m2.matrix_val[0].nums_len)
	{
		perror("The matrixes doesnt have equal size!");
	}
	else
	{
		// Sumamos las matrices dentro del grupo
		for (int i = 0; i < mg->m1.matrix_len; i++)
			for (int j = 0; j < mg->m1.matrix_val[0].nums_len;j++)
				result.matrix_val[i].nums_val[j] = mg->m1.matrix_val[i].nums_val[j] - mg->m2.matrix_val[i].nums_val[j];
	}

	return &result;
}


matrix *
mul_matrix_1_svc(matrix_group *mg, struct svc_req *rqstp)
{
	static matrix result;
	result.matrix_val = NULL;
	result.matrix_len = 0;
	
	if (result.matrix_val != NULL)
		xdr_free((xdrproc_t)xdr_nums, &(result.matrix_val)); // Liberamos memoria de otras ejecuciones de la operacion


	// Asignamos memoria pertinente para esta operación
	result.matrix_len = mg->m1.matrix_len;
	result.matrix_val = (nums *)malloc(mg->m1.matrix_len * sizeof(nums));
	for (int i = 0; i < mg->m1.matrix_len; i++) {
		result.matrix_val[i].nums_len = mg->m2.matrix_val[i].nums_len;
		result.matrix_val[i].nums_val = (int *)malloc(result.matrix_val[i].nums_len * sizeof(int));
	}

	// Tenemos que verificar que se cumpla la propiedad de que en A x B, la matriz A tiene un número de columnas n y la matriz
	// B tiene un número de filas n, los otros dos valores si pueden ser distintos. Las filas y columnas de A y B respectivamente 
	// lo que harán es determinar el tamaño de la matriz resultante
	if (mg->m1.matrix_val[0].nums_len != mg->m2.matrix_len)
	{
		perror("The columns of the first matrix and rows of the second one are different!");
	}
	else
	{	
		int tmp = 0;
		for (int i = 0; i < mg->m1.matrix_len; i++){ // Recorremos las filas de m1 primero
			for (int j = 0; j < mg->m2.matrix_val[0].nums_len; j++){	// Luego para cada fila de m1 recorremos todas las columnas de m2
				for(int z = 0; z < mg->m2.matrix_len; z++){ // Por último, para cada fila m1 por cada columna de m2, recorremos el valor n (columnas m1=filas m2)
					tmp += mg->m1.matrix_val[i].nums_val[z] * mg->m2.matrix_val[z].nums_val[j];
				}
				result.matrix_val[i].nums_val[j] = tmp;
				tmp = 0;
			}
		}
	}

	return &result;
}
