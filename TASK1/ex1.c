#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>


const int ITEMS_COUNT = 10;
const char* CURRENCIES[] = { "BGN", "USD", "EUR"};

struct BankAccount {
    uint16_t number;
    char ownerName[100];  
    double balance;
    char currency[2];  
    struct BankAccount* next;
};

typedef struct BankAccount* node_t;

double totalBalance(node_t* head) {
    double total = 0.0;
    while (*head != NULL) {
        total += (*head)->balance;
        *head = (*head)->next;
    }
    return total;
}

node_t maxBalance(node_t* head) {
    node_t maxNode = NULL;
    double maxBalance = -1.0;

    while (*head != NULL) {
        if ((*head)->balance > maxBalance) {
            maxBalance = (*head)->balance;
            maxNode = *head;
        }
        *head = (*head)->next;
    }

    return maxNode;
}

void generateRandomData(uint16_t* num, char* name, double* balance, char* currency) {
    *num = rand() % 65536;

    strcpy(name, "");
    int wordCount = 2 + rand() % 4;
    for (int i = 0; i < wordCount; ++i) {
        char word[21] = "";
        int wordLength = 1 + rand() % 20;
        for (int j = 0; j < wordLength; ++j) {
            char letter = 'A' + rand() % 26;
            word[j] = letter;
        }
        if (strlen(name) > 0) {
            strcat(name, " ");
        }
        strcat(name, word);
    }

    *balance = (rand() % 10000000) / 100.0;
    strcpy(currency, CURRENCIES[rand() % (sizeof(CURRENCIES) / sizeof(CURRENCIES[0]))]);
}

void printBankAccounts(node_t head) {
    while (head != NULL) {
        printf("Number: %u\n", head->number);
        printf("Owner: %s\n", head->ownerName);
        printf("Balance: %.2lf %s\n\n", head->balance, head->currency);
        head = head->next;
    }
}

int main() {
    srand((unsigned int)time(NULL));

    node_t head = NULL;
    for (int i = 0; i < ITEMS_COUNT; ++i) {
        uint16_t num;
        char name[100];
        double balance;
        char currency[4];
        generateRandomData(&num, name, &balance, currency);

        node_t newNode = (node_t)malloc(sizeof(struct BankAccount));
        newNode->number = num;
        strcpy(newNode->ownerName, name);
        newNode->balance = balance;
        strcpy(newNode->currency, currency);
        newNode->next = head;
        head = newNode;
    }

    printf("Bank Account Details:\n");
    printBankAccounts(head);

    double total = totalBalance(&head);
    if (total >= 0) {
        printf("Total Balance of All Accounts: %.2lf\n", total);
    } else {
        printf("Error calculating total balance.\n");
    }

    node_t maxBalAccount = maxBalance(&head);
    if (maxBalAccount != NULL) {
        printf("Account with Maximum Balance:\n");
        printf("Number: %u\n", maxBalAccount->number);
        printf("Owner: %s\n", maxBalAccount->ownerName);
        printf("Balance: %.2lf %s\n", maxBalAccount->balance, maxBalAccount->currency);
    

    while (head != NULL) {
        node_t temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
//end of main