#include <stdio.h>
#include <string.h>
#include <conio.h>

#define MAX 10

char items[MAX][30];
int qty[MAX];
float price[MAX];
int count = 0;

// Function Declarations
void addItem(void);
void viewItems(void);
void searchItem(void);
void updateItem(void);
void deleteItem(void);
void saveToFile(void);
void loadFromFile(void);

// LOGIN
int login(void) {
    char user[20];
    char pass[20];
    int attempts = 0;
    int i;
    char ch;

    while(attempts < 3) {

        printf("LOGIN\n");
        printf("Username: ");
        scanf("%s", user);

        printf("Password: ");
        i = 0;

        // HIDE PASSWORD 
        while(1) {
            ch = getch();

            if(ch == 13) {
                pass[i] = '\0';
                break;
            }
            else if(ch == 8 && i > 0) {
                i--;
                printf("\b \b");
            }
            else {
                pass[i++] = ch;
                printf("*");
            }
        }

        // CHECK LOGIN
        if(strcmp(user, "admin") == 0 && strcmp(pass, "admin12345") == 0) {
            printf("\n\033[1;32mLogin Successful!\033[0m\n");
            return 1;
        } else {
            attempts++;
            printf("\n\033[1;31mInvalid username or password!\033[0m\n");

            if(attempts < 3) {
                printf("Try again (%d/3 used)\n\n", attempts);
            }
        }
    }

    printf("\033[1;33mToo many failed attempts. Access denied!\033[0m\n");
    return 0;
}

int main(void) {
    int choice;

    if(!login()) {
        return 0;
    }

    loadFromFile();

    do {
        printf("\nMENU\n");
        printf("1 Add\n");
        printf("2 View\n");
        printf("3 Search\n");
        printf("4 Update\n");
        printf("5 Delete\n");
        printf("6 Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if(choice == 1) { addItem(); saveToFile(); }
        else if(choice == 2) viewItems();
        else if(choice == 3) searchItem();
        else if(choice == 4) { updateItem(); saveToFile(); }
        else if(choice == 5) { deleteItem(); saveToFile(); }

    } while(choice != 6);

    printf("Data saved. Exiting...\n");
    return 0;
}

// ADD ITEM
void addItem(void) {
    char name[30];
    int q, i;
    float p;
    char choice;

    printf("\nItem: ");
    scanf("%s", name);

    for(i = 0; i < count; i++) {
        if(strcmp(items[i], name) == 0) {
            printf("Item already exists!\n");
            printf("Add quantity? (Y/N): ");
            scanf(" %c", &choice);

            if(choice == 'Y' || choice == 'y') {
                printf("Quantity to add: ");
                scanf("%d", &q);

                qty[i] += q;
                printf("\033[1;32mQuantity updated!\033[0m\n");
            } else {
                printf("\033[1;3mNo changes made.\033[0m\n");
            }
            return;
        }
    }

    if(count >= MAX) {
        printf("Full!\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &q);

    printf("Price: ");
    scanf("%f", &p);

    strcpy(items[count], name);
    qty[count] = q;
    price[count] = p;
    count++;

    printf("\033[1;32mNew item added!\033[0m\n");
}

// VIEW ITEMS
void viewItems(void) {
    int i;

    if(count == 0) {
        printf("\n\033[1;33mNo items!\033[0m\n");
        return;
    }

    printf("\nLIST\n");

    for(i = 0; i < count; i++) {
        printf("%d %s %d %.2f\n", i+1, items[i], qty[i], price[i]);

        if(qty[i] < 5) {
            printf("\033[1;31mLOW STOCK!\033[0m\n\n");
        }
        else if(qty[i] <= 10) {
            printf("\033[1;33mWARNING STOCK!\033[0m\n\n");
        }
        else {
            printf("\033[1;32mNORMAL STOCK!\033[0m\n\n");
        }
    }
}

// SEARCH ITEM
void searchItem(void) {
    char s[30];
    int i, found = 0;

    printf("\nSearch: ");
    scanf("%s", s);

    for(i = 0; i < count; i++) {
        if(strcmp(items[i], s) == 0) {

            printf("Found: %s | Qty: %d | Price: %.2f\n",
                   items[i], qty[i], price[i]);

            if(qty[i] < 5) {
                printf("\033[1;31mLOW STOCK!\033[0m\n\n");
            }
            else if(qty[i] <= 10) {
                printf("\033[1;33mWARNING STOCK!\033[0m\n\n");
            }
            else {
                printf("\033[1;32mNORMAL STOCK!\033[0m\n\n");
            }

            found = 1;
        }
    }

    if(found == 0) {
        printf("Not found\n");
    }
}

// UPDATE ITEM
void updateItem(void) {
    int n;

    printf("\nNumber: ");
    scanf("%d", &n);

    if(n < 1 || n > count) {
        printf("Invalid\n");
        return;
    }

    printf("New Item: ");
    scanf("%s", items[n-1]);

    printf("New Quantity: ");
    scanf("%d", &qty[n-1]);

    printf("New Price: ");
    scanf("%f", &price[n-1]);
}

// DELETE ITEM
void deleteItem(void) {
    int n, i;

    printf("\nNumber: ");
    scanf("%d", &n);

    if(n < 1 || n > count) {
        printf("\033[1;31mInvalid!\033[0m\n");
        return;
    }

    for(i = n-1; i < count - 1; i++) {
        strcpy(items[i], items[i+1]);
        qty[i] = qty[i+1];
        price[i] = price[i+1];
    }

    count--;
}

// SAVE FILE
void saveToFile(void) {
    FILE *fp;
    int i;

    fp = fopen("items.txt", "w");

    if(fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    fprintf(fp, "%d\n", count);

    for(i = 0; i < count; i++) {
        fprintf(fp, "%s %d %.2f\n", items[i], qty[i], price[i]);
    }

    fclose(fp);
}

// LOAD FILE
void loadFromFile(void) {
    FILE *fp;
    int i;

    fp = fopen("items.txt", "r");

    if(fp == NULL) {
        return;
    }

    fscanf(fp, "%d", &count);

    for(i = 0; i < count; i++) {
        fscanf(fp, "%s %d %f", items[i], &qty[i], &price[i]);
    }

    fclose(fp);
}
