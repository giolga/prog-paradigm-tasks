#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

int board[33], n, solution;

void draw() {
    solution++;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(board[j] == i) {
                printf("K");
            }
            else printf(".");
        }
        printf("\n");
    }
    printf("\n");
}

bool danger(int row, int col) {
    for(int i = 1; i < col; i++) {
        int prevCol = i, prevRow = board[i];
        if(row == prevRow || abs(row - prevRow) == abs(col - prevCol)) return true;
    }
    return false;
}

void start(int col) {
    if(col > n) {
        draw();
    }
    else {
        for(int row = 1; row <= n; row++) {
            if(!danger(row, col)) {
                board[col] = row;
                start(col + 1);
            }
        }
    }
}

int main() {
    // ჩემი დაწერილი პროგრამა იყენებს backtracking-ის ალგორითმს, და გამოაქვს ყველა ის შესაძლებელი ვარიანტი, რომლითაც 
    // შეიძლება განლაგნენ დედოფლები, 8x8 -ზე 92 ვარიანტი, 4x4 2 შესაძლო ვარიანტი
    // რაც შეეხება 30X30, რეკურსიას ძალიან უჭირს ამხელა მონაცემთან მუშაობა, თუმცა პირველ output-ს სჭირდება დაახლოებით 35 წმ.

    printf("Insert the size of Board: ");
    scanf("%d", &n);
    
    start(1);

    printf("Here is the solution: %d", solution);

    return 0;
}