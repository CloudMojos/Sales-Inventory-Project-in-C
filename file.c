#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX 99

// Global Variables
FILE *services, *record, *report, *dreport, *menu, *newServices, *apick, *spick, *unfRec, *sspick, *sps, *aapick, *prp;

// Function Prototypes
void Normal_Transaction(FILE *x, FILE *y, FILE *z);
void Advanced_Transaction(FILE *x, FILE *y, FILE *z);
void Generate_Report();
void File_Management();

// A prototype for processing transaction
void Process_Transaction();
void Auto_Process();

int main()
{
    time_t rawtime;
    struct tm * timeinfo;

    rawtime = time (NULL);
    timeinfo = localtime ( &rawtime );
    int tm_day = timeinfo->tm_mday, tm_mon = timeinfo->tm_mon + 1, tm_year = timeinfo->tm_year - 100;
    char date[MAX];

    Auto_Process();

    sprintf(date, "Day_Reports/%d.%02d.%d.txt", tm_day, tm_mon, tm_year);
    dreport = fopen(date, "a");
    record = fopen("record.txt", "a");
    unfRec = fopen("Ref_Files/unfRec.txt","a");
    printf("\t\tMenu\n");
    int menuChoice = -1;
    char option[MAX];
    while (menuChoice != 5)
    {
        menu = fopen("Ref_Files/menu.txt", "r");
        int menucounter = 1;
        printf("\n\n");
        while (fgets(option, MAX, menu) != NULL)
        {
            printf("\t|%i| %s", menucounter, option);
            menucounter++;
        }
        menuChoice = -1;

        while (menuChoice > 5 || menuChoice < 0)
        {
            printf("\n\nEnter your choice: ");
            char term = 0;
            if(scanf("%1d%c", &menuChoice, &term) != 2 || term != '\n')
            {
                 printf("Encountered an error. Please try again.\n");
                 return 1;
            }
            else
            {
                switch (menuChoice)
                {
                    case 1:
                        Normal_Transaction(services, record, dreport);
                        break;
                    case 2:
                        Advanced_Transaction(services, apick, spick);
                        break;
                    case 3:
                        Generate_Report();
                        break;
                    case 4:
                        File_Management();
                        break;
                    case 5:
                        printf("Thank you!\n");
                        break;
                    default:
                        printf("Out of range");
                        break;
                }
            }
        }
    }
}

void Normal_Transaction(FILE* x, FILE* y, FILE* z)
{
    services = fopen("Ref_Files/services.txt", "r");
    report = fopen("report.txt", "w");
    record = fopen("record.txt", "a");
    char service[MAX][MAX], atype[MAX][MAX];
    float cost[MAX], price = 0, total_price = 0;
    int quantity;
    printf("\n\tNormal Transaction\n");
    int serviceCounter = 0;
    while (fscanf(services, "%f %[^\t] %[^\n]", &cost[serviceCounter],  service[serviceCounter], atype[serviceCounter]) != EOF)
    {
        printf("|%i %-17s |\t| %05.2f per %s \n", serviceCounter + 1, service[serviceCounter], cost[serviceCounter], atype[serviceCounter]);
        serviceCounter++;
    }
    int normalChoice = 0;
    while(normalChoice < 1 || normalChoice > serviceCounter)
    {
        char boolean = 'a';
        char boolean1 = 'a';
        if (normalChoice < serviceCounter)
        {
            printf("Enter the type of service: ");
            char term = 0;
            if(scanf("%1d%c", &normalChoice, &term) != 2 || term != '\n')
            {
                 printf("Encountered an error. Please try again.\n");
                 break;
            }
            if (normalChoice > serviceCounter || normalChoice < 1)
            {
                normalChoice = 0;
            }
        }
        for (int i = 0; i < serviceCounter; i++)
        {
            if (i == normalChoice - 1)
            {
                printf("\nYou picked : %s\n", service[i]);
                printf("%s costs %0.2f. Do you want to continue?(y/n)\n", service[i], cost[i]);
                while (tolower(boolean) != 'y' && tolower(boolean) != 'n')
                    scanf("%c", &boolean);
                if (tolower(boolean) == 'y')
                {
                    printf("Enter the amount : ");
                    scanf("%i", &quantity);
                    price = cost[i] * quantity;
                    total_price += cost[i] * quantity;
                    printf("The price is : %0.2f\n", price);

                    fprintf(record, "| %-17s |\t| %-8i |\t| %-8.2f | \n", service[i], quantity, price);
                    fprintf(report, "| %-17s |\t| %-8i |\t| %-8.2f | \n", service[i], quantity, price);
                    fprintf(dreport, "| %-17s |\t| %-8i |\t| %-8.2f | \n", service[i], quantity, price);
                    fprintf(unfRec,"%i\t%f\t%s\n",quantity, price, service[i]);
                    printf("Do you want another service? (y/n)\n");
                    while (tolower(boolean1) != 'y' && tolower(boolean1) != 'n')
                        scanf("%c", &boolean1);
                    if (tolower(boolean1) == 'y')
                        normalChoice = 0;
                }

                else if (tolower(boolean) == 'n')
                {
                    normalChoice = MAX;
                    printf("Do you want another service? (y/n)\n");
                    while (tolower(boolean1) != 'y' && tolower(boolean1) != 'n')
                        scanf(" %c", &boolean1);
                    if (tolower(boolean1) == 'y')
                        normalChoice = 0;
                    else if (tolower(boolean1) == 'n')
                    {
                        normalChoice = 1;
                        break;
                    }
                }
            }
        }
    }
    char* Total = "Total";
    fprintf(report, "| %-25s|\t\t| %-8.2f | \n", Total, total_price);
}

void Advanced_Transaction(FILE *x, FILE *y, FILE *z)
{

    services = fopen("Ref_Files/services.txt", "r");
    apick = fopen("autopick.txt", "a+");
    spick = fopen("manualpick.txt", "a+");
    char service[MAX][MAX], atype[MAX][MAX];
    float cost[MAX], price = 0, total_price = 0;
    int quantity, day = 0, mon = 0, year = 0;
    printf("\nAdvanced Transaction\n");
    int advancedChoice = 0;
    while (advancedChoice < 1 || advancedChoice > 2)
    {
        printf("|1 Transact                      |\n");
        printf("|2 Process Scheduled Transaction |\n");
        printf("|3 Quit                          |\n");
        printf("Enter your choice: ");
        char term = 0;
        if(scanf("%1d%c", &advancedChoice, &term) != 2 || term != '\n')
        {
             printf("Encountered an error. Please try again.\n");
             break;
        }
        int advancedChoice1 = 0;
        if (advancedChoice == 1)
        {
            printf("\n Transact\n");
            printf("|1 Automatic Transaction |\n");
            printf("|2 Scheduled Transaction |\n");
            printf("|3 Quit                  |\n");
            printf("Enter your choice: ");
            scanf("%d", &advancedChoice1);
            int advancedChoice2 = 0;

            // Automatic pickup
            if (advancedChoice1 == 1)
            {
                printf("\n Transact -> Automatic Transaction \n");
                printf("\n");
                int serviceCounter = 0;
                while (fscanf(services, "%f %[^\t] %[^\n]", &cost[serviceCounter],  service[serviceCounter], atype[serviceCounter]) != EOF)
                {
                    printf("|%i %-17s |\t| %05.2f per %s  \n", serviceCounter + 1, service[serviceCounter], cost[serviceCounter], atype[serviceCounter]);
                    serviceCounter++;
                }
                printf("\n");
                while (advancedChoice2 < 1 || advancedChoice2 > serviceCounter)
                {
                    char boolean = 'a';
                    char boolean1 = 'a';
                    if (advancedChoice2 < serviceCounter)
                    {
                        printf("Enter date of pick-up in dd-mm-yy\n");
                        printf("Day: ");
                        scanf("%d", &day);
                        printf("Month: ");
                        scanf("%d", &mon);
                        printf("Year: ");
                        scanf("%d", &year);

                        printf("Enter the type of service: ");
                        scanf("%i", &advancedChoice2);
                        if (advancedChoice2 > serviceCounter || advancedChoice2 < 1)
                        {
                            advancedChoice2 = 0;
                        }
                    }
                    for (int i = 0; i < serviceCounter; i++)
                    {
                        if (i == advancedChoice2 - 1)
                        {
                            printf("\nYou picked : %s\n", service[i]);
                            printf("%s costs %0.2f. Do you want to continue?(y/n)\n", service[i], cost[i]);
                            while (tolower(boolean) != 'y' && tolower(boolean) != 'n')
                                scanf("%c", &boolean);
                            if (tolower(boolean) == 'y')
                            {
                                printf("Enter the amount : ");
                                scanf("%i", &quantity);
                                price = cost[i] * quantity;
                                total_price += cost[i] * quantity;
                                printf("The price is : %0.2f\n", price);
                                int lineCounter = 0;
                                char ch;
                                while((ch=fgetc(apick))!=EOF)
                                {
                                    if (ch == '\n')
                                        lineCounter++;
                                }
                                // Print the sale to the automaticpick.txt file
                                fprintf(apick, "\n%s\t%i\t%f\t%02d%02d%02d", service[i], quantity, price, year * 100, mon * 100, day);
                            }

                            else if (tolower(boolean) == 'n')
                            {
                                advancedChoice2 = MAX;
                                printf("Do you want another service? (y/n)\n");
                                while (tolower(boolean1) != 'y' && tolower(boolean1) != 'n')
                                    scanf(" %c", &boolean1);
                                if (tolower(boolean1) == 'y')
                                    advancedChoice2 = 0;
                                else if (tolower(boolean1) == 'n')
                                {
                                    advancedChoice2 = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            // Manual pickup
            else if (advancedChoice1 == 2)
            {
                printf("\n Transact -> Manual Transaction \n");
                int serviceCounter = 0;
                while (fscanf(services, "%f %[^\t] %[^\n]", &cost[serviceCounter],  service[serviceCounter], atype[serviceCounter]) != EOF)
                {
                    printf("|%i %-17s |\t| %05.2f  %s | \n", serviceCounter + 1, service[serviceCounter], cost[serviceCounter], atype[serviceCounter]);
                    serviceCounter++;
                }
                printf("\n");
                while (advancedChoice2 < 1 || advancedChoice2 > serviceCounter)
                {
                    char boolean = 'a';
                    char boolean1 = 'a';
                    if (advancedChoice2 < serviceCounter)
                    {
                        printf("Enter date of pick-up in dd-mm-yy\n");
                        printf("Day: ");
                        scanf("%d", &day);
                        printf("Month: ");
                        scanf("%d", &mon);
                        printf("Year: ");
                        scanf("%d", &year);

                        printf("Enter the type of service: ");
                        scanf("%i", &advancedChoice2);
                        if (advancedChoice2 > serviceCounter || advancedChoice2 < 1)
                        {
                            advancedChoice2 = 0;
                        }
                    }
                    for (int i = 0; i < serviceCounter; i++)
                    {
                        if (i == advancedChoice2 - 1)
                        {
                            printf("\nYou picked : %s\n", service[i]);
                            printf("%s costs %0.2f. Do you want to continue?(y/n)\n", service[i], cost[i]);
                            while (tolower(boolean) != 'y' && tolower(boolean) != 'n')
                                scanf("%c", &boolean);
                            if (tolower(boolean) == 'y')
                            {
                                printf("Enter the amount : ");
                                scanf("%i", &quantity);
                                price = cost[i] * quantity;
                                total_price += cost[i] * quantity;
                                printf("The price is : %0.2f\n", price);
                                int lineCounter = 0;
                                char ch;
                                while((ch=fgetc(spick))!=EOF)
                                {
                                    if (ch == '\n')
                                        lineCounter++;
                                }
                                // Print the sale to the manualpick.txt file
                                fprintf(spick, "|%-17s |\t| %-8i |\t| %-8.2f | %02d%02d%02d-%02d \n", service[i], quantity, price, day, mon, year, lineCounter + 1);
                                break;
                            }

                            else if (tolower(boolean) == 'n')
                            {
                                advancedChoice2 = MAX;
                                printf("Do you want another service? (y/n)\n");
                                while (tolower(boolean1) != 'y' && tolower(boolean1) != 'n')
                                    scanf(" %c", &boolean1);
                                if (tolower(boolean1) == 'y')
                                    advancedChoice = 0;
                                else if (tolower(boolean1) == 'n')
                                {
                                    advancedChoice2 = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else
                break;
        }

        // Process scheduled transaction
        else if (advancedChoice == 2)
        {
           Process_Transaction();
        }
        else
            break;
    }
}

void Auto_Process()
{
    time_t rawtime;
    struct tm * timeinfo;
    rawtime = time (NULL);
    timeinfo = localtime ( &rawtime );
    int tm_day = timeinfo->tm_mday, tm_mon = timeinfo->tm_mon + 1, tm_year = timeinfo->tm_year - 100;

    apick = fopen("autopick.txt", "r");
    aapick = fopen("apicktemp.txt", "w");
    prp = fopen("processed.txt", "a");
    char name[MAX][MAX], date[MAX][MAX];
    float price[MAX];
    int counter = 0, amount[MAX], linecounter = 1;
    char nowdate[MAX];
    sprintf(nowdate, "%02d%02d%02d", tm_year * 100, tm_mon * 100, tm_day);

    while (fscanf(apick ,"%[^\t] %i %f %[^\n]", name[counter], &amount[counter], &price[counter], date[counter]) != EOF)
    {
        counter++;
    }

    for (int i = 0; i < counter; i++)
    {
        if (strcmp(date[i], nowdate) <= 0)
        {
            fprintf(prp, "%-10s |\t| %-8i |\t| %-8.2f | %6s \n", name[i], amount[i], price[i], date[i]);
        }
        else
        {
            fprintf(aapick, "%s\t%i\t%f\t%s", name[i], amount[i], price[i], date[i]);
        }
    }
    fclose(apick);
    fclose(aapick);

    remove("autopick.txt");
    rename("apicktemp.txt", "autopick.txt");

    printf("\n");

}



void Process_Transaction()
{
    spick = fopen("manualpick.txt", "r");
    sspick = fopen("spicktemp.txt", "w");
    int lineCounter = 1, lineDelete;
    char c;
    printf("\n Process Scheduled Transaction\n");
    c = getc(spick);
    while (c != EOF)
    {
        printf("%c", c);
        c = getc(spick);
    }
    rewind(spick);
    printf("\nEnter the number you want to pickup: ");
    scanf("%d", &lineDelete);

    c = getc(spick);
    while (c != EOF)
    {
        c = getc(spick);
        if (c == '\n')
            lineCounter++;
        if (lineCounter != lineDelete)
            putc(c, sspick);
    }
        fclose(spick);
        fclose(sspick);

        remove("manualpick.txt");
        rename("spicktemp.txt", "manualpick.txt");

    printf("\n");
}


void Generate_Report()
{

    // Generate_Report Functions
    void GenerateSalesRecord();
    void GenerateSalesInventory();
    void GenerateSalesPerService();

    int GenReportChoice =-1;
    printf("\nGenerate Report\n");
    printf("\t|1 Generate Sales Record      |\n");
    printf("\t|2 Generate Sales Report      |\n");
    printf("\t|3 Generate Sales per Service |\n");

    while (GenReportChoice>3 || GenReportChoice <0)
    {
        printf("Please input your desired action:");
        scanf(" %i", &GenReportChoice);

        switch(GenReportChoice)
        {
            case 1:
                GenerateSalesRecord();
                break;

            case 2:
                GenerateSalesInventory();
                    break;

            case 3:
                GenerateSalesPerService();
                    break;


        }
    }

}


    void GenerateSalesRecord()
    {
        record = fopen("record.txt", "r");
        if (record == NULL)
            {
                printf("filenotfound");
            }

        printf("\t\tSALES RECORD\n");
        char c;
        c = getc(record);
        while (c != EOF)
        {
            printf("%c", c);
            c = getc(record);
        }
    }

    void GenerateSalesInventory()
    {
        report = fopen("report.txt", "r");

        printf("\t\tSALES REPORT\n");
        char c;
        c = getc(report);
        while (c != EOF)
        {
            printf("%c", c);
            c = getc(report);
        }
    }

    void GenerateSalesPerService()
    {
        services = fopen("Ref_Files/services.txt", "r");
        char service[MAX][MAX], atype[MAX][MAX], filename[MAX], perservice[MAX][MAX], servicename[MAX];
        float cost[MAX], price[MAX];
        int quantity[MAX], salesChoice;
        int serviceCounter = 0;
        while (fscanf(services, "%f %[^\t] %[^\n]", &cost[serviceCounter],  service[serviceCounter], atype[serviceCounter]) != EOF)
        {
            printf("\t|%i %-17s |\n", serviceCounter + 1, service[serviceCounter]);
            serviceCounter++;
        }
        printf("Choose the service you want to generate: ");
        scanf("%d", &salesChoice);
        for (int i = 0; i < serviceCounter; i++)
        {
            if (i == salesChoice - 1)
            {
                sprintf(filename, "Sale_Per_Service/%s.txt", service[i]);
                sprintf(servicename, "%s", service[i]);
            }
        }
        sps = fopen(filename, "w");
        record = fopen("Ref_Files/unfRec.txt", "r");
        if (record == NULL)
        {
            printf("filenotfound");
        }

        int fileCounter = 0;
        while (fscanf(record, "%i %f %[^\n]", &quantity[fileCounter], &price[fileCounter], perservice[fileCounter]) != EOF)
        {
            if (strcmp(perservice[fileCounter], servicename) == 0)
                fprintf(sps, "| %-17s |\t| %-8i |\t| %-8.2f | \n", perservice[fileCounter], quantity[fileCounter], price[fileCounter]);
            fileCounter++;
        }
        printf("\nSale Per Service Generated. You can find it in 'Sale_Per_Service' directory\n");

    }

void File_Management()
{
    // File_Management Functions
    void addService();
    void editService();
    void deleteService();
    void displayallServices();

    int fileManagementChoice = -1;
    printf("\t FILE MANAGEMENT\n");
    printf("\t|1 ADD NEW SERVICE     |\n");
    printf("\t|2 EDIT SERVICE        |\n");
    printf("\t|3 DELETE SERVICE      |\n");
    printf("\t|4 DISPLAY ALL SERVICES|\n");

    while (fileManagementChoice > 4 || fileManagementChoice < 0 )
        {

            printf("Please input your desired action:");
            scanf(" %i", &fileManagementChoice);

            switch(fileManagementChoice)
                {
                    case 1:
                        addService();
                            break;
                    case 2:

                        editService();
                            break;
                    case 3:
                        deleteService();
                            break;
                    case 4:
                        displayallServices();
                            break;
                    default:
                        printf("Out of range");
                            break;
                 }
        }
}

void addService()
{
    services = fopen("Ref_Files/services.txt", "a");
    float newcost;
    char newservice[MAX], newtype[MAX];
    printf("Please enter the new service:");
    scanf(" %[^\n]", newservice);
    printf("Please enter the cost of new service:");
    scanf(" %f", &newcost);
    printf("Please enter the type of service: (eg. page, hour)");
    scanf(" %[^\n]", newtype);
    fprintf(services, "\n%05.2f\t%s\t%s", newcost, newservice, newtype);
}

void editService()
{
    services = fopen("Ref_Files/services.txt","r");
    newServices = fopen("services_temp.txt", "w");

    char service[MAX][MAX], atype[MAX][MAX];
    int i = 0, counter = 0, num;
    float cost[MAX], newPrice;

    while (fscanf(services, "%f %[^\t] %[^\n]", &cost[counter], service[counter], atype[counter]) != EOF)
    {
        printf("|%i %-17s |\t| %05.2f  %s | \n", counter + 1, service[counter], cost[counter], atype[counter]);
        counter++;
    }
    printf("\n");
    printf("Please choose the corresponding number of service you wish to edit: ");
    scanf(" %i", &num);
    printf("Please enter the new price for %s: ", service[num - 1]);
    scanf(" %f", &newPrice);
    printf("\n");


    for(i = 0; i < counter; i++)
    {
        if(i == num - 1)
        {
            printf("|%i %-17s |\t| %05.2f %s| \n", i+1, service[i], newPrice,atype[i]);
            fprintf(newServices,"%05.2f\t%s\t%s\n", newPrice, service[i],atype[i]);
        }
        else
        {
            printf("|%i %-17s |\t| %05.2f %s| \n", i+1, service[i], cost[i],atype[i]);
            fprintf(newServices,"%05.2f\t%s\t%s\n",cost[i],service[i],atype[i]);
        }
    }

    printf("\n\tPROCESS COMPLETE.\n\n");

    fclose(services);
    fclose(newServices);

    remove("services.txt");
    rename("services_temp.txt","services.txt");

}
void deleteService()
{
    char line[MAX][MAX];
    services = fopen("Ref_Files/services.txt","r");
    newServices = fopen("services_temp.txt", "w");

    int i = 0, num;
    int counter = 0;

    printf("Please choose the number of service you wish to delete.\n\n");
    while(fgets(line[i], MAX, services))
	{
        line[i][strlen(line[i])] = '\0';
        i++;
    }
    counter = i;

    for (i = 0; i < counter; ++i)
    {
        printf("(%i)  %s\n", i+1, line[i]);
    }

    printf("\n");

    printf("Please choose the number of service you wish to delete: ");
    scanf("%i", &num);

    for (counter = 0; counter <= i; counter++)
    {
        if (counter == num - 1)
        {
            continue;
        }
        fprintf(newServices,"%s", line[counter]);
    }
    printf("\nPROCESS COMPLETE\n");

    fclose(services);
    fclose(newServices);

    remove("services.txt");
    rename("services_temp.txt","services.txt");
}
void displayallServices()
{

    int mainmenu;
    services = fopen("Ref_Files/services.txt", "r");
    char service[MAX][MAX], atype[MAX][MAX];
    float cost[MAX], price = 0, total_price = 0;
    int quantity;
    int serviceCounter = 0;
    while (fscanf(services, "%f %[^\t] %[^\n]", &cost[serviceCounter], service[serviceCounter], atype[serviceCounter]) != EOF)
        {
        printf("|%i %-17s |\t| %05.2f %s \n",serviceCounter+1, service[serviceCounter], cost[serviceCounter],atype[serviceCounter]);
        serviceCounter++;
        }
    printf("Please input 1 to return to the main menu:");
    scanf(" %i", &mainmenu);

}
