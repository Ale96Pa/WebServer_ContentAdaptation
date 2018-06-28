#include "adapt_image.h"

#define DIM 200

FILE *open_file(void)
{
		FILE *f;
		f = fopen("services/file.txt", "a+");

		if(f==NULL)
		{
            fprintf(stderr,"Error in fopen\n");
            exit(EXIT_FAILURE);
		}

		return f;
}

char *check_device(char *device, char *line)
{
		char *p;
		p=strstr(line,device);
		if(p==NULL)
			return p;

		return p;
}

int convert(char **array, int v)
{
		errno=0;
		int n;
		char *endptr;

		n = strtol(array[v],&endptr,0);
		if(errno!=0)
		{
            fprintf(stderr,"Error strtol\n");
            exit(EXIT_FAILURE);
		}

		return n;

}

int *find_value(char *dev, int *value)
{
		char *p;
		int i;
		char *tem;
		char *array[DIM];

		tem=strdup(dev);
		p=strtok(tem,":");
		array[i]=p;

		while(p)
		{
            p=strtok(NULL,"*");
            array[i++]=p;
		}

		array[i++]=NULL;

		value[0]=convert(array,0);
		value[1]=convert(array,1);
}

void read_line(char *device, FILE *f, int *value)
{

	char buff[DIM];
	char *s;
	char *pointer;
	int n[2];

	memset(buff, 0, DIM);
	while(!feof(f))
    {
		if(fgets(buff, DIM, f) == 0)
            break;

		buff[strlen(buff)-1]='\0';
		s = strdup(buff);
		printf("Looking for: %s\n", s);
		pointer = check_device(device,s);
		if(pointer==NULL)
			continue;
		if(pointer != NULL)
		{
			find_value(s,value);
			break;
		}
	}
}

void get_sizes(int dimensions[2])
{
	FILE *f;
	char device[DIM];
	sprintf(device, "%s", "Asus-Laptop");

	f=open_file();
	read_line(device,f,dimensions);

	if(fclose(f) == -1)
    {
        fprintf(stderr, "Error in fclose\n");
        return;
    }
}