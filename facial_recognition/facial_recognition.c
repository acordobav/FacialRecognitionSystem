#include <stdio.h>
#include "facial_recognition.h"
#include "read_image.c"

int main(void)
{
  /*
  int i, j;
  gsl_matrix * m = gsl_matrix_alloc (10, 3);
  int rows = m->size1;
  int cols = m->size2;
  printf("rows: %d - cols: %d\n", rows, cols);

  for (i = 0; i < 10; i++)
    for (j = 0; j < 3; j++)
      gsl_matrix_set (m, i, j, 0.23 + 100*i + j);

  
  int a = gsl_matrix_get(m, 1, 1);
  printf("%d\n", a);
  
  gsl_matrix_free (m);
  */

  gsl_matrix* m = read_image("test.png");
  int rows = m->size1;
  int cols = m->size2;
  printf("rows: %d - cols: %d\n", rows, cols);

  for (int i = 0; i < m->size1; i++)
  {
    for (int j = 0; j < m->size2; j++)
    {
      int element = gsl_matrix_get(m, i, j);
      printf("%d ", element);
    }
    printf("\n");
  }
  

  gsl_matrix_free(m);

  return 0;
}