#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS 50
#define MAX_NAME_LEN 50
#define MAX_ITEM_LEN 20
#define MAX_DATE_LEN 30

struct items {
    char item[MAX_ITEM_LEN];
    float price;
    int qty;
};

struct orders {
    char customer[MAX_NAME_LEN];
    char date[MAX_DATE_LEN];
    int numOfItems;
    struct items itm[MAX_ITEMS];
};

// Functions to generate bills
void generateBillHeader(const char name[MAX_NAME_LEN], const char date[MAX_DATE_LEN]) {
    printf("\n\n");
    printf("\t    ADV. Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\tQty\t\tTotal\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}

void generateBillBody(const char item[MAX_ITEM_LEN], int qty, float price) {
    printf("%s\t\t%d\t\t%.2f\t\t", item, qty, qty * price);
    printf("\n");
}

void generateBillFooter(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal;
    float grandTotal = netTotal + 2 * cgst; // Net Total + CGST + SGST
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", dis);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\nSGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

void getCurrentDate(char date[MAX_DATE_LEN]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

int main() {
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill;
    char contFlag = 'y';
    char name[MAX_NAME_LEN];
    FILE *fp;

    // Dashboard
    while (contFlag == 'y') {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        float total = 0;
        int invoiceFound = 0;
        printf("\t============ADV. RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");
        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin); // Clear the newline character left by scanf

        switch (opt) {
            case 1:
                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif

                printf("\nPlease enter the name of the customer:\t");
                fgets(ord.customer, MAX_NAME_LEN, stdin);
                ord.customer[strcspn(ord.customer, "\n")] = 0; // Remove trailing newline
                getCurrentDate(ord.date);
                printf("\nPlease enter the number of items:\t");
                scanf("%d", &n);
                ord.numOfItems = n;
                for (int i = 0; i < n; i++) {
                    fgetc(stdin); // Clear the newline character left by scanf
                    printf("\n\n");
                    printf("Please enter the item %d:\t", i + 1);
                    fgets(ord.itm[i].item, MAX_ITEM_LEN, stdin);
                    ord.itm[i].item[strcspn(ord.itm[i].item, "\n")] = 0; // Remove trailing newline
                    printf("Please enter the quantity:\t");
                    scanf("%d", &ord.itm[i].qty);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\nDo you want to save the invoice [y/n]:\t");
                fgetc(stdin); // Clear newline
                scanf("%c", &saveBill);

                if (saveBill == 'y') {
                    fp = fopen("RestaurantBill.dat", "a+");
                    if (fp == NULL) {
                        printf("\nError opening file for writing.\n");
                        break;
                    }
                    if (fwrite(&ord, sizeof(struct orders), 1, fp) != 1) {
                        printf("\nError saving the invoice.\n");
                    } else {
                        printf("\nSuccessfully saved.\n");
                    }
                    fclose(fp);
                }
                break;

            case 2:
                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif

                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file for reading.\n");
                    break;
                }
                printf("\n  ****Your Previous Invoices\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                fgets(name, MAX_NAME_LEN, stdin);
                name[strcspn(name, "\n")] = 0; // Remove trailing newline

                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif

                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file for reading.\n");
                    break;
                }
                printf("\t**Invoice of %s", name);
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    if (strcmp(order.customer, name) == 0) {
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist.\n", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);

            default:
                printf("Sorry, invalid option.\n");
                break;
        }
        printf("\nDo you want to perform another operation? [y/n]:\t");
        fgetc(stdin); // Clear newline
        scanf("%c", &contFlag);
    }

    printf("\n\t\t Bye Bye :)\n\n");
    return 0;
}
