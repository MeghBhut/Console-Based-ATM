#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
char *getTimestamp()
{
    static char buffer[20];
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}
struct Account
{
    int Acno;
    int PIN;
    int Balance;
    char name[50];
};
struct Transition
{
    char timestamp[20];
    int acno_from;
    int acno_To;
    int amount;
    char Type[50];
};
int main()
{
    printf("WELCOME TO THE CONSOLE ATM!\n");
    int choice;
    while (1)
    {
        printf("\n=================================\n");
        printf("1.Create New Account\n");
        printf("2.View Account Balance and Statement\n");
        printf("3.Deposit Amount\n");
        printf("4.Withdraw Amount\n");
        printf("5.Transfer Amount\n");
        printf("6.Exit ATM\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        printf("\n=================================\n");
        if (choice == 1)
        {
            Create_Account();
        }
        if (choice == 2)
        {
            View_Account();
        }
        if (choice == 3)
        {
            Deposit_Amount();
        }
        if (choice == 4)
        {
            Withdraw_Amount();
        }
        if (choice == 5)
        {
            Transfer_Amount();
        }
        if (choice == 6)
        {
            printf("Thanks for using our console ATM");
            return 0;
        }
    }
    return 0;
}

void Create_Account()
{

    struct Account a[100];
    int No_of_acc = 0;
    FILE *fc = fopen("accounts.dat", "ab");
    fclose(fc);
    FILE *fr = fopen("accounts.dat", "rb");
    if (fr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    No_of_acc = fread(a, sizeof(struct Account), 100, fr);
    fclose(fr);
    FILE *fw = fopen("accounts.dat", "ab");
    if (fw == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    printf("Enter Account No: ");
    scanf("%d", &a[No_of_acc].Acno);
    printf("Enter PIN: ");
    scanf("%d", &a[No_of_acc].PIN);
    printf("Enter Initial Balance: ");
    scanf("%d", &a[No_of_acc].Balance);
    getchar();
    printf("Enter Name: ");
    fgets(a[No_of_acc].name, sizeof(a[No_of_acc].name), stdin);
    a[No_of_acc].name[strcspn(a[No_of_acc].name, "\n")] = '\0';
    fwrite(&a[No_of_acc], sizeof(struct Account), 1, fw);
    fclose(fw);
    printf("account Created sucessfully");
    char filename[50];
    sprintf(filename, "%d.dat", a[No_of_acc].Acno);
    struct Transition t[100];
    FILE *flc = fopen(filename, "ab");
    fclose(flc);
    FILE *flr = fopen(filename, "rb");
    if (flr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    int No_of_Transition = fread(t, sizeof(struct Transition), 100, flr);
    fclose(flr);
    FILE *flw = fopen(filename, "ab");
    strcpy(t[No_of_Transition].timestamp, getTimestamp());
    t[No_of_Transition].acno_from = a[No_of_acc].Acno;
    t[No_of_Transition].acno_To = 0;
    strcpy(t[No_of_Transition].Type, "Account Created");
    t[No_of_Transition].amount = a[No_of_acc].Balance;
    fwrite(&t[No_of_Transition], sizeof(struct Transition), 1, flw);
    fclose(flw);
}

void View_Account()
{
    struct Account a[100];
    int No_of_acc;
    FILE *fr = fopen("accounts.dat", "rb");
    if (fr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    No_of_acc = fread(a, sizeof(struct Account), 100, fr);
    fclose(fr);
    int check_acno;
    int check_PIN;
    printf("Total no of account found: %d\n", No_of_acc);
    printf("Enter accno of account you want to check: ");
    scanf("%d", &check_acno);
    for (int i = 0; i < No_of_acc; i++)
    {
        if (a[i].Acno == check_acno)
        {
            printf("Enter PIN: ");
            scanf("%d", &check_PIN);
            if (a[i].PIN == check_PIN)
            {
                printf("Name of account holder is: %s\n", a[i].name);
                printf("Current Balance in account is: %d\n", a[i].Balance);
                char filename[50];
                sprintf(filename, "%d.dat", a[i].Acno);
                struct Transition t[100];
                FILE *flr = fopen(filename, "rb");
                if (flr == NULL)
                {
                    printf("Issue with file handling!");
                    exit(1);
                }
                int No_of_Transition = fread(t, sizeof(struct Transition), 100, flr);
                fclose(flr);
                printf("Your Statement: \n");
                printf("\n--------------------------------------------------------------------------------\n");
                printf("%-21s | %-4s | %-4s | %-5s | %-20s", "Timestamp", "From", "To", "Amount", "Type");
                printf("\n--------------------------------------------------------------------------------\n");
                for (int j = 0; j < No_of_Transition; j++)
                {
                    printf("%s | %d | %d | %d | %s \n", t[j].timestamp, t[j].acno_from, t[j].acno_To, t[j].amount, t[j].Type);
                }
            }
        }
    }
}

void Deposit_Amount()
{
    struct Account a[100];
    int No_of_acc;
    FILE *fr = fopen("accounts.dat", "rb");
    if (fr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    No_of_acc = fread(a, sizeof(struct Account), 100, fr);
    fclose(fr);
    FILE *temp = fopen("temp.dat", "ab");
    int check_acno;
    int check_PIN;
    printf("Total no of account found: %d\n", No_of_acc);
    printf("Enter accno of account you want to deposit: ");
    scanf("%d", &check_acno);
    int deposit = 0;
    for (int i = 0; i < No_of_acc; i++)
    {
        if (a[i].Acno == check_acno)
        {
            printf("Enter PIN: ");
            scanf("%d", &check_PIN);
            if (a[i].PIN == check_PIN)
            {
                printf("Name of account holder is: %s\n", a[i].name);
                printf("Current Balance in account is: %d\n", a[i].Balance);
                printf("Enter amount you want to deposit: ");
                scanf("%d", &deposit);
                a[i].Balance += deposit;
                char filename[50];
                sprintf(filename, "%d.dat", a[i].Acno);
                struct Transition t[100];
                FILE *flc = fopen(filename, "ab");
                fclose(flc);
                FILE *flr = fopen(filename, "rb");
                if (flr == NULL)
                {
                    printf("Issue with file handling!");
                    exit(1);
                }
                int No_of_Transition = fread(t, sizeof(struct Transition), 100, flr);
                fclose(flr);
                FILE *flw = fopen(filename, "ab");
                strcpy(t[No_of_Transition].timestamp, getTimestamp());
                t[No_of_Transition].acno_from = a[i].Acno;
                t[No_of_Transition].acno_To = a[i].Acno;
                strcpy(t[No_of_Transition].Type, "Deposit");
                t[No_of_Transition].amount = deposit;
                fwrite(&t[No_of_Transition], sizeof(struct Transition), 1, flw);
                fclose(flw);
            }
        }
        fwrite(&a[i], sizeof(struct Account), 1, temp);
    }
    fclose(temp);
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    printf("Deposit sucessfully");
}

void Withdraw_Amount()
{
    struct Account a[100];
    int No_of_acc;
    FILE *fr = fopen("accounts.dat", "rb");
    if (fr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    No_of_acc = fread(a, sizeof(struct Account), 100, fr);
    fclose(fr);
    FILE *temp = fopen("temp.dat", "ab");
    int check_acno;
    int check_PIN;
    printf("Total no of account found: %d\n", No_of_acc);
    printf("Enter accno of account you want to Withdraw: ");
    scanf("%d", &check_acno);
    int Withdraw = 0;
    for (int i = 0; i < No_of_acc; i++)
    {
        if (a[i].Acno == check_acno)
        {
            printf("Enter PIN: ");
            scanf("%d", &check_PIN);
            if (a[i].PIN == check_PIN)
            {
                printf("Name of account holder is: %s\n", a[i].name);
                printf("Current Balance in account is: %d\n", a[i].Balance);
                printf("Enter amount you want to Withdraw: ");
                scanf("%d", &Withdraw);
                a[i].Balance -= Withdraw;
                char filename[50];
                sprintf(filename, "%d.dat", a[i].Acno);
                struct Transition t[100];
                FILE *flc = fopen(filename, "ab");
                fclose(flc);
                FILE *flr = fopen(filename, "rb");
                if (flr == NULL)
                {
                    printf("Issue with file handling!");
                    exit(1);
                }
                int No_of_Transition = fread(t, sizeof(struct Transition), 100, flr);
                fclose(flr);
                FILE *flw = fopen(filename, "ab");
                strcpy(t[No_of_Transition].timestamp, getTimestamp());
                t[No_of_Transition].acno_from = a[i].Acno;
                t[No_of_Transition].acno_To = a[i].Acno;
                strcpy(t[No_of_Transition].Type, "Withdraw");
                t[No_of_Transition].amount = Withdraw;
                fwrite(&t[No_of_Transition], sizeof(struct Transition), 1, flw);
                fclose(flw);
            }
        }
        fwrite(&a[i], sizeof(struct Account), 1, temp);
    }
    fclose(temp);
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    printf("Withdraw sucessfully");
}

void Transfer_Amount()
{
    struct Account a[100];
    int No_of_acc;
    FILE *fr = fopen("accounts.dat", "rb");
    if (fr == NULL)
    {
        printf("Issue with file handling!");
        exit(1);
    }
    No_of_acc = fread(a, sizeof(struct Account), 100, fr);
    fclose(fr);
    FILE *temp = fopen("temp.dat", "ab");
    int from_acno;
    int to_acno;
    int check_PIN;
    printf("Total no of account found: %d\n", No_of_acc);
    printf("Enter accno of account from you want to transfer: ");
    scanf("%d", &from_acno);
    printf("Enter accno of account to you want to transfer: ");
    scanf("%d", &to_acno);
    int Transfer = 0;
    for (int i = 0; i < No_of_acc; i++)
    {
        if (a[i].Acno == from_acno)
        {
            for (int j = 0; j < No_of_acc; j++)
            {
                if (a[j].Acno == to_acno)
                {
                    printf("Enter PIN of from account: ");
                    scanf("%d", &check_PIN);
                    if (a[i].PIN == check_PIN)
                    {
                        printf("Name of account holder is: %s\n", a[i].name);
                        printf("Current Balance in account is: %d\n", a[i].Balance);
                        printf("Enter amount you want to Transfer: ");
                        scanf("%d", &Transfer);
                        a[i].Balance += Transfer;
                        a[j].Balance -= Transfer;
                        char filename[50];
                        sprintf(filename, "%d.dat", a[i].Acno);
                        struct Transition t[100];
                        FILE *flc = fopen(filename, "ab");
                        fclose(flc);
                        FILE *flr = fopen(filename, "rb");
                        if (flr == NULL)
                        {
                            printf("Issue with file handling!");
                            exit(1);
                        }
                        int No_of_Transition = fread(t, sizeof(struct Transition), 100, flr);
                        fclose(flr);
                        FILE *flw = fopen(filename, "ab");
                        strcpy(t[No_of_Transition].timestamp, getTimestamp());
                        t[No_of_Transition].acno_from = a[i].Acno;
                        t[No_of_Transition].acno_To = a[j].Acno;
                        strcpy(t[No_of_Transition].Type, "Transfer");
                        t[No_of_Transition].amount = Transfer;
                        fwrite(&t[No_of_Transition], sizeof(struct Transition), 1, flw);
                        fclose(flw);
                        sprintf(filename, "%d.dat", a[j].Acno);
                        FILE *flc2 = fopen(filename, "ab");
                        fclose(flc2);
                        FILE *flr2 = fopen(filename, "rb");
                        if (flr2 == NULL)
                        {
                            printf("Issue with file handling!");
                            exit(1);
                        }
                        No_of_Transition = fread(t, sizeof(struct Transition), 100, flr2);
                        fclose(flr2);
                        FILE *flw2 = fopen(filename, "ab");
                        strcpy(t[No_of_Transition].timestamp, getTimestamp());
                        t[No_of_Transition].acno_from = a[i].Acno;
                        t[No_of_Transition].acno_To = a[j].Acno;
                        strcpy(t[No_of_Transition].Type, "Transfer");
                        t[No_of_Transition].amount = Transfer;
                        fwrite(&t[No_of_Transition], sizeof(struct Transition), 1, flw2);
                        fclose(flw2);
                    }
                }
            }
        }
        fwrite(&a[i], sizeof(struct Account), 1, temp);
    }
    fclose(temp);
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    printf("Transfer sucessfully");
}
