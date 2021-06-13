#include <stdio.h>
#include "facial_recognition.h"
#include "read_image.c"
#include "directory_content.c"

gsl_vector *vectorize_matrix(gsl_matrix *m)
{
    // Creation of the result vector
    gsl_vector *result = gsl_vector_alloc(m->size1 * m->size2);

    // Iterates each column of the original matrix
    double element, counter = 0;
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
            gsl_vector_set(result, counter, element);

            // Increment position counter
            counter++;
        }
    }
    return result;
}

gsl_matrix *gen_training_set(char *folder_path, int rows, int cols)
{
    // Read filenames that are inside the folderpath
    llist *files = get_directory_content(folder_path);
    int length = list_length(files);

    gsl_matrix *training_set = gsl_matrix_alloc(rows * cols, length);

    for (int i = 0; i < length; i++)
    {
        // Get the filename
        char *filename = (char *)find(files, i);

        // Builds the filepath
        char file[100] = "";
        strcat(file, folder_path);
        strcat(file, "/");
        strcat(file, filename);

        // Reads the image and vectorize it
        gsl_matrix *image = read_image(file);
        gsl_vector *image_vectorized = vectorize_matrix(image);

        // Copies the vectorized image into a column
        gsl_matrix_set_col(training_set, i, image_vectorized);

        // Clean memory
        gsl_matrix_free(image);
        gsl_vector_free(image_vectorized);
        free(filename);
    }

    clean_list(files);

    return training_set;
}

gsl_vector *get_average_face(gsl_matrix *training_set)
{
    gsl_vector *average_face = gsl_vector_alloc(training_set->size1);
    gsl_vector_set_zero(average_face);

    // Sums each column in the trainign set
    for (int j = 0; j < training_set->size2; j++)
    {
        // Get column view of the original matrix
        gsl_vector_view vec_view = gsl_matrix_column(training_set, j);
        gsl_vector *vector = &vec_view.vector;

        // Adds the current column to the global counter
        gsl_vector_add(average_face, vector);
    }

    // Creates a vector to calculate the average
    gsl_vector *div_vector = gsl_vector_alloc(training_set->size1);
    gsl_vector_set_all(div_vector, training_set->size2);

    // Calculates the average
    gsl_vector_div(average_face, div_vector);
    gsl_vector_free(div_vector);

    return average_face;
}

gsl_matrix *sub_average_face(gsl_matrix *training_set, gsl_vector *average_face)
{
    // Sums each column in the trainign set
    for (int j = 0; j < training_set->size2; j++)
    {
        // Get column view of the training_set matrix
        gsl_vector_view vec_view = gsl_matrix_column(training_set, j);
        gsl_vector *vector = &vec_view.vector;

        // Adds the current column to the global counter
        gsl_vector_sub(vector, average_face);
    }

    return training_set;
}

gsl_matrix *transpose(gsl_matrix *matrix)
{
    gsl_matrix *transposed = gsl_matrix_alloc(matrix->size2, matrix->size1);

    for (int j = 0; j < matrix->size2; j++)
    {
        // Get column view of the matrix
        gsl_vector_view vec_view = gsl_matrix_column(matrix, j);
        gsl_vector *vector = &vec_view.vector;

        // Copy the column values into the row
        gsl_matrix_set_row(transposed, j, vector);
    }

    return transposed;
}

gsl_vector *mul_matrix_vector(gsl_matrix *matrix, gsl_vector *vector)
{
    gsl_vector *vec_aux = gsl_vector_alloc(vector->size);
    gsl_vector *result = gsl_vector_alloc(matrix->size1);

    for (int i = 0; i < matrix->size1; i++)
    {
        gsl_vector_view row_view = gsl_matrix_row(matrix, i);
        gsl_vector *row = &row_view.vector;
        
        // Copy the elements of row into vec_aux
        gsl_vector_memcpy(vec_aux, row);

        // Multiply vec_aux and vector
        gsl_vector_mul(vec_aux, vector);

        // Add the values and stores the result
        double sum = 0;
        for (int j = 0; j < vec_aux->size; j++)
        {
            sum += gsl_vector_get(vec_aux, j);
        }
        gsl_vector_set(result, i, sum);
    }

    gsl_vector_free(vec_aux);

    return result;
}

int main()
{
    // gsl_matrix *training_set = gen_training_set("./database", 112, 92);
    gsl_matrix *training_set = gen_training_set("./test", 2, 5);
    int images = training_set->size2;

    gsl_vector *average_face = get_average_face(training_set);
    gsl_matrix *A = sub_average_face(training_set, average_face);

    gsl_vector *work = gsl_vector_alloc(A->size2);
    gsl_vector *S = gsl_vector_alloc(A->size2);
    gsl_matrix *V = gsl_matrix_alloc(A->size2, A->size2);

    gsl_linalg_SV_decomp(A, V, S, work);
    gsl_matrix *U = A;

    gsl_vector *x = gsl_vector_alloc(images);
    gsl_matrix *U_t = transpose(U);

    /*
    gsl_vector *x_i = gsl_vector_alloc(images);
    for (int i = 0; i < U->size2; i++)
    {
    }
    gsl_vector_free(x_i);
    */

    gsl_vector *test = gsl_vector_alloc(3);
    for (int i = 0; i < 3; i++)
    {
        gsl_vector_set(test, i, i);
    }

    gsl_vector* r = mul_matrix_vector(U, test);

    for (int i = 0; i < r->size; i++)
    {
        printf("%f ", gsl_vector_get(r, i));
    }
    

    /*for (int i = 0; i < U->size1; i++)
    {
        for (int j = 0; j < U->size2; j++)
        {
            printf("%f ", gsl_matrix_get(U, i, j));
        }
        printf("\n");
    }*/

    /*for (int i = 0; i < average_face->size; i++)
    {
        double element = gsl_vector_get(average_face, i);
        printf("%f ", element);
    }
    printf("\n");*/

    /*
    for (int j = 0; j < image->size2; j++)
    {
        for (int i = 0; i < image->size1; i++)
        {
            printf("m(%d,%d) = %g\n", i, j,
                   gsl_matrix_get(image, i, j));
        }
    }*/

    gsl_vector_free(x);
    gsl_vector_free(work);
    gsl_vector_free(S);
    gsl_matrix_free(V);
    gsl_matrix_free(U);
    gsl_vector_free(average_face);

    return 0;
}