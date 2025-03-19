/* Archivo calculadora_avanzada.x para realización de 
 * operaciones avanzadas, de vectores y matrices   
 */


const MAX = 10; /* Máximo vectores de 10 elementos */
typedef int nums<MAX>; /* Listado de int que son los operandos */

struct vector_operands { 
    nums op1;     
    nums op2;
};

program CALCADVANPROG{
    version CALCADVANVER{            
        nums VECTORSUM(vector_operands) = 1;
    } = 1;
} = 0x20000002;