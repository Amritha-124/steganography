/*
Name : Amritha G
Date : 07/11/23
Description:Project - Steganography
 */

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"
int main(int argc,char **argv)
{
    int status;
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    

    // Fill with sample filenames
    encInfo.src_image_fname = "beautiful.bmp";
    encInfo.secret_fname = "secret.txt";
    encInfo.stego_image_fname = "stego_img.bmp";
    //checking operation type
     status = check_operation_type(argv);
    if(status == e_encode)
    { 
      printf("selected encoding\n"); 	
    }
    else if(status == e_decode)
    {
        printf("selected decoding\n"); 
    }
    else
    {
        printf("ERROR:\nfor encode--> ./a.out -e <.bmp> <.txt> [optional stego.bmp]\n");
        printf("for deccode--> ./a.out -d <.bmp>  [optional decode.txt]\n");
    }
    // read and validating encode arguments
    if(check_operation_type(argv) == e_encode)
    {
    status = read_and_validate_encode_args(argv,&encInfo);
    if(status == e_success)
    {
	printf("Read and validation of encode arguments is successfull\n");
    }
    else
    {
       printf("FAILURE\n");
       printf("for encode--> ./a.out -e <.bmp> <.txt> [optional stego.bmp]\n");
       return 1;

    }
    
    
     //encoding 
    
   if(do_encoding(&encInfo) == e_success)
   {
      printf("\n>>>>>>> ENCODING SUCCESSFULL <<<<<<<<<\n");

   }
   else
   {
     printf("\n------ERROR:ENCODING FAILED-----------\n");
     return 1;
   } 
    }
    else if(check_operation_type(argv) == e_decode)
    {


   //DECODING

  //read and validating decode arguments
   if(read_and_validate_decode_args(argv,&decInfo) == d_success)
   {
     printf("Read and validation of decode arguments Completed\n");
   }
   else
   {
     printf("Read and validation Failed\n");
     printf("for deccode--> ./a.out -d <.bmp>  [optional decode.txt]\n");
     return 1;
   }
   //decoding process
   if(do_decoding(&decInfo) == d_success)
   {
     printf("********** DECODE SUCCESSFUL **********\n");
   }
   else
   {
     printf("***********DECODE FAILED ***********\n");
     return 1;
   }
    }
    
    return 0;
}
