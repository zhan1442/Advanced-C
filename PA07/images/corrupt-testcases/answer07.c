#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>
#include"answer07.h"

#define ece264_MAGIC_NUMBER 0x21343632

static int checkValid(ImageHeader * header);
static void printHeader(ImageHeader * header);

Image * Image_load(const char * filename)
{
	FILE * fp = NULL;
	size_t read;
	ImageHeader header;
	Image * im = NULL, * tmp_im = NULL;
	size_t n_bytes = 0;
	int err = 0;

	if (!err) { //open file
		fp = fopen(filename,"rb");
		//(1) Make sure you can open the file
		if(!fp) {
			fprintf(stderr,"Failed to open file '%s'\n", filename);
			err = 1;
		}
	}

	if(!err) { //read the header
		read = fread(&header, sizeof(ImageHeader),1,fp);
		//(2.a) Make sure that you can read all the bytes of the header
		if(read != 1) {
			fprintf(stderr, "Failed to read header from '%s'\n", filename);
			err = 1;
		}
	}

	if(!err) { //check if valid
		if(!checkValid(&header)) {
			fprintf(stderr, "Invalid header in '%s'\n", filename);
			err = 1;
		}
	}

	if(!err){ //print the header
		printHeader(&header);
	}

	if(!err) { // Allocate Image struct
		tmp_im = malloc(sizeof(Image));		//////////////////	
		//(3) Allocate space for the image, comment, and pixels. Remember,
		//malloc will return NULL if it cannot do its job.
		if(tmp_im == NULL) {
			fprintf(stderr, "Failed to allocate im structure\n");
			err = 1;
		}
		tmp_im->comment = malloc(sizeof(char) * header.comment_len);/////////////////
		if(tmp_im->comment == NULL) {
			fprintf(stderr, "Failed to allocate comment\n");
			err = 1;
		}
		n_bytes = sizeof(uint8_t) * header.width * header.height;
		tmp_im->data = malloc(n_bytes);////////////////
		if(tmp_im->data == NULL) {
			fprintf(stderr, "Failed to allocate %zd bytes for image data\n", n_bytes);
			err = 1;
		}

	}

	if(!err) { // Init the Image struct:comment
		tmp_im->width = header.width;
		tmp_im->height = header.height;

		//Read comment
		read = fread(tmp_im->comment,sizeof(char) * header.comment_len,1,fp);
		//(4.a) Make sure you read the entire comment
		if(read != 1) {
			fprintf(stderr, "Failed to read comment from '%s'\n", filename);
			err = 1;
		}
		//(4.b) Make sure the comment ends in a null-byte.
		if(tmp_im->comment[header.comment_len - 1] != '\0') {
			fprintf(stderr,"the comment is not end in a null-byte\n");
			err = 1;
		}
		printf("  comment: %s\n",tmp_im->comment);
	}

	if(!err) { //read pixels
		n_bytes = header.width * header.height;
		read = fread(tmp_im->data, sizeof(uint8_t), n_bytes, fp);
		//(5.a) Make sure you read *all* width*height pixels
		if(n_bytes != read) {
			fprintf(stderr, "Only read %zd of %zd bytes of image data\n", 
					read, n_bytes);
			err = 1;
		}
	}

	if(!err) { //(5.b) We should be at the end of the file now
		uint8_t byte;
		read = fread(&byte, sizeof(uint8_t), 1, fp);
		if(read != 0) {
			fprintf(stderr, "Stray bytes at the end of bmp file '%s'\n",
					filename);
			err = 1;
		}
	}

	if(!err) { 
		im = tmp_im;  // bmp will be returned
		tmp_im = NULL; // and not cleaned up
	}

	// Cleanup
	if(tmp_im != NULL) {
		free(tmp_im->comment); // Remember, you can always free(NULL)
		free(tmp_im->data);
		free(tmp_im);
	}
	if(fp) {
		fclose(fp);
	}

	return im;

}

int Image_save(const char * filename, Image * image)
{
	int err = 0;
	FILE * fp = NULL;
	size_t written = 0;
	size_t n_bytes = 0;

	// Attempt to open file for writing
	fp = fopen(filename, "wb");
	if(fp == NULL) {
		return 0;
	}

	// Prepare the header
	ImageHeader header;
	header.magic_number = ECE264_IMAGE_MAGIC_NUMBER;
	header.width = image->width;
	header.height = image->height; 
	header.comment_len = (image->comment == NULL) ? 1:strlen(image->comment) + 1;
	printf("\ncomment_len = %d\n\n",header.comment_len);

	if(!err) {  // Write the header
		written = fwrite(&header, sizeof(ImageHeader), 1, fp);
		if(written != 1) {
			fprintf(stderr, "Error: only wrote %zd of %zd of file header to '%s'\n", written, sizeof(ImageHeader), filename);
			err = 1;	
		}
	}

	if(!err) {
		written = fwrite(image->comment, sizeof(char) * header.comment_len, 1, fp);
		if(written != 1) {
			fprintf(stderr, "Error:can not write comment\n");
			err = 1;
		}
	}

	if(!err) { //write pixels
		n_bytes = header.width * header.height;
		written = fwrite(image->data, sizeof(uint8_t), n_bytes, fp);
		if(n_bytes != written) {
			fprintf(stderr, "Only write %zd of %zd bytes of image data\n", written, n_bytes);
			err = 1;
		}
	}

	if(fp) fclose(fp);

	return (!err);

}

static int checkValid(ImageHeader * header)
{	
	//(2.b) Make sure that the magic_number in the header is correct
	if(header->magic_number != ece264_MAGIC_NUMBER) return 0;

	//(2.c) Make sure that neither the width nor height is zero
	if(header->width == 0 || header->height == 0) return 0;

	//(2.d) Make sure that the comment length is not zero. (Remember, it
	//includes the null-byte.)
	if(header->comment_len == 1) return 0;

	return 1;
}

static void printHeader(ImageHeader * header)
{
	printf("  Printing 264 header information:\n");
	printf("  file type (should be %x): %x\n", ece264_MAGIC_NUMBER, header->magic_number);
	printf("  width: %d\n", header->width);
	printf("  height: %d\n", header->height);
	printf("  comment_len: %d\n", header->comment_len);
}
