#include "utils.h"
/* sorteia um numero aleatorio entre [low,high] */
int RandomInteger(int low, int high)
{
  int k;
  double d;
  //  srand(time(NULL));

  d = (double)rand() / ((double)RAND_MAX + 1);
  k = d * (high - low + 1);
  return low + k;
}

/* put your local methods here, and declare them static */
// SCIP_RETCODE SCIPtrySolMine(SCIP* scip, SCIP_SOL* sol, SCIP_Bool printreason, SCIP_Bool checkbounds, SCIP_Bool checkintegrality, SCIP_Bool checklprows, SCIP_Bool *stored)
// {
// #if (defined SCIP_VERSION_MAJOR)  ||  (SCIP_VERSION_MAJOR==6)
//    return SCIPtrySol(scip,sol, TRUE,printreason,checkbounds,checkintegrality,checklprows,stored);
// #else
//    return SCIPtrySol(scip,sol,printreason,checkbounds,checkintegrality,checklprows,stored);
// #endif
// }

// Função auxiliar de comparacao para o qsort
int comparador(const void *valor1, const void *valor2)
{
  if ((*(itemType *)valor1).value > (*(itemType *)valor2).value)
  {
    return 1;
  }
  else if ((*(itemType *)valor1).value == (*(itemType *)valor2).value)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}
