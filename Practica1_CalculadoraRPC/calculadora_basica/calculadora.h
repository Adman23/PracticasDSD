/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CALCULADORA_H_RPCGEN
#define _CALCULADORA_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX 10

typedef struct {
	u_int nums_len;
	int *nums_val;
} nums;

typedef struct {
	u_int matrix_len;
	nums *matrix_val;
} matrix;

struct matrix_group {
	matrix m1;
	matrix m2;
};
typedef struct matrix_group matrix_group;

struct oper {
	float base;
	float operator;
};
typedef struct oper oper;

#define CALCPROG 0x20000001
#define CALCSIMPLEVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define SUM 1
extern  int * sum_1(nums *, CLIENT *);
extern  int * sum_1_svc(nums *, struct svc_req *);
#define SUB 2
extern  int * sub_1(nums *, CLIENT *);
extern  int * sub_1_svc(nums *, struct svc_req *);
#define MULT 3
extern  int * mult_1(nums *, CLIENT *);
extern  int * mult_1_svc(nums *, struct svc_req *);
#define DIV 4
extern  float * div_1(nums *, CLIENT *);
extern  float * div_1_svc(nums *, struct svc_req *);
extern int calcprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SUM 1
extern  int * sum_1();
extern  int * sum_1_svc();
#define SUB 2
extern  int * sub_1();
extern  int * sub_1_svc();
#define MULT 3
extern  int * mult_1();
extern  int * mult_1_svc();
#define DIV 4
extern  float * div_1();
extern  float * div_1_svc();
extern int calcprog_1_freeresult ();
#endif /* K&R C */

#define CALCADVANCEDPROG 0x20000002
#define CALCADVANVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define SUFIX 1
extern  float * sufix_1(char **, CLIENT *);
extern  float * sufix_1_svc(char **, struct svc_req *);
#define POWER 2
extern  int * power_1(oper *, CLIENT *);
extern  int * power_1_svc(oper *, struct svc_req *);
#define SQRT 3
extern  float * sqrt_1(oper *, CLIENT *);
extern  float * sqrt_1_svc(oper *, struct svc_req *);
#define MOD 4
extern  int * mod_1(oper *, CLIENT *);
extern  int * mod_1_svc(oper *, struct svc_req *);
#define GCD 5
extern  int * gcd_1(oper *, CLIENT *);
extern  int * gcd_1_svc(oper *, struct svc_req *);
#define FIBONACCI 6
extern  int * fibonacci_1(int *, CLIENT *);
extern  int * fibonacci_1_svc(int *, struct svc_req *);
extern int calcadvancedprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SUFIX 1
extern  float * sufix_1();
extern  float * sufix_1_svc();
#define POWER 2
extern  int * power_1();
extern  int * power_1_svc();
#define SQRT 3
extern  float * sqrt_1();
extern  float * sqrt_1_svc();
#define MOD 4
extern  int * mod_1();
extern  int * mod_1_svc();
#define GCD 5
extern  int * gcd_1();
extern  int * gcd_1_svc();
#define FIBONACCI 6
extern  int * fibonacci_1();
extern  int * fibonacci_1_svc();
extern int calcadvancedprog_1_freeresult ();
#endif /* K&R C */

#define CALCVECTORMATRIXPROG 0x20000003
#define CALCVMVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define SUM_VECTOR 1
extern  nums * sum_vector_1(matrix *, CLIENT *);
extern  nums * sum_vector_1_svc(matrix *, struct svc_req *);
#define SUB_VECTOR 2
extern  nums * sub_vector_1(matrix *, CLIENT *);
extern  nums * sub_vector_1_svc(matrix *, struct svc_req *);
#define CROSS_VECTOR 3
extern  nums * cross_vector_1(matrix *, CLIENT *);
extern  nums * cross_vector_1_svc(matrix *, struct svc_req *);
#define MUL_VECTOR 4
extern  nums * mul_vector_1(nums *, CLIENT *);
extern  nums * mul_vector_1_svc(nums *, struct svc_req *);
#define MOD_VECTOR 5
extern  float * mod_vector_1(nums *, CLIENT *);
extern  float * mod_vector_1_svc(nums *, struct svc_req *);
#define SUM_MATRIX 6
extern  matrix * sum_matrix_1(matrix_group *, CLIENT *);
extern  matrix * sum_matrix_1_svc(matrix_group *, struct svc_req *);
#define SUB_MATRIX 7
extern  matrix * sub_matrix_1(matrix_group *, CLIENT *);
extern  matrix * sub_matrix_1_svc(matrix_group *, struct svc_req *);
#define MUL_MATRIX 8
extern  matrix * mul_matrix_1(matrix_group *, CLIENT *);
extern  matrix * mul_matrix_1_svc(matrix_group *, struct svc_req *);
#define MOD_MATRIX 9
extern  float * mod_matrix_1(matrix *, CLIENT *);
extern  float * mod_matrix_1_svc(matrix *, struct svc_req *);
extern int calcvectormatrixprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SUM_VECTOR 1
extern  nums * sum_vector_1();
extern  nums * sum_vector_1_svc();
#define SUB_VECTOR 2
extern  nums * sub_vector_1();
extern  nums * sub_vector_1_svc();
#define CROSS_VECTOR 3
extern  nums * cross_vector_1();
extern  nums * cross_vector_1_svc();
#define MUL_VECTOR 4
extern  nums * mul_vector_1();
extern  nums * mul_vector_1_svc();
#define MOD_VECTOR 5
extern  float * mod_vector_1();
extern  float * mod_vector_1_svc();
#define SUM_MATRIX 6
extern  matrix * sum_matrix_1();
extern  matrix * sum_matrix_1_svc();
#define SUB_MATRIX 7
extern  matrix * sub_matrix_1();
extern  matrix * sub_matrix_1_svc();
#define MUL_MATRIX 8
extern  matrix * mul_matrix_1();
extern  matrix * mul_matrix_1_svc();
#define MOD_MATRIX 9
extern  float * mod_matrix_1();
extern  float * mod_matrix_1_svc();
extern int calcvectormatrixprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_nums (XDR *, nums*);
extern  bool_t xdr_matrix (XDR *, matrix*);
extern  bool_t xdr_matrix_group (XDR *, matrix_group*);
extern  bool_t xdr_oper (XDR *, oper*);

#else /* K&R C */
extern bool_t xdr_nums ();
extern bool_t xdr_matrix ();
extern bool_t xdr_matrix_group ();
extern bool_t xdr_oper ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CALCULADORA_H_RPCGEN */
