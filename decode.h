#ifndef DECODE_H
#define DECODE_H 


#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{

    char *d_src_image_fname;
    FILE *d_fptr_src_image;


    char d_image_data[MAX_IMAGE_BUF_SIZE];
    
    

    char *d_secret_fname;
    FILE *fptr_d_secret_file;
    

    long size_secret_file;
    FILE *d_fptr_dest_image;

}DecodeInfo;

//function arguments

//Operation type checking
OperationType check_operation_type(char *argv[]);

//Read and validation of decode arguments from argv
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

//perform the decoding
Status do_decoding(DecodeInfo *decInfo);

//Get file pointers for i/p and o/p files
Status d_open_files(DecodeInfo *decInfo);

//decoding magic string
Status decode_magic_string(DecodeInfo *decInfo);

//decoding data to image
Status decode_data_from_image(int data,FILE *d_fptr_src_image,char *image_data);

//decoding byte from lsb of image data array
Status decode_byte_from_lsb(char *image_data,char *image_buffer);

//file extension size
Status decode_file_extn_size(int size,DecodeInfo *decInfo);

//size to lsb
Status decode_size_from_lsb(long int *data,char *image_buffer);

//decoding file extension
Status decode_file_extn( char *file_extn,DecodeInfo *decInfo);

//decode secret file size
Status decode_secret_size(DecodeInfo *decInfo);

//decoding secret file data
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
