#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "common.h"
#include "types.h"



/*
   OperationType check_operation_type(char *argv[])
   {
   if(strcmp(argv[1],"-e")==0)
   {
   return e_encode;
   }
   else if(strcmp(argv[1],"-d")==0)
   {
   return e_decode;
   }
   else
   return e_unsupported;

   }  */
//read and validate
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
	decInfo->d_src_image_fname = argv[2];
	printf("read and validation of bmp file is DONE\n");
    }
    else
    {
	printf("ERROR:Read and validation FAILED\n");
	return d_failure;
    }
    if(argv[3] == NULL)
    {
	decInfo->d_secret_fname = "decode.txt";
	printf("Decode file is created as default\n");
    }
    else
    {
	if(strcmp(strstr(argv[3],"."),".txt") == 0)
	{
	    decInfo->d_secret_fname = argv[3];
	    //fopen(argv[3],"r+");
	    printf("%s File is created\n",argv[3]);

	}

    }
    return d_success;

}

//opening files
Status d_open_files(DecodeInfo *decInfo)
{
    decInfo->d_fptr_src_image = fopen(decInfo->d_src_image_fname,"r");
    if(decInfo->d_fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: Unable to open file %s\n", decInfo->d_src_image_fname);
	return d_failure;
    }
    decInfo->fptr_d_secret_file = fopen(decInfo->d_secret_fname,"w");
    if(decInfo->fptr_d_secret_file == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: Unable to open file %s\n", decInfo->d_secret_fname);
	return d_failure;
    }
    return d_success;

}

//decoding
Status do_decoding(DecodeInfo *decInfo)
{
    //open files
    if(d_open_files(decInfo) == d_success)
    {
	printf("opening file completed\n");

    }
    else
    {
	printf("opening files failed\n");
	return d_failure;
    }

    //decoding magic string
    if(decode_magic_string(decInfo) == d_success)
    {
	printf("decoding magic string successfull\n");

    }
    else
    {
	printf("decoding magic_string failed\n");
	return d_failure;
    }

    //decoding file extn size
    if(decode_file_extn_size(strlen(".txt"),decInfo) == d_success)
    {
	printf("decoding file extension size success\n");
    }
    else
    {
	printf("decoding file extension size Failed\n");
	return d_failure;
    }

    //decoding file extn
    if(decode_file_extn(".txt",decInfo) == d_success)
    {
	printf("decoded file extn\n");
    }
    else
    {
	printf("file extn decoding failed\n");
	return d_failure;
    }

    //decoding secret file size
    if(decode_secret_size(decInfo) == d_success)
    {
	printf("decoding secret file size successfull\n");
    }
    else
    {
	printf("decoding secret file size failed\n");
	return d_failure;
    }

    //decoding secret file data
    if(decode_secret_file_data(decInfo) == d_success)
    {
	printf("data decoded successfully\n");
    }
    else
    {
	printf("ERROR:data decode failed\n");
	return d_failure;
    }
    return d_success;
}

//decoding magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->d_fptr_src_image,54,SEEK_SET);
    char image_data[2]; 

    decode_data_from_image(strlen(MAGIC_STRING),decInfo->d_fptr_src_image,image_data);



    if(strcmp(image_data,MAGIC_STRING) == 0)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}


Status decode_data_from_image(int size,FILE *d_fptr_src_image,char *image_data)
{
    char buff[8];
    for(int i=0;i<size;i++)
    {
	fread(buff,1,8,d_fptr_src_image);
	decode_byte_from_lsb(&image_data[i],buff);


    }

    return d_success;

}
Status decode_byte_from_lsb(char *image_data,char *image_buffer)
{
    unsigned char ch = 0x00;

    for(int i=0;i<8;i++)
    {

	ch = ((image_buffer[i] & 0x01) << i) | ch;

    }

    *image_data = ch;

    return d_success;
}
Status decode_file_extn_size(int size,DecodeInfo *decInfo)
{
    char buff[32];
    int long length;


    fread(buff,1,32,decInfo->d_fptr_src_image);
    decode_size_from_lsb(&length,buff);



    if(length == size)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}

Status decode_size_from_lsb(long int *data,char *image_buffer)
{
    int num = 0x00;

    for(int i=0;i<32;i++)
    {

	num = ((image_buffer[i] & 0x01) << i) | num;

    }

    *data = num;
    return d_success;

}

Status decode_file_extn(char *file_extn,DecodeInfo *decInfo)
{
    char d_extn_scrt_file[4]; 
    decode_data_from_image(strlen(file_extn),decInfo->d_fptr_src_image,d_extn_scrt_file);
    if(strcmp(file_extn,d_extn_scrt_file)==0)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}
Status decode_secret_size(DecodeInfo *decInfo)
{
    char buff[32];


    fread(buff,1,32,decInfo->d_fptr_src_image);
    decode_size_from_lsb(&decInfo->size_secret_file,buff);

    return d_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    rewind(decInfo->fptr_d_secret_file);
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
	fread(decInfo->d_image_data,1,8,decInfo->d_fptr_src_image);
	decode_byte_from_lsb(&ch,decInfo->d_image_data);
	fwrite(&ch,1,1,decInfo->fptr_d_secret_file);


    } 
    return d_success;



}
