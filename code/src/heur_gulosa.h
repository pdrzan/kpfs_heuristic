#include "scip/scip.h"
  
int gulosa(SCIP* scip, SCIP_SOL** sol, SCIP_HEUR* heur);

/** creates the gulosa_crtp primal heuristic and includes it in SCIP */
SCIP_RETCODE SCIPincludeHeurGulosa(
   SCIP* scip                /**< SCIP data structure */
   );
