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
	int years;

	long int monthly_totals[12] = {0};		//initialize all values to 0 so they can be incremented
	long int monthly_counts[12] = {0};
	int monthly_avgs[12];
	long int temp_whole;		//decimal long integer to store whole temperature (comprised of temp_main and temp_sub)
	Date date;
	char title[30], time[30], region[30];
	char temp_main[3], temp_sub[3];		//read in components of temperature as characters and then concatenate to form a single integer


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

		while(!feof(data))		//continue until end of file
		{
			++lineno;
			//parse each line
			while(nfields == (nret = fscanf(data, "%s %s	%s %2[^.].%2[^.].%d	%2[^,],%s", title, time, region, date.day, date.month, &date.year, temp_main, temp_sub))) 
			{		
				/** Calculating averages from data **/

				//convert components into one decimal (long) integer
				strcat(temp_main, temp_sub);
				temp_whole = atol(temp_main);

				if(strcmp(date.month, "01") == 0) {
						monthly_totals[0] += temp_whole;
						monthly_counts[0]++;			
				}

				if(strcmp(date.month, "02") == 0) {
						monthly_totals[1] += temp_whole;
						monthly_counts[1]++;			
				}

				if(strcmp(date.month, "03") == 0) {
						monthly_totals[2] += temp_whole;
						monthly_counts[2]++;			
				}

				if(strcmp(date.month, "04") == 0) {
						monthly_totals[3] += temp_whole;
						monthly_counts[3]++;			
				}

				if(strcmp(date.month, "05") == 0) {
						monthly_totals[4] += temp_whole;
						monthly_counts[4]++;			
				}

				if(strcmp(date.month, "06") == 0) {
						monthly_totals[5] += temp_whole;
						monthly_counts[5]++;			
				}

				if(strcmp(date.month, "07") == 0) {
						monthly_totals[6] += temp_whole;
						monthly_counts[6]++;			
				}

				if(strcmp(date.month, "08") == 0) {
						monthly_totals[7] += temp_whole;
						monthly_counts[7]++;			
				}

				if(strcmp(date.month, "09") == 0) {
						monthly_totals[8] += temp_whole;
						monthly_counts[8]++;			
				}

				if(strcmp(date.month, "10") == 0) {
						monthly_totals[9] += temp_whole;
						monthly_counts[9]++;			
				}

				if(strcmp(date.month, "11") == 0) {
						monthly_totals[10] += temp_whole;
						monthly_counts[10]++;			
				}

				if(strcmp(date.month, "12") == 0) {
						monthly_totals[11] += temp_whole;
						monthly_counts[11]++;			
				}
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

		/** Writing averages to file **/


		averages = fopen(averages_file, "w+");		//open file we're writing to
		if (averages == NULL)
		{
		  printf("Error opening file '%s'!\n", averages_file);
		  exit(1);
		}

		double avg_temp;		//double to hold average temperature
    int i = 0;

		for(i; i < 12; i++) {
			//calculate temp
			avg_temp = ((double)monthly_totals[i]/(double)monthly_counts[i])/10;

			fprintf(averages, "%d/%d Average Temperature: %lf\n", i+1, date.year, avg_temp);
		}

		fclose(averages);

		printf("\n\tAverage monthly temperatures successfully calculated and written to file '%s'!\n\t\tWould you like to read another file? (Y/N)\n", averages_file);

		scanf(" %c", &user_choice);
		user_choice = toupper(user_choice);
	}

	return 0;

}
