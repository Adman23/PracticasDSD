
typedef i32 int
service CalculatorService
{
        // Todas estas operaciones aceptan doubles porque pueden, ahora, si se pasan int
        // no pasa nada, simplemente se hace casting al pasarlos como argumento y
        // luego se vuelve a hacer casting si se quiere el resultado explicitamente como int
        // Si se pide solo int se limita la funcionalidad. Otra solución es hacer 2 funciones
        // de cada, una para double otra para int.

        double mul(1:double n1, 2:double n2),

        double sum(1:double n1, 2:double n2),

        // Acepta dos double para restarlos
        double sub(1:double n1, 2:double n2),

        // Acepta dos double para dividirlos
        double div(1:double n1, 2:double n2),

        // Acepta dos dobles para hacer la potencia
        double pow(1:double n1, 2:double n2),

        // Modulo de un número
        int mod(1:int num, 2:int num)

        // Máximo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcd(1:list<int> nums),

        // Mínimo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcm(1:list<int> nums),

        // Factorial de un número
        int factorial(1:int num),



}