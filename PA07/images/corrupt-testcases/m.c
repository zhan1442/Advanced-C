#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "answer07.h"

int main(int argc, char * * argv)
{
	Image * image = NULL;
	image = Image_load(argv[1]);
	printf("width = %d\n",image->width);
	printf("height = %d\n",image->height);
	printf("comment = %s\n",image->comment);
	Image_save(argv[2],image);

	free(image->comment);
	free(image->data);
	free(image);
	
	return EXIT_SUCCESS;
}
