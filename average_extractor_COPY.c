#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/** Program reads a file with daily temperature averages over an entire year and calculates the monthly averages from them. It then writes these averages to a file specified by the user. **/

typedef struct {
	char day[3];
	char month[3];
	int year;
}	Date;

int main () {

	char user_choice = 'Y';

	size_t nret;
	size_t lineno = 0;
	int nfields = 8;	
	int current_month = 1;    //start with January
  double avg_temp;

	long int monthly_totals[12] = {0};		//initialize all values to 0 so they can be incremented
	long int monthly_counts[12] = {0};
  long int monthly_total = 0;
  long int monthly_count = 0;
	int monthly_avgs[12];
	long int temp_whole;		//decimal long integer to store whole temperature (comprised of temp_main and temp_sub)
	Date date;
	char title[30], time[30], region[30], temp_main[3], temp_sub[3];



	FILE *data;
	char data_file[30];
	FILE *averages;
	char averages_file[30];

	while(user_choice == 'Y') {

		printf("Enter name of file to be read: ");
		scanf("%s", data_file);
		printf("Enter name of file to hold averages: ");
		scanf("%s", averages_file);
		//printf("Enter how many years you want to calculate: ");
		//scanf("%d", &years);

		/** Reading data from file **/

		data = fopen(data_file, "rw+");		//open file with data to be read
		if (data == NULL)
		{
		  printf("Error opening file '%s'! Please check spelling.\n", data_file);
		  exit(1);
		}

    averages = fopen(averages_file, "w+");    //open file we're writing to
    if (averages == NULL)
    {
      printf("Error opening file '%s'!\n", averages_file);
      exit(1);
    }


		while(!feof(data))		//continue until end of file
		{
			++lineno;

			//parse each line
			while(nfields == (nret = fscanf(data, "%s %s	%s %2[^.].%2[^.].%d	%2[^,],%s", title, time, region, date.day, date.month, &date.year, temp_main, temp_sub))) 
			{		
			
        current_month = atoi(date.month);

				//convert components into one decimal (long) integer
				strcat(temp_main, temp_sub);
				temp_whole = atol(temp_main);

				++lineno;
			}
  
       while(atoi(date.month) == current_month) {       //essentially we want to accumulate all the temperatures for one month, and then, upon the month changing, calculate the average and write it to file.
          //this needs to happen in a loop which also eventually changes the date.month....
          monthly_total += temp_whole;
          monthly_count++;
          printf("hier sind wir\n");
        }

        //calculate average
        avg_temp = ((double)monthly_total/(double)monthly_count)/10;
        fprintf(averages, "%s/%d Average Temperature: %lf\n", date.month, date.year, avg_temp);

        monthly_count = 0;    //reset for next month
        monthly_total = 0;
  
        current_month = atoi(date.month);   //set current month to new month

			  if(ferror(data)) {
			    printf("Error reading file '%s'! Aborting program.\n", data_file);
			    exit(1);

  			} else if(nret != EOF) {
	  				fprintf(stderr, "Warning: ignoring malformed line %zu\n", lineno);
		        fscanf(data, "%*[^\n]");
		  	}
		}

		fclose(data);

		/** Writing averages to file **/

		fclose(averages);

		printf("\n\tAverage monthly temperatures successfully calculated and written to file '%s'!\n\t\tWould you like to read another file? (Y/N)\n", averages_file);

		scanf(" %c", &user_choice);
		user_choice = toupper(user_choice);
	}

	return 0;

}
