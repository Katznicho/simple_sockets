#include<stdio.h>
#include<stdlib.h>
 int add_patient_file(char *district_name){
 	FILE*patient_file = fopen("add_patient.txt", "w+");
 	char patient_name[200];
	char patient_gender[10];
	char DOI[100];
	char patient_category[10];
	char signs[10];
	char officer_user_name[100];
	 fprintf(patient_file, "DistrictName:\t%s\n", district_name);
	printf("Enter officer_name:");
	scanf("%s", &officer_user_name);
	fprintf(patient_file, "officerName:\t%s\n", officer_user_name);
	
	printf("Enter patient_name: ");
	scanf("%s", &patient_name);
	fprintf(patient_file, "patientName:\t%s\n", patient_name);
	
	printf("Enter patientGender:");
	scanf("%s", &patient_gender);
	fprintf(patient_file,  "patientGender:\t%s\n", patient_gender);
	
	printf("Enter dOI forexample 01/01/2021 11:12 :");
	scanf("%s", &DOI);
	fprintf(patient_file, "DOI:\t\t%s\n", DOI);
	
	
	printf("Is the patient asymptomatic Yes/No?:");
	scanf("%s", &signs);
	fprintf(patient_file, "Asymptoomatic:\t%s\n", signs);
	
	fclose(patient_file);
	return 0;
 	
 }
 char check_character(){
 	char press;
  scanf(" %c", &press);
  return press;
 	
 }
 void display_commands(){
 	//display commands
 	  char commands[5][30] = {"A:ADD_PATIENT", "B:ADD_PATIENT_LIST", "C:CHECK_STATUS:C","D:ADD_PATIENT_FILE.TXT:D", "E:SEARCH_CRITERIA"};
 	puts("List of Commands");
  	int com;
  	for(com=0; com<5; com++){
  		printf("%s\n", commands+com);
	  }
	 // display_commands();
 	
 }
 
int main(){
  int i;
  char district_name[20];
  //displays commands
  display_commands();
  printf("To select a command enter a letter against it:");
  //gets commands
  if(check_character()=='A'){
  	printf("Command :ADD_PATIENT\n");
    printf("Enter districtName:");
	scanf("%s", &district_name);
	//add one patient
	add_patient_file(district_name);  
		
  }
  else if(check_character()=='B'){
  	printf("Command : ADD_PATIENT_LIST\n");
  }
  else if(check_character()=='C'){
  	printf("Command:CHECK_STATUS");
  	
  }
  else if(check_character()=='D'){
  	printf("Command:ADD_PATIENT_FILE.TXT");
  }
  else if(check_character()=='E'){
  	printf("Command:SEARCH_CRITERIA");
  }
  else{
  	printf("No command selected");
  }


  	return 0;	
}
