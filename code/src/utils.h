#ifndef UTILS_H
#define UTILS_H
#include "scip/scip.h"
#include "problem.h"
/* sorteia um numero aleatorio entre [low,high] */
int RandomInteger(int low, int high);
/* put your local methods here, and declare them static */
SCIP_RETCODE SCIPtrySolMine(SCIP* scip, SCIP_SOL* sol, SCIP_Bool printreason, SCIP_Bool checkbounds, SCIP_Bool checkintegrality, SCIP_Bool checklprows, SCIP_Bool *stored);
int comparador(const void *valor1, const void *valor2);
#endif
