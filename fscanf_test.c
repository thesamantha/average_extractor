#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


/** Program reads a file with daily temperature averages over an arbitrary number of years and calculates the monthly averages from them. It then writes these averages to a file specified by the user. **/

typedef struct {
	char day[2];
	char month[3];
	int year;
}	Date;

int main () {


	/** Tracking values **/

	size_t nret;
	size_t lineno = 0;
	int nfields = 5;

	/** Read-in values **/
	Date date;
	char temp_main[4];
	char temp_sub[4];
	char garbage[2];


	FILE *data;
	char data_file[30];
	FILE *averages;
	char averages_file[30];

	strcpy(data_file, "leber_niederschlaege.txt");
	strcpy(averages_file, "avg");

		/** Reading data from file **/

		data = fopen(data_file, "r+");		//open file with data to be read
		if (data == NULL)
		{
		  printf("Error opening file '%s'! Please check spelling. Aborting program.\n", data_file);
		  exit(1);
		}

    averages = fopen(averages_file, "w+");    //open file we're writing to
    if (averages == NULL)
    {
      printf("Error opening file '%s'! Aborting program.\n", averages_file);
      exit(1);
    }

		while(!feof(data))		//continue until end of file
		{

			++lineno;
			//parse each line
     while(nfields == (nret = fscanf(data, " %*c %[^.].%[^.].%d %[^,],%s", date.day, date.month, &date.year, temp_main, temp_sub))) {
				
				printf("%s\n", date.day);

				fprintf(averages, "%s/%s/%d Temperature: %s,%s\n", date.day, date.month, date.year, temp_main, temp_sub);
    		++lineno;
      }

			if(ferror(data)) {
				printf("Error reading file '%s'! Aborting program.\n", data_file);
			  exit(1);

			} else if(nret != EOF) {
					fprintf(stderr, "Warning: ignoring malformed line %zu\n", lineno);
		      fscanf(data, "%*[^\n]");
			}
		}

		fclose(data);
		fclose(averages);

		printf("\n\tAverage monthly temperatures successfully calculated and written to file '%s'!\n", averages_file);

	return 0;

}
