//
// Created by ale96 on 07/04/18.
//

//TODO: Adattare dimensioni immagini in base ai dispositivi (usando WURFL)

#include "adapt_image.h"

void compress_image(void)
{
    MagickWand *m_wand = NULL;

    int width,height;

    MagickWandGenesis();

    m_wand = NewMagickWand();
    // Read the image - all you need to do is change "logo:" to some other
    // filename to have this resize and, if necessary, convert a different file
    if(MagickReadImage(m_wand,"/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/images/test1.jpg") == MagickFalse)
    {
        perror("Error in reading image\n");
        exit(EXIT_FAILURE);
    }

    // Get the image's width and height
    width = MagickGetImageWidth(m_wand);
    height = MagickGetImageHeight(m_wand);

    // Cut them in half but make sure they don't underflow
    /*
    if((width /= 2) < 1)width = 1;
    if((height /= 2) < 1)height = 1;
     */

    // Resize the image using information about device
    //MagickResizeImage(m_wand,width/2,height/2,LanczosFilter,1);

    // Set the compression quality to 95 (high quality = low compression)
    if(MagickSetImageCompressionQuality(m_wand,20) == MagickFalse)
    {
        perror("Error in compressing image\n");
        exit(EXIT_FAILURE);
    }

    /* Write the new image */
    if(MagickWriteImage(m_wand,"/home/ale96/Documents/internetWeb/project/Server_ContentAdaptation/pages/test1.jpg") == MagickFalse)
    {
        perror("Error in writing and copy image\n");
        exit(EXIT_FAILURE);
    }

    /* Clean up */
    if(m_wand)m_wand = DestroyMagickWand(m_wand);

    MagickWandTerminus();


}



void main (void){
    compress_image();
}