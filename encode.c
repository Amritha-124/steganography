#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if((strcmp(argv[1],"-e") )== 0)
    {
	return e_encode;  

    }
    else if(strcmp(argv[1],"-d") == 0 )
    {
	return e_decode;
    }
    else
	return e_unsupported;
}


Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)

{


    if(strcmp( strstr(argv[2],"."),".bmp") == 0)
    {
	printf("Read and validation of bmp file successfull\n");
	encInfo->src_image_fname = argv[2];
    }
    else
    {
	printf("Read and validation of bmp file not done\n");
	return e_failure;
    }


    if(strcmp(strstr(argv[3],".") , ".txt")== 0)
    {

	encInfo->secret_fname = argv[3];
	strcpy(encInfo ->extn_secret_file,".txt");
	printf("Read and validation of txt successfull\n");
    }
    else
    {
	printf("Read and validation of txt file not done\n");
	return e_failure;

    }
    if( argv[4] == NULL)
    {
	fopen("stego.bmp","r+");
	printf("Stego File created\n");

    }
    else
    {
	if(strcmp(strstr(argv[4],"."),".bmp")==0)
	{
	    encInfo->stego_image_fname = argv[4];
	    fopen(argv[4],"r+");
	    printf("%s file is created\n",argv[4]);
	}
	else
	{

	    printf("validation successfulll\n");
	}
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
      //opening file
    if(open_files(encInfo) == e_success)
    {
	printf("opening file successfull\n");
    }
    else if(open_files(encInfo) == e_failure)
    {
	printf("opening file failed\n");
	return e_failure;
    }
    //checking capacity
    if(check_capacity(encInfo)==e_success)
    {
	printf("checking capacity Successfull\n");
    } 
    else
    {
	printf("checking capacity failed\n");
	return e_failure;

    }
    //copying header file
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
	printf("copying header file successfull\n");
    }
    else
    {
	printf("copying header file failed\n");
	return e_failure;
    }
    //encoding magic string
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
    {
	printf("Encoding MAGIC STRING successfull\n");
    }
    else
    {
	printf("ERROR:Encoding MAGIC STRING FAILED\n");
	return e_failure;
    }
    //encoding secret file extension size
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
    {
	printf("Encoding secret file extension size successfull\n");
    }
    else
    {
	printf("Encoding secret file extension size failed\n");
	return e_failure;
    }
    //encoding secret file extn
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
    {
	printf("Encoding secret file extension successfull\n");

    }
    else
    {
	printf("ERROR:Encoding secret file extension failed\n");
	return e_failure;
    }
    //encoding secret file size
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
    {
	printf("Encoded secret file size\n");
    }
    else
    {
	printf("ERROR:encoding secret file size failed\n");
	return e_failure;
    }
    //encode secret file data
    if(encode_secret_file_data(encInfo) == e_success)
    {
	printf("Encoded secret file data\n");
    }
    else
    {
	printf("ERROR:Encoding secret file data failed\n");
	return e_failure;
    }
    //copy remaining data
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)== e_success)
    {
	printf("Remaining data copied\n");	
    }
    else
    {
	printf("copying remaining data failed\n");
	return e_failure;
    }
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo ->image_capacity > (strlen(MAGIC_STRING)*8 +32 +(strlen(encInfo->extn_secret_file))*8 +32+encInfo->size_secret_file *8))
    {

	return e_success;

    }
    else
    {

	return e_failure;
    }

}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{

    char buff[54];
    rewind(fptr_src_image);
    fread(buff,1,54,fptr_src_image);
    fwrite(buff,1,54,fptr_stego_image);


    return e_success;

}
Status encode_magic_string(const char*magic_string,EncodeInfo *encInfo)
{
    encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_data_to_image( char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char image_data[8];
    for(int i=0;i<size;i++)
    {
	fread(image_data,1,8,fptr_src_image);
	encode_byte_to_lsb(data[i],image_data);
	fwrite(image_data,1,8,fptr_stego_image);
	
    }
    
    
    return e_success;
}
Status encode_byte_to_lsb(char data,char *image_buffer)
{
    int i;
    char bit;
    for(i=0;i<8;i++)
    {
        
	image_buffer[i] = image_buffer[i] & 0xFE;
	bit = data & 1;
	data = data >>1;
	image_buffer[i] = image_buffer[i] | bit ;
        
    }
    return e_success;
}
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{

    char data_image[32];
    fread(data_image,1,32,encInfo->fptr_src_image);
    encode_size_to_lsb(size,data_image);
    fwrite(data_image,1,32,encInfo->fptr_stego_image);
    return e_success;

}
Status encode_size_to_lsb(int data,char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
	image_buffer[i] = image_buffer[i] & 0xFE;
	int bit = data & 1;
	data = data >>1;
	image_buffer[i] = image_buffer[i] | bit ;

    }
    return e_success;

}
Status encode_secret_file_extn(  char* file_extn,EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)

{
    char secrt_file_size[32];
    fread(secrt_file_size,1,32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,secrt_file_size);
    fwrite( secrt_file_size,1,32,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buff[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(buff,1,encInfo->size_secret_file,encInfo->fptr_secret);
    encode_data_to_image(buff,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_stego)
{
    char ch;
    while(fread(&ch,1,1,fptr_src) != 0)
    {
	fwrite(&ch,1,1,fptr_stego);
    }
    return e_success;
}
