#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#define MAX_PATIENTS 100
int patients_number = 0;
//file
FILE *patient_file;

//structure
typedef struct Patients
{
    char patient_name[50];
    char patient_gender[50];
    char DOI[100];
    char status[100];
} patient;
//officer name
char officer_name[50];
char district_name[30];

//socket globals
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];
portno = 3007;

//create global structure
patient patients[MAX_PATIENTS];

int display_commands()
{
    char commands[5][30] =
        {"1:ADD_PATIENT", "2:ADD_PATIENT_LIST_",
         "3:CHECK_STATUS", "4:ADD_PATIENT_FILE.TXT", "5:SEARCH_CRITERIA"};
    int next_command;
    puts("============Lists of Commands=========\t");
    for (next_command = 0; next_command < 5; next_command++)
    {
        puts(commands[next_command]);
    }
    int selected_command;
    printf("\nSelectCommand:");
    scanf("%d", &selected_command);
    return selected_command;
}
//patient functions
void add_patient()
{
    //struct
    //set up some condition to stop

    printf("OfficerName:");
    scanf("%s", officer_name);
    char stop;
    int loop;
    puts("\t\n=========ENTER_PATIENT_DETAILS===========\n");
    for (loop = patients_number; loop <= MAX_PATIENTS; loop++)
    {
        puts("PatientName");
        scanf("%s", patients[loop].patient_name);
        puts("PatientGender");
        scanf("%s", patients[loop].patient_gender);
        puts("Date:forexample 01/25/2021 or 01-25-2021");
        scanf("%s", patients[loop].DOI);
        puts("Asymptomatic/NotAsymptomatic");
        scanf("%s", patients[loop].status);
        patients_number += 1;
        puts("More patients y/n?");
        scanf(" %c", &stop);
        if (stop == 'n' || stop == 'N')
        {
            break;
        }
        puts("\n========NEXT_PATIENT========\n");
    }
}
//patient_list
void add_patient_list()
{
    if (patients_number == 0)
    {
        printf("\nAlert !! no patients yet !!\n");
        get_command(display_commands());
    }
    else
    {
        patient_file = fopen("patient_file.txt", "a+");
        //loop
        if (patient_file == NULL)
        {
            printf("\nFile not found We are sorry\n");
        }
        else
        {
            //loop
            int i;
            for (i = 0; i < patients_number; i++)
            {
                fprintf(patient_file, "%s\t\t%s\t%s\t%s\n", patients[i].patient_name, patients[i].DOI, patients[i].patient_gender, officer_name);
            }
            printf("\n====== All Patients Added=====\n");
            printf("\n");
            patients_number = 0;
            fclose(patient_file);
        }
    }
}

void check_status()
{
    patient_file = fopen("patient_file.txt", "r");
    if (patient_file == NULL)
    {
        puts("\nFiles does not exit\n");
        get_command(display_commands());
    }
    else
    {
        char store[200];
        int num_of_cases = 0;
        while (fgets(store, 100, patient_file) != NULL)
        {
            num_of_cases++;
        }
        num_of_cases == 1 ? printf("\nThere is %d case stored in the file", num_of_cases)
                          : printf("\nThere are %d cases stored in the file\n", num_of_cases);
    }
}
void search_details()
{
    //date/name
    patient_file = fopen("patient_file.txt", "r");
    char search_name[50];
    printf("Search by name or date(01/01/2021 or 01-01-2021):");
    scanf("%s", search_name);
    char store[200];

    int records = 0;
    int total_records = 0;
    puts("\n===================================================================");
    puts("PatientName\t\tDate\t\tGender\t\tOfficerName");
    puts("=====================================================================");
    while (fgets(store, 100, patient_file) != NULL)
    {
        total_records++;
        if (strstr(store, search_name) != NULL)
        {
            puts(store);
            records++;
        }
    }

    if (records == 0)
        printf("\nNo records found please search well\n");
    else
    {
        int i = 0;
        for (i = 0; i <= total_records; i++)
        {
            if (records == i)
            {
                int get_available = records;
                get_available == 1 ? printf("\n%d record available out of %d\n", get_available, total_records)
                                   : printf("\n%d records available out of %d\n", get_available, total_records);
            }
        }
    }
    printf("\n");
}
void send_patient_file()
{
    patient_file = fopen("patient_file.text", "r");
    if (patient_file == NULL)
    {
        printf("\nFile does not exist");
    }
    // create socket and get file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname("127.0.0.1");

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    //connect to server with server address which is set above (serv_addr)

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR while connecting");
        exit(1);
    }

    //implement connections
    while (1)
    {
        printf("What do you want to say? ");
        //making sure variable buffer is initiallized to zero.
        bzero(buffer, 256);

        //Input that goes to the server is obtained and stored in buffer.
        //scanf("%s", buffer);
        fgets(buffer, 255, patient_file);
        //gets(buffer);
        n = write(sockfd, buffer, strlen(buffer));

        if (n < 0)
        {
            perror("ERROR while writing to socket");
            exit(1);
        }

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);

        if (n < 0)
        {
            perror("ERROR while reading from socket");
            exit(1);
        }
        printf("server replied: %s \n", buffer);

        // escape this loop, if the server sends message "quit"

        if (!bcmp(buffer, "quit", 4))
            break;
    }
}
//patients functions
//returns a commad
int get_command(int selected_command)
{
    if (selected_command == 1)
    {
        //f
        add_patient();
        printf("\n");
        return 0;
    }
    else if (selected_command == 2)
    {
        add_patient_list();
        printf("\n");
        return 0;
    }
    else if (selected_command == 3)
    {
        check_status();
        printf("\n");
        return 0;
    }
    else if (selected_command == 4)
    {
        //add file
        //connect
        send_patient_file();
        return 0;
    }
    else if (selected_command == 5)
    {
        search_details();
        printf("\n");
    }
    else
    {
        //printf
        //display_commands();
        puts("\nInvalid Command!! Enter a valid Command:\n");
        get_command(display_commands());
    }
    return 0;
}
int main(int argc, char *argv[])
{
    //fopen("patient_file.txt", "a+");
    printf("Enter district name forexamle Kampala: ");

    scanf("%s", district_name);
    while (1)
    {
        get_command(display_commands());
        continue;
    }

    char username[100], password[100];
    // //Prompt user to enter district
    // puts("   Welcome to our covid system   ");
    // puts("  Please enter username ");
    // gets(username);
    // puts("Enter password now!");
    // gets(password);

    // create socket and get file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname("127.0.0.1");

    // if (server == NULL) {
    //     fprintf(stderr,"ERROR, no such host\n");
    //     exit(0);
    // }

    // bzero((char *) &serv_addr, sizeof(serv_addr));
    // serv_addr.sin_family = AF_INET;
    // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    // serv_addr.sin_port = htons(portno);

    // connect to server with server address which is set above (serv_addr)

    // if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    //     perror("ERROR while connecting");
    //     exit(1);
    // }
    // FILE *fileHd = fopen("patients.txt","r");
    // // inside this while loop, implement communicating with read/write or send/recv function
    // while (1) {
    //     printf("What do you want to say? ");
    //     //making sure variable buffer is initiallized to zero.
    //     bzero(buffer,256);

    //     //Input that goes to the server is obtained and stored in buffer.
    //     //scanf("%s", buffer);
    //     fgets(buffer,255,fileHd);
    // 	//gets(buffer);
    //     n = write(sockfd,buffer,strlen(buffer));

    //     if (n < 0){
    //         perror("ERROR while writing to socket");
    //         exit(1);
    //     }

    //     bzero(buffer,256);
    //     n = read(sockfd, buffer, 255);

    //     if (n < 0){
    //         perror("ERROR while reading from socket");
    //         exit(1);
    //     }
    //     printf("server replied: %s \n", buffer);

    //     // escape this loop, if the server sends message "quit"

    //     if (!bcmp(buffer, "quit", 4))
    //         break;
    // }
    return 0;
}
