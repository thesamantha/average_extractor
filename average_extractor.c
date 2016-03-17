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

	char user_choice = 'Y';			//set while-loop condition to allow us to enter the loop


	/** Tracking values **/

	size_t nret;
	size_t lineno = 0;
	int nfields = 5;	
  char current_month[3] = "01";  //track which month we're in to know when to calculate average
	int current_year;

	/** Variables to hold values read from file **/
  long int monthly_sum = 0;   //accumulate temperatures to calculate average
  int monthly_count = 0;
  double avg_temp;    //double to hold average temperature
	long int temp_whole;		//decimal long integer to store whole temperature (composed of temp_main and temp_sub)
	Date date;
  char title[30], time[30], region[30], temp_main[4], temp_sub[3];


	FILE *data;
	char data_file[30];
	FILE *averages;
	char averages_file[30];

	while(user_choice == 'Y') {			//continue until user sets user_choice to 'N'

		printf("Enter name of file to be read: ");
		scanf("%s", data_file);
		printf("Enter name of file to hold averages: ");
		scanf("%s", averages_file);

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
     while(nfields == (nret = fscanf(data, " %*c %2[^.].%2[^.].%d %[^,],%s", date.day, date.month, &date.year, temp_main, temp_sub))) {
				
				/** Calculating averages from data **/
				current_year = date.year;

        if(strcmp(current_month, date.month) != 0) {   //when the month changes

          //calculate average
          avg_temp = roundf((((double)monthly_sum / (double)monthly_count) / 10) * 100) / 100;

					if(strcmp(current_month, "12") == 0) {

						current_year--;
					}

          //write average to file
          fprintf(averages, "%s/%d Average Temperature: %.2lf\n", current_month, current_year, avg_temp);

          //reset values
          strcpy(current_month, date.month);    //setup tracker for next month
          monthly_sum = 0;
          monthly_count = 0;
        }


				//convert components into one decimal (long) integer
				strcat(temp_main, temp_sub);
				temp_whole = atol(temp_main);

        //accumulate monthly values
        monthly_sum += temp_whole;
        monthly_count++;

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

    //snag the last month
    avg_temp = roundf((((double)monthly_sum / (double)monthly_count) / 10) * 100) / 100;
    fprintf(averages, "%s/%d Average Temperature: %.2lf\n", date.month, date.year, avg_temp);

		fclose(data);

		/** Writing averages to file **/


		printf("\n\tAverage monthly temperatures successfully calculated and written to file '%s'!\n\t\tWould you like to read another file? (Y/N)\n", averages_file);

		scanf(" %c", &user_choice);
		user_choice = toupper(user_choice);		//pass user_choice through toupper() so that we can also accept lower-case letters
	}

	return 0;

}
