//Ben Johnston - 2432411j - SP Exercise 1a
//This is my own work as defined in the Academic Ethics agreement I have signed.

#include<stdio.h>
#include"date.h"
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

//Declaring date structure
struct date{int dd,mm,yyyy;}; 

//Validates date is 10 characters, slashes are in right position and the rest of the
//characters are digits, if anything fails,including Malloc, return NULL
//Then convert the characters to longs and assign them to date values
//return the created date
Date * date_create(char *datestr){
	if(strlen(datestr) != 10){
		return NULL;
	}
	if(datestr[2]!='/' && datestr[5]!='/'){
		return NULL;
	}
	int index[]={0,1,3,4,6,7,8,9};
	for(int i=0; i<7; i++){
		if(!(isdigit(datestr[index[i]]))){
			return NULL;
		}
	}
	Date* d= (Date*) malloc(sizeof(Date));
	if(!d){
                fprintf(stderr, "Syntax Error\n");
                return NULL;
        }
	d->dd=strtol(datestr,&datestr,10);
	++datestr;
	d->mm=strtol(datestr,&datestr,10);
	++datestr;
	d->yyyy=strtol(datestr,&datestr,10);
	return d;
}

//Creates a duplicate of a given date and returns this duplicate
//If Malloc fails return NULL
Date * date_duplicate(Date *d){
	Date * dupe = (Date*) malloc(sizeof(Date));
	if(!dupe){
		fprintf(stderr, "Memory Allocation Failure\n");
		return NULL;
	}
	dupe->dd=d->dd;
	dupe->mm=d->mm;
	dupe->yyyy=d->yyyy;	
	return dupe;
}

//Compares date values from two dates
//Returns -1 if date1 < date2
//Returns 0 if date1 = date2
//Returns 1 if date1 > date2
int date_compare(Date *date1, Date *date2){
	if(date1->yyyy < date2->yyyy){
		return -1;
	}
	if(date1->yyyy > date2->yyyy){
		return 1;
	}
	if(date1->yyyy == date2->yyyy){
		if(date1->mm < date2->mm){
			return -1;
		}	
		if(date1->mm > date2->mm){
			return 1;
		}
		if(date1->dd < date2->dd){
			return -1;
		}
		if(date1->dd > date2->dd){
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

//Free the date
void date_destroy(Date *d){
	free(d);
}

