

/*  Archivo calculadora.x  */


/*  Opcion -N para rpcgen es para no tener las funciones
    con el parámetro extra struct svc_req *req que contiene
    información del cliente, también hace que los argumentos se
    tengan que pasar por valor */


const MAX = 10; /* Máximo de 10 operandos */
typedef int nums<MAX>; /* Listado de int que son los operandos */
typedef nums matrix<MAX>; /* Doble listado de int (matriz) */

struct matrix_group{
    matrix m1;
    matrix m2;
};

struct oper{
    float base;
    float operator;
};

program CALCPROG{
    version CALCSIMPLEVER{            
        int SUM(nums) = 1;  /* Suma simple */
        int SUB(nums) = 2;  /* Resta simple */
        int MULT(nums) = 3; /* Multiplicación simple */
        float DIV(nums) = 4;/* División simple */
    } = 1;
} = 0x20000001;

program CALCADVANCEDPROG{
    version CALCADVANVER{
        float SUFIX(string) = 1;
        int POWER(oper) = 2;
        float SQRT(oper) = 3; 
        int MOD(oper) = 4;
        int GCD(oper) = 5;
        int FIBONACCI(int) = 6;
    } = 1;
} = 0x20000002; 


program CALCVECTORMATRIXPROG{
    version CALCVMVER{
        nums SUM_VECTOR(matrix) = 1;
        nums SUB_VECTOR(matrix) = 2;
        nums CROSS_VECTOR(matrix) = 3; /*Multiplicación entre 2 vectores*/
        nums MUL_VECTOR(nums) = 4; /*Escalar multiplicado por vector, se considera que el último valor del vector es el*/
                                   /*que se usa para multiplicar todo el vector*/
        float MOD_VECTOR(nums) = 5;

        matrix SUM_MATRIX(matrix_group) = 6;
        matrix SUB_MATRIX(matrix_group) = 7;
        matrix MUL_MATRIX(matrix_group) = 8;
        float MOD_MATRIX(matrix) = 9;        
    } = 1;                         
} = 0x20000003;