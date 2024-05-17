#include <assert.h>

#include "probdata_mochila.h"
#include "parameters_mochila.h"
#include "heur_gulosa.h"
#include "heur_problem.h"

/* configuracao da heuristica */
#define HEUR_NAME             "gulosa"
#define HEUR_DESC             "gulosa primal heuristic"
#define HEUR_DISPCHAR         'g'
#define HEUR_PRIORITY         2 
#define HEUR_FREQ             1 
#define HEUR_FREQOFS          0 
#define HEUR_MAXDEPTH         -1 
#define HEUR_TIMING           SCIP_HEURTIMING_AFTERNODE 
#define HEUR_USESSUBSCIP      FALSE 

/** copy method for primal heuristic plugins (called when SCIP copies plugins) */
static
SCIP_DECL_HEURCOPY(heurCopyGulosa)
{  /*lint --e{715}*/

   return SCIP_OKAY;
}

/** destructor of primal heuristic to free user data (called when SCIP is exiting) */
static
SCIP_DECL_HEURFREE(heurFreeGulosa)
{  /*lint --e{715}*/

   return SCIP_OKAY;
}


/** initialization method of primal heuristic (called after problem was transformed) */
static
SCIP_DECL_HEURINIT(heurInitGulosa)
{  /*lint --e{715}*/


   return SCIP_OKAY;
}


/** deinitialization method of primal heuristic (called before transformed problem is freed) */
static
SCIP_DECL_HEUREXIT(heurExitGulosa)
{  /*lint --e{715}*/

   return SCIP_OKAY;
}


/** solving process initialization method of primal heuristic (called when branch and bound process is about to begin) */
static
SCIP_DECL_HEURINITSOL(heurInitsolGulosa)
{  /*lint --e{715}*/

   return SCIP_OKAY;
}


/** solving process deinitialization method of primal heuristic (called before branch and bound process data is freed) */
static
SCIP_DECL_HEUREXITSOL(heurExitsolGulosa)
{  /*lint --e{715}*/

   return SCIP_OKAY;
}


int gulosa(SCIP* scip, SCIP_SOL** sol, SCIP_HEUR* heur)
{
   int found, infeasible, nInSolution;
   unsigned int stored;
   int nvars;
   int n, custo, *covered, *cand, nCands, selected, *forfeit, valor, violations, pre_violations;
   itemType *cands;
   SCIP_VAR *var, **solution, **varlist;
   SCIP_Real bestUb;
   SCIP_PROBDATA* probdata;
   int i, residual, j, peso, nS, ii, jj, best, iBest, pre_violationsBest;
   instanceT* I;
   
   probdata=SCIPgetProbData(scip);
   assert(probdata != NULL);

   nvars = SCIPprobdataGetNVars(probdata);
   varlist = SCIPprobdataGetVars(probdata);
   I = SCIPprobdataGetInstance(probdata);
   n = I->n;
   nS = I->nS;
    
   solution = (SCIP_VAR**) malloc(sizeof(SCIP_VAR*)*(n+nS));
   forfeit = (int*) calloc(nS,sizeof(int));
   cand = (int*) malloc(n*sizeof(int));
   nInSolution = 0;
   nCands = 0;
   custo = 0;
   residual = I->C;
   violations = 0;

   for(i=0;i<nvars;i++){
      var = varlist[i];
      if(SCIPvarGetLbLocal(var) > 1.0 - EPSILON){ 
        if (i<n){
          solution[nInSolution++]=var;
          residual -= I->item[i].weight;
          for(j=0;j<I->item[i].nsets;j++){
             ii = I->item[i].set[j];
             forfeit[ii]++; 
            //  if(forfeit[ii] >= I->S[ii].h){
             if(forfeit[ii] > I->S[ii].h){
               violations++;
             }
          }
          custo += I->item[i].value;
          infeasible = residual < 0?1:0;
        }
        else{
           j = n - i;
        }
      }
      else{ 
        if(SCIPvarGetUbLocal(var) < EPSILON){ 
        }
        else{
          if (i < n){ 
           cand[nCands++]=i;
          }
        }
      }
   }
   

   cands = (itemType *)calloc(nCands, sizeof(itemType));
   covered = (int *)calloc(nCands, sizeof(int));
   for(i = 0; i < nCands; i++)
   {
     cands[i].weight = I->item[cand[i]].weight;
     cands[i].label = I->item[cand[i]].label;
     cands[i].value = I->item[cand[i]].value;
   }

   
   for(i = 0; i < nCands && residual > 0; i++)
   {
     best = 0;
     iBest = 0;
     for(j = 0; j < nCands; j++)
     {
       if(cands[j].weight <= residual)
       {
         valor = cands[j].value;
         selected = cands[j].label;
         pre_violations = 0;
         for(jj=0;jj<I->item[selected].nsets;jj++)
         {
           ii = I->item[selected].set[jj];
           if(forfeit[ii] >= I->S[ii].h)
           {
             valor -= I->S[ii].d;
             pre_violations++;
           }
         }
       }
       if(valor > best && violations + pre_violations <= I->k)
       {
         best = valor;
         iBest = selected;
         pre_violationsBest = pre_violations;
       }
     }
     if(best > 0)
     {
       var = varlist[iBest];
       solution[nInSolution++]=var;
       residual -= I->item[iBest].weight;
       custo += best;
       violations += pre_violationsBest;
       for(j=0;j<I->item[iBest].nsets;j++){
         ii = I->item[iBest].set[j];
         forfeit[ii]++;
       }
     infeasible = residual<0?1:0;
     }
   }
   if(!infeasible){
      SCIP_CALL( SCIPcreateSol(scip, sol, heur) );
      for(i=0;i<nInSolution;i++){
         var = solution[i];
         SCIP_CALL( SCIPsetSolVal(scip, *sol, var, 1.0) );
      }
      for(j=0;j<nS;j++){
         valor = forfeit[j]>I->S[j].h? forfeit[j] - I->S[j].h:0;
         SCIP_CALL( SCIPsetSolVal(scip, *sol, varlist[I->n+j], (double) valor) );
      }
      bestUb = SCIPgetPrimalbound(scip);
     if(!infeasible && custo > bestUb + EPSILON){
         SCIP_CALL( SCIPtrySolMine(scip, *sol, TRUE, TRUE, FALSE, TRUE, &stored) );
         if( stored )
         {
            found = 1;
         }
         else{
            found = 0;
         }
      }
   }
   free(cand);
   free(solution);
   free(forfeit);
   free(cands);
   return found;

}
 /** execution method of primal heuristic */
static
SCIP_DECL_HEUREXEC(heurExecGulosa)
{  /*lint --e{715}*/
   SCIP_SOL*             sol;                /**< solution to round */
   int nlpcands;

   assert(result != NULL);
   //   assert(SCIPhasCurrentNodeLP(scip));

   *result = SCIP_DIDNOTRUN;

   /* continue only if the LP is finished */
   if ( SCIPgetLPSolstat(scip) != SCIP_LPSOLSTAT_OPTIMAL )
      return SCIP_OKAY;

   /* continue only if the LP value is less than the cutoff bound */
   if( SCIPisGE(scip, SCIPgetLPObjval(scip), SCIPgetCutoffbound(scip)) )
      return SCIP_OKAY;


   /* check if there exists integer variables with fractionary values in the LP */
   SCIP_CALL( SCIPgetLPBranchCands(scip, NULL, NULL, NULL, &nlpcands, NULL, NULL) );
   //Fractional implicit integer variables are stored at the positions *nlpcands to *nlpcands + *nfrac - 1
  
   /* stop if the LP solution is already integer   */
   if ( nlpcands == 0 )
     return SCIP_OKAY;

   if(gulosa(scip, &sol, heur)){
     *result = SCIP_FOUNDSOL;
   }
   else{
     *result = SCIP_DIDNOTFIND;
#ifdef DEBUG_PRIMAL
     printf("\nGulosa could not find feasible solution!");      
#endif
   }
   return SCIP_OKAY;
}


/*
 * primal heuristic specific interface methods
 */

SCIP_RETCODE SCIPincludeHeurGulosa(
   SCIP* scip                /**< SCIP data structure */
   )
{
   SCIP_HEURDATA* heurdata;
   SCIP_HEUR* heur;

   heurdata = NULL;

   heur = NULL;

   /* include primal heuristic */
#if 0
   /* use SCIPincludeHeur() if you want to set all callbacks explicitly and realize (by getting compiler errors) when
    * new callbacks are added in future SCIP versions
    */
   SCIP_CALL( SCIPincludeHeur(scip, HEUR_NAME, HEUR_DESC, HEUR_DISPCHAR, HEUR_PRIORITY, param.heur_freq, param.heur_freqofs,
         param.heur_maxdepth, HEUR_TIMING, HEUR_USESSUBSCIP,
         heurCopyGulosa, heurFreeGulosa, heurInitGulosa, heurExitGulosa, heurInitsolGulosa, heurExitsolGulosa, heurExecGulosa,
         heurdata) );
#else
   /* use SCIPincludeHeurBasic() plus setter functions if you want to set callbacks one-by-one and your code should
    * compile independent of new callbacks being added in future SCIP versions
    */
   SCIP_CALL( SCIPincludeHeurBasic(scip, &heur,
         HEUR_NAME, HEUR_DESC, HEUR_DISPCHAR, HEUR_PRIORITY, param.heur_round_freq, param.heur_round_freqofs,
         param.heur_round_maxdepth, HEUR_TIMING, HEUR_USESSUBSCIP, heurExecGulosa, heurdata) );

   assert(heur != NULL);

   /* set non fundamental callbacks via setter functions */
   SCIP_CALL( SCIPsetHeurCopy(scip, heur, heurCopyGulosa) );
   SCIP_CALL( SCIPsetHeurFree(scip, heur, heurFreeGulosa) );
   SCIP_CALL( SCIPsetHeurInit(scip, heur, heurInitGulosa) );
   SCIP_CALL( SCIPsetHeurExit(scip, heur, heurExitGulosa) );
   SCIP_CALL( SCIPsetHeurInitsol(scip, heur, heurInitsolGulosa) );
   SCIP_CALL( SCIPsetHeurExitsol(scip, heur, heurExitsolGulosa) );
#endif

   return SCIP_OKAY;
}       
