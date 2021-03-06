char *global_path = "storage/images/";

#include "adapt_image.h"

/**
 * This function find the correct path of an image, depending on its name (given in html page)
 * @Param: image's name, variable path set to be used
 * @Return: void (the variable interested is path)
 */
void find_image(char *name, char *path)
{
    int id_name = 0;
    strcpy(path, global_path);

    // Assign an integer ID to each image
    if(strcmp(name, "img1") == 0)
        id_name = 1;
    if(strcmp(name, "img2") == 0)
        id_name = 2;
    if(strcmp(name, "img3") == 0)
        id_name = 3;
    if(strcmp(name, "img4") == 0)
        id_name = 4;
    if(strcmp(name, "img5") == 0)
        id_name = 5;
    if(strcmp(name, "img6") == 0)
        id_name = 6;
    if(strcmp(name, "img7") == 0)
        id_name = 7;
    if(strcmp(name, "img8") == 0)
        id_name = 8;
    if(strcmp(name, "img9") == 0)
        id_name = 9;
    if(strcmp(name, "img10") == 0)
        id_name = 10;

    // Complete the path referring to each image (image's name)
    switch (id_name){
        case 1: strcat(path, "img1.jpg");
                break;
        case 2: strcat(path, "img2.jpg");
                break;
        case 3: strcat(path, "img3.jpeg");
                break;
        case 4: strcat(path, "img4.png");
                break;
        case 5: strcat(path, "img5.jpeg");
                break;
        case 6: strcat(path, "img6.jpg");
                break;
        case 7: strcat(path, "img7.jpeg");
                break;
        case 8: strcat(path, "img8.jpg");
                break;
        case 9: strcat(path, "img9.jpg");
                break;
        case 10: strcat(path, "img10.jpg");
                break;
        default: strcpy(path, "NULL");
                break;
    }
}

/**
 * This function compress the image to a quality q and with a specified format
 * @Param: source name, quality-factor q, folder destination, format of image
 * @Return: void (the image is copied in destination folder)
 */
int compress_image(char *source, double q, char *destination, char *format)
{
    char *path = malloc(sizeof(char)*DIM_PATH);
    find_image(source, path); // The variable path is set

    MagickWand *m_wand = NULL;
    MagickWandGenesis();
    m_wand = NewMagickWand();

    // Read the image
    if(MagickReadImage(m_wand, path) == MagickFalse)
    {
        perror("Error in reading image\n");
        return -1;
        //exit(EXIT_FAILURE);
    }


    // Resize the image using information about device
    /*
    int sizes[2];
    memset(sizes, 0, 2*sizeof(int));
    get_sizes(sizes);
    int width = sizes[0];
    int height = sizes[1];
    MagickResizeImage(m_wand,(size_t)width, (size_t)height,LanczosFilter,1);
    */

    // Set the compression quality
    size_t real_q = (size_t) q*100;
    if(MagickSetImageCompressionQuality(m_wand, real_q) == MagickFalse)
    {
        perror("Error in compressing image\n");
        return -1;
        //exit(EXIT_FAILURE);
    }

    // Set the destination path
    char *name;
    char *real_dest = malloc(sizeof(char)*DIM_PATH);
    strcpy(real_dest, destination);
    if((name = strstr(source, "img")) == NULL)
    {
        perror("Error in finding image's name\n");
        return -1;
        //exit(EXIT_FAILURE);
    }
    strcat(real_dest, name);
    strcat(real_dest, ".");
    strcat(real_dest, format);

    // Write the new image
    if(MagickWriteImage(m_wand, real_dest) == MagickFalse)
    {
        perror("Error in writing and copy image\n");
        return -1;
        //exit(EXIT_FAILURE);
    }

    // Clean up
    if(m_wand)m_wand = DestroyMagickWand(m_wand);
    MagickWandTerminus();
    free(path);
    free(real_dest);
    return 0;
}