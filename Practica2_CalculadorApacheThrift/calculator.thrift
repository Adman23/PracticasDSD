
typedef i32 int
service CalculatorService
{
        // Acepta dos doubles para sumarlos
        double mul(1:double n1, 2:double n2),

        // Acepta dos double para multiplicarlos
        double sum(1:double n1, 2:double n2),

        // Acepta dos double para restarlos
        double sub(1:double n1, 2:double n2),

        // Acepta dos double para dividirlos
        double div(1:double n1, 2:double n2),

        // Acepta dos dobles para hacer la potencia
        double pow(1:double n1, 2:double n2),

        // Modulo de un número
        int mod(1:int n1, 2:int n2)

        // Máximo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcd(1:list<int> nums),

        // Mínimo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcm(1:list<int> nums),

        // Factorial de un número
        int factorial(1:int num),

}