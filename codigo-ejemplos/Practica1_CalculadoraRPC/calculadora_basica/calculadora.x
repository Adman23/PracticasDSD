
/*  Archivo calculadora.x para realización de operaciones
    aritméticas simples */

/*  Los programas recibiran una estructura que contendrá un listado de 
    operandos (ya que se lo estas pasando a funciones específicas), esto
    aplica para los 4 primeros procesos */

/*  Opcion -N para rpcgen es para no tener las funciones
    con el parámetro extra struct svc_req *req que contiene
    información del cliente, también hace que los argumentos se
    tengan que pasar por valor */

const MAX = 10; /* Máximo de 10 operandos */
typedef int nums<MAX>; /* Listado de int que son los operandos */

struct operands { 
    int op1;     
    int op2;
};

program CALCPROG{
    version CALCSIMPLEVER{            
        int SUM(nums) = 1;  /* Suma simple */
        int SUB(operands) = 2;  /* Resta simple */
        int MULT(nums) = 3; /* Multiplicación simple */
        float DIV(operands) = 4;/* División simple */
    } = 1;
} = 0x20000001;