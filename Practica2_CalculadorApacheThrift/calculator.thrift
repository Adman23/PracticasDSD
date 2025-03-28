
typedef i32 int
service CalculatorService
{
        // Todas estas operaciones aceptan doubles porque pueden, ahora, si se pasan int
        // no pasa nada, simplemente se hace casting al pasarlos como argumento y
        // luego se vuelve a hacer casting si se quiere el resultado explicitamente como int
        // Si se pide solo int se limita la funcionalidad. Otra solución es hacer 2 funciones
        // de cada, una para double otra para int.

        // Acepta un listado de números de tamaño dinámico que se multiplican en cadena
        double mul(1:list<double> nums),

        // Acepta un listado de números de tamaño dinámico que se suman en cadena
        double sum(1:list<double> nums),

        // Acepta dos double para restarlos
        double sub(1:double n1, 2:double n2),

        // Acepta dos double para dividirlos
        double div(1:double n1, 2:double n2),

        // Máximo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcd(1:list<int> nums),

        // Mínimo común divisor con un mínimo de 4 números, si se pasan menos de 4 se controla
        // desde la definición de la función
        int mcm(1:list<int> nums),

        // Factorial de un número
        int factorial(1:int num)

}