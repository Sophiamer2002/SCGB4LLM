#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

/**
 * @struct img
 * @brief Represents an image with a fixed 1000x1000 data array.
 * 
 * This structure is used to store image data in a 2D character array.
 * Each image is represented as a 1000x1000 grid of characters.
 */
typedef struct img {
    char data[1000][1000];
} img;

extern void process_image(img *image);

size_t user_input(const char *prompt) {
    size_t n;
    printf("%s", prompt);
    scanf("%zu", &n);
    return n;
}

/**
 * @brief Allocates memory for a specified number of images.
 * 
 * This function allocates memory for an array of `img` structures.
 * 
 * @param cnt The number of images to allocate, is a user input.
 * @return img* A pointer to the allocated memory for the images.
 *              Returns `NULL` if any error occurs during allocation.
 */
img *allocate_images(size_t cnt) {
// BEGIN SOLUTION
    if (cnt != 0 && sizeof(img) > SIZE_MAX / cnt) {
        return NULL;
    }

    return (img *)malloc(cnt * sizeof(img));
// END SOLUTION
}

/**
 * @brief Retrieves an image by its index from the allocated collection.
 * 
 * This function retrieves a pointer to an image at the specified index
 * from the array of images. It ensures that the index is within bounds.
 * 
 * @param images A pointer to the array of images.
 * @param cnt The total number of images in the array.
 * @param index The index of the image to retrieve.
 * @return img* A pointer to the image at the specified index.
 *              Returns `NULL` if the index is out of bounds.
 */
img *get_image(img *images, size_t cnt, int index) {
    if (index < 0 || index >= cnt) {
        return NULL;
    }
    return &images[index];
}

int main()
{
    int cnt = user_input("Enter the number of images: ");
    img *images = allocate_images(cnt);
    
    if (images == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int query = user_input("Enter the image index to query: ");
    img *query_image = get_image(images, cnt, query);
    process_image(query_image);

    return 0;
}