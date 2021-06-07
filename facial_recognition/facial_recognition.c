#include <stdio.h>
#include "facial_recognition.h"
#include "read_image.c"

gsl_matrix *vectorize_matrix(gsl_matrix *m)
{
    // Creation of the result matrix
    int rows = m->size1 * m->size2;
    int cols = 1;
    gsl_matrix *result = gsl_matrix_alloc(rows, cols);

    // Iterates each column of the original matrix
    int element, counter = 0;
    for (int j = 0; j < m->size2; j++)
    {
        // Get column view of the original matrix
        gsl_vector_view vec_view = gsl_matrix_column(m, j);
        gsl_vector *vector = &vec_view.vector;

        // Iterates each element in the column
        for (int i = 0; i < vector->size; i++)
        {
            // Get column element
            element = gsl_vector_get(vector, i);

            // Stores element in the result matrix
            gsl_matrix_set(result, counter, 0, element);

            // Increment position counter
            counter++;
        }
    }
    return result;
}

int main()
{
    gsl_matrix *a = gsl_matrix_alloc(2, 5);
    FILE *f = fopen("test.dat", "rb");
    gsl_matrix_fread(f, a);
    fclose(f);

    gsl_matrix *r = vectorize_matrix(a);
    gsl_matrix_free(a);

    for (int i = 0; i < r->size1; i++)
    {
        for (int j = 0; j < r->size2; j++)
        {
            int element = gsl_matrix_get(r, i, j);
            printf("%d ", element);
        }
        printf("\n");
    }

    return 0;
}