/* Archivo calculadora_avanzada.x para realización de 
 * operaciones avanzadas, de vectores y matrices   
 */

const MAX_SIZE = 10; /* Máximo de 10 operandos */
typedef int v<MAX>; /* Listado de int que son los operandos */


struct vector_operands { 
    v op1;     
    v op2;
};

program CALCADVANPROG{
    version CALCADVANVER{            
        v VECTORSUM(vector_operands) = 1;
    } = 1;
} = 0x20000002;