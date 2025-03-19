/*
 * Autor: Adam Navarro Megías
 */

#include "calculadora.h"
#include "../calculadora_avanzada/calculadora_avanzada.h"




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


nums *
sum_vector_1_svc(vs *vectors, struct svc_req *rqstp)
{
	CLIENT *clnt_2;
	char *host = "localhost";
	nums  *result;
	vector_operands  vectorsum_1_arg;
	vectorsum_1_arg.op1.v_val = vectors->op1.nums_val;
	vectorsum_1_arg.op1.v_len = vectors->op1.nums_len;
	vectorsum_1_arg.op2.v_val = vectors->op2.nums_val;
	vectorsum_1_arg.op2.v_len = vectors->op2.nums_len;


#ifndef	DEBUG_2
	clnt_2 = clnt_create (host, CALCADVANPROG, CALCADVANVER, "udp");
	if (clnt_2 == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	v* vec = vectorsum_1(&vectorsum_1_arg, clnt_2);
	result->nums_val = vec->v_val;
	result->nums_len = vec->v_len;

	if (result == (nums *) NULL) {
		clnt_perror (clnt_2, "call to vectorsum failed");
	}

#ifndef	DEBUG_2
	clnt_destroy (clnt_2);
#endif	 /* DEBUG */

	return &result;
}


