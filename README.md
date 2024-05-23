# kpfs_heuristic

This repository contains a exact algorithm and two heuristics for the Knapsack Problem with Forfeit Sets (KPFS). The KPFS was first introduced by [D’Ambrosio, 2022](https://doi.org/10.1016/j.cor.2022.106093) and is a variant of the classical Knapsack Problem.

The objective of this work was to create two heuristics for the problem. We decided to implement the following heuristics:
1. A random heuristic
2. A greedy heuristic

The first one creates another solution from a already know one. It does that by selecting a random item trying to insert it in the solution. This process is repeated n (number of items) time.

The greedy heuristic sorts the items by  ${Value}/{Weight}$ rate and inserts the most valuable one in the solution. It does that iteratively while some item is worth inserting into the backpack.

## Dependencies
- [SCIP](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.scipopt.org/&ved=2ahUKEwiiwpr-w6GGAxWBIbkGHVcfALYQFnoECAYQAQ&usg=AOvVaw38_N8upHjIO8DUS_lJgDLp)