//TODO: Adattare dimensioni immagini in base ai dispositivi (usando WURFL ???)
//TODO: aggiungi possibilita' di cambiare formato immagine (jpg, png, jpeg)

char *global_path = "/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/images/";

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

    // Complete the path referring to each image
    switch (id_name){
        case 1: strcat(path, "test1.jpg");
                break;
        case 2: strcat(path, "test2.jpg");
                break;
        case 3: strcat(path, "test3.jpeg");
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
void compress_image(char *source, double q, char *destination, char *format)
{
    char *path = malloc(sizeof(char)*DIM_PATH);
    find_image(source, path); // The variable path is set

    MagickWand *m_wand = NULL;
    size_t width,height;
    MagickWandGenesis();
    m_wand = NewMagickWand();

    // Read the image
    if(MagickReadImage(m_wand, path) == MagickFalse)
    {
        perror("Error in reading image\n");
        exit(EXIT_FAILURE);
    }

    /*
    // Get the image's width and height for resizing (optional)
    width = MagickGetImageWidth(m_wand);
    height = MagickGetImageHeight(m_wand);
    // Cut them in half but make sure they don't underflow
    if((width /= 2) < 1)width = 1;
    if((height /= 2) < 1)height = 1;
    // Resize the image using information about device
    //MagickResizeImage(m_wand,width/2,height/2,LanczosFilter,1);
    */

    // Set the compression quality
    size_t real_q = (size_t) q*100;
    if(MagickSetImageCompressionQuality(m_wand, real_q) == MagickFalse)
    {
        perror("Error in compressing image\n");
        exit(EXIT_FAILURE);
    }

    // Set the destination path
    char *name;
    char *real_dest = malloc(sizeof(char)*DIM_PATH);
    strcpy(real_dest, destination);
    if((name = strstr(source, "img")) == NULL)
    {
        perror("Error in finding image's name\n");
        exit(EXIT_FAILURE);
    }
    strcat(real_dest, name);
    strcat(real_dest, ".");
    strcat(real_dest, format);

    // Write the new image
    if(MagickWriteImage(m_wand, real_dest) == MagickFalse)
    {
        perror("Error in writing and copy image\n");
        exit(EXIT_FAILURE);
    }

    // Clean up
    if(m_wand)m_wand = DestroyMagickWand(m_wand);
    MagickWandTerminus();
    free(path);
    free(real_dest);
}


/*
void main (void)
{
    char *path = malloc(sizeof(char)*100);
    char *dest = "/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages/";
    compress_image("img1", 0.8, dest, "jpg");
    free(path);

}
 */