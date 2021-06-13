#include <stdio.h>
#include "facial_recognition.h"
#include "handle_image.c"
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

gsl_matrix *get_coordinate_matrix(gsl_matrix *U_t, gsl_matrix *A)
{
    gsl_matrix *coordinate_matrix = gsl_matrix_alloc(U_t->size1, U_t->size1);

    for (int j = 0; j < A->size2; j++)
    {
        // Get column view of the matrix
        gsl_vector_view col_view = gsl_matrix_column(A, j);
        gsl_vector *column = &col_view.vector;

        // Multiply vector and column
        gsl_vector *x_i = mul_matrix_vector(U_t, column);

        // Copy result
        gsl_matrix_set_col(coordinate_matrix, j, x_i);

        gsl_vector_free(x_i);
    }

    return coordinate_matrix;
}

void write_matrix(gsl_matrix *matrix, char *filename)
{
    FILE *f = fopen(filename, "wb");
    gsl_matrix_fwrite(f, matrix);
    fclose(f);
}

void read_matrix(gsl_matrix *matrix, char *filename)
{
    FILE *f = fopen(filename, "rb");
    gsl_matrix_fread(f, matrix);
    fclose(f);
}

void write_vector(gsl_vector *vector, char *filename)
{
    FILE *f = fopen(filename, "wb");
    gsl_vector_fwrite(f, vector);
    fclose(f);
}

void read_vector(gsl_vector *vector, char *filename)
{
    FILE *f = fopen(filename, "rb");
    gsl_vector_fread(f, vector);
    fclose(f);
}

void write_int(int i, char *filename)
{
    FILE *f = fopen(filename, "wb");
    fwrite(&i, sizeof(int), 1, f);
    fclose(f);
}

int read_int(char *filename)
{
    int i = 0;
    FILE *f = fopen(filename, "rb");
    fread(&i, sizeof(int), 1, f);
    fclose(f);
    return i;
}

void analyze_database(char *folderpath, int rows, int cols)
{
    gsl_matrix *training_set = gen_training_set(folderpath, rows, cols);
    write_matrix(training_set, "training_set.dat");

    int images = training_set->size2;
    write_int(images, "total_images.dat");

    gsl_vector *average_face = get_average_face(training_set);
    write_vector(average_face, "average_face.dat");

    gsl_matrix *A = sub_average_face(training_set, average_face);

    gsl_matrix *U = gsl_matrix_alloc(A->size1, A->size2);
    gsl_matrix_memcpy(U, A);

    gsl_vector *work = gsl_vector_alloc(A->size2);
    gsl_vector *S = gsl_vector_alloc(A->size2);
    gsl_matrix *V = gsl_matrix_alloc(A->size2, A->size2);

    gsl_linalg_SV_decomp(U, V, S, work);

    gsl_matrix *U_t = transpose(U);
    write_matrix(U_t, "U_transpose.dat");

    gsl_matrix *coordinate_matrix = get_coordinate_matrix(U_t, A);

    write_matrix(coordinate_matrix, "coordinates_face_subspace.dat");

    gsl_vector_free(work);
    gsl_vector_free(S);
    gsl_matrix_free(V);
    gsl_matrix_free(U);
    gsl_matrix_free(U_t);
    gsl_vector_free(average_face);
    gsl_matrix_free(coordinate_matrix);
}

int compare(char *filepath, int rows, int cols, int e1, int e0, int max_mismatch)
{
    // Load total images number
    int images = read_int("total_images.dat");

    // Load the training set
    gsl_matrix *training_set = gsl_matrix_alloc(rows * cols, images);
    read_matrix(training_set, "training_set.dat");

    // Read the image
    gsl_matrix *image = read_image(filepath);

    // Vectorize the image
    gsl_vector *vectorized_image = vectorize_matrix(image);

    // Load average face
    gsl_vector *average_face = gsl_vector_alloc(rows * cols);
    read_vector(average_face, "average_face.dat");

    // Load U tranpose
    gsl_matrix *U_t = gsl_matrix_alloc(images, rows * cols);
    read_matrix(U_t, "U_transpose.dat");

    // Load coordinates_face_subspace
    gsl_matrix *coordinates_fs = gsl_matrix_alloc(images, images);
    read_matrix(coordinates_fs, "coordinates_face_subspace.dat");

    // Calculate U
    gsl_matrix *U = transpose(U_t);

    // Calculate coordinate vector
    gsl_vector *distance = gsl_vector_alloc(vectorized_image->size);
    gsl_vector_memcpy(distance, vectorized_image);
    gsl_vector_sub(distance, average_face);

    gsl_vector *coordinate = mul_matrix_vector(U_t, distance);

    // Calculate projection vector
    gsl_vector *projection = mul_matrix_vector(U, coordinate);

    // Calculate distance to face space
    gsl_vector_sub(distance, projection);
    double ef = gsl_blas_dnrm2(distance);

    if (ef > e1) {
        printf("Image is not a face! | ef = %f\n", ef);
        return -1; // Image is not a face
    }

    // Compares the input image against the training set
    gsl_vector *comparisons = gsl_vector_alloc(images);
    gsl_vector *e_i = gsl_vector_alloc(coordinate->size);
    for (int j = 0; j < images; j++)
    {
        // Get column view of the coordinates face space
        gsl_vector_view col_view = gsl_matrix_column(coordinates_fs, j);
        gsl_vector *column = &col_view.vector;

        // Copy image coordinate vector
        gsl_vector_memcpy(e_i, coordinate);

        // Compares the image agains the face space
        gsl_vector_sub(e_i, column);
        double norm = gsl_blas_dnrm2(e_i);

        // Stores the result
        gsl_vector_set(comparisons, j, norm);
    }


    // Checks if it is a known face
    int known_face = 0;
    for (int i = 0; i < comparisons->size; i++)
    {
        double e_i = gsl_vector_get(comparisons, i);

        if(e_i > e0) known_face++;
        // printf("%f ", e_i);
    }
    // printf("\n%d\n", known_face);

    // Gets the comparison with the minimum value
    int index = gsl_vector_min_index(comparisons);
    rebuild_coincidence(training_set, index, rows, cols);

    if (known_face > max_mismatch) {
        printf("Input image is an unknown face!\n");
        return -2; // Face is unknown
    }
    


    gsl_vector_free(e_i);
    gsl_matrix_free(U_t);
    gsl_matrix_free(U);
    gsl_matrix_free(image);
    gsl_matrix_free(coordinates_fs);
    gsl_matrix_free(training_set);
    gsl_vector_free(coordinate);
    gsl_vector_free(distance);
    gsl_vector_free(projection);
    gsl_vector_free(average_face);
    gsl_vector_free(vectorized_image);
    gsl_vector_free(comparisons);

    printf("Known face!\n");
    return 0;

}

void rebuild_coincidence(gsl_matrix *training_set, int index, int rows, int cols)
{
    // Get original vectorized image
    gsl_vector_view vec_image_view = gsl_matrix_column(training_set, index);
    gsl_vector *vec_image = &vec_image_view.vector;

    gsl_matrix *image = gsl_matrix_alloc(rows, cols);

    int counter = 0;
    for (int j = 0; j < cols; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            double pixel = gsl_vector_get(vec_image, counter) * 255;
            gsl_matrix_set(image, i, j, pixel);
            counter++;
        }
    }

    write_image("coincidence.png", image);
    gsl_matrix_free(image);
}

int main()
{
    int e1 = 21;
    int e0 = 20;
    int max_mismatch = 5;
    //char *folderpath = "./test";
    //int rows = 2;
    //int cols = 5;

    char *folderpath = "./database";
    int rows = 112;
    int cols = 92;

    analyze_database(folderpath, rows, cols);

    //compare("./test/1.png", 2, 5);
    //compare("./person1.png", rows, cols, e1, e0, max_mismatch);
    compare("./test.png", rows, cols, e1, e0, max_mismatch);

    return 0;
}