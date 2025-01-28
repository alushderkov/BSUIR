#include <stdio.h>
#include <stdlib.h>

// За один проход найти 2 минимальных числа

int main() {
    int N;
    printf("N = ");
    scanf("%d",&N);

    int *array = (int *) malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
	    printf("array[%d] = ", i);
	    scanf("%d",&array[i]);
    }

    int min1 = array[0];
    int min2 = array[1];

    for (int i = 2; i < N; i++)
	    if (array[i] < min1 && min1 >= min2)
		    min1 = array[i];
	    else if (array[i] < min2)
            min2 = array[i];

    printf("\nmin1 = %d min2 = %d\n", min1, min2);
    free(array);
    return 0;
}
