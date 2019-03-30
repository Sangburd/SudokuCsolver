#include <math.h>
#include "mt19937.h"

int sudoku[9][9] = {{0,3,5,0,8,0,2,0,0},
					{0,0,0,0,0,0,5,9,0},
					{1,0,0,4,0,0,0,7,0},
					{0,7,0,9,0,0,0,5,0},
					{0,9,0,0,0,0,0,6,0},
					{0,1,0,0,0,2,0,3,0},
					{0,8,0,0,0,3,0,0,5},
					{0,5,4,0,0,0,0,0,0},
					{0,0,3,0,2,0,9,8,0}};
						
int newdoku[9][9];

double B = 1.0;

int duplicate(int puzzle[][9], int row, int col,int n) {
	// calculates the amount of duplicates caused by the number puzzle[row][col]
	int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i, j, dup=0;
    
    for(i=0;i<9;++i) {
    	if ((puzzle[i][col] == n)&&(i!=row)) {
			dup+=1;
		}
		if ((puzzle[row][i] == n)&&(i!=col)) {
			dup+=1;
		}
	}
	for(i=0;i<3;++i){
		for(j=0;j<3;++j) {
			if ((puzzle[rowStart+i][colStart +j] == n)&&((rowStart +i != row) && (colStart+j != col))) dup+=1;
		}
	}
	return dup;
}

int e_dif(int rown, int coln, int n) {
	int e_o = duplicate(newdoku,rown,coln,newdoku[rown][coln]);
	int e_n = duplicate(newdoku, rown, coln, n);
	return e_n - e_o;
}

int total_e(int puzzle[][9]) {
	int i,j,e=0;
	for (i = 0; i<9; ++i) {
		for(j=0; j<9; ++j) {
			e+= duplicate(puzzle,i,j,puzzle[i][j]);
		}
	}
	return e;
}
	
int main() {
	dsfmt_seed(time(NULL));
	int i,j;
// generate random solution for all unfilled positions in sudoku				
	for (i=0;i<9;++i) {
		for (j=0;j<9;++j) {
			if (sudoku[i][j] == 0) {
				newdoku[i][j] = dsfmt_genrand()*9+1;
			}
			else{
				newdoku[i][j] = sudoku[i][j];
		}
		}
	}
//	printf(" %d \t %d \t %d\t %d \n", duplicate(testdoku,0,1),duplicate(testdoku,1,6),duplicate(testdoku,7,1),duplicate(testdoku,1,8));
// do mc simulation etc.

	int e,rown,coln,num;
	while(total_e(newdoku) !=0) {
		for(B=1; B<1000; B*=1.001) {
			int boolean = 1;
			rown = dsfmt_genrand()*9;
			coln = dsfmt_genrand()*9;
			if (sudoku[rown][coln]!=0) continue;
			while(boolean ==1){
				boolean = 0;
				num = dsfmt_genrand()*9+1;
				for(i=0;i<9;++i) {
					if (i==rown) continue;
					if(sudoku[i][coln] != 0 && newdoku[i][coln] == num) {
						boolean = 1;
						break;
					}
				}
				for(j=0;j<9;++j) {
					if (j==coln) continue;
					if(sudoku[rown][j] != 0 && newdoku[rown][j] == num) {
						boolean = 1;
						break;
					}
				}	
				int rowStart;
				int colStart;
				 
				for (rowStart= ((int) rown/3)*3; rowStart < ((int) rown/3)*3 + 3; ++rowStart) {
					if (boolean == 1) {break;}
					for (colStart = ((int) coln/3) * 3; colStart < ((int) coln/3)*3 + 3; ++colStart) {
						if (rowStart == rown && colStart == coln) continue;
						if (sudoku[rowStart][colStart]!=0 && newdoku[rowStart][colStart] == num) {
							boolean = 1;
							break;
						}
					}
				}
			}
			e = e_dif(rown,coln,num);
			if(dsfmt_genrand() < exp(-B*e)) newdoku[rown][coln] = num;
		}
	}
	printf("\n+-----+-----+-----+\n");
    for(i=0; i<9; ++i) {
        for(j=0; j<9; ++j) printf("|%d", newdoku[i][j]);
        printf("|\n");
        if ((i+1)%3 == 0) printf("+-----+-----+-----+\n");
	}
	getchar();
	return 1;
}






