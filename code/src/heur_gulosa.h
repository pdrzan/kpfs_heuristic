#include "scip/scip.h"
  
int gulosa(SCIP* scip, SCIP_SOL** sol, SCIP_HEUR* heur);

/** creates the gulosa_crtp primal heuristic and includes it in SCIP */
SCIP_RETCODE SCIPincludeHeurGulosa(
   SCIP* scip                /**< SCIP data structure */
   );

int itemComparator(const void* item1, const void* item2)
{
   // float valueItem1 = (float) ((itemType*)item1)->value / (float) ((itemType*)item1)->weight;
   // float valueItem2 = (float) ((itemType*)item2)->value / (float) ((itemType*)item2)->weight;
   // printf("Value 1: %f / Value 2: %f\n", valueItem1, valueItem2);
   // if(valueItem1 > valueItem2)
   // {
   //   return -1;
   // }
   // else
   // {
   //   return 1;
   // }
   // Aparentemente não vale a pena fazer a divisão do valor pelo peso
   float valueItem1 = ((itemType*)item1)->value; 
   float valueItem2 = ((itemType*)item2)->value;
   return (valueItem2 - valueItem1);
}
