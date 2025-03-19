/*
 * Autor: Adam Navarro Megías
 */

#include "calculadora.h"

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
sub_1_svc(operands *op, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	result = op->op1 - op->op2;
	
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
div_1_svc(operands *operands, struct svc_req *rqstp)
{
	static float  result;
	result = 0;

	if (operands->op2 == 0) {
		printf("Error: División por cero\n");
		result = 0;
		return &result;
	}

	result = (float)operands->op1 / (float)operands->op2;

	return &result;
}
