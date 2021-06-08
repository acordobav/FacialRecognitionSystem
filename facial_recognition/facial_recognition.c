#include <stdio.h>
#include "facial_recognition.h"
#include "read_image.c"
#include "directory_content.c"

gsl_vector *vectorize_matrix(gsl_matrix *m)
{
    // Creation of the result vector
    gsl_vector *result = gsl_vector_alloc(m->size1 * m->size2);

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

int main()
{
    /*gsl_matrix *a = gsl_matrix_alloc(2, 5);
    FILE *f = fopen("test.dat", "rb");
    gsl_matrix_fread(f, a);
    fclose(f);

    gsl_vector *r = vectorize_matrix(a);
    gsl_matrix_free(a);

    for (int i = 0; i < r->size; i++)
    {
        int element = gsl_vector_get(r, i);
        printf("%d ", element);
    }*/

    gsl_matrix *training_set = gen_training_set("./database", 112, 92);

    // gsl_vector average_face = get_average_face(training_set);

    return 0;
}