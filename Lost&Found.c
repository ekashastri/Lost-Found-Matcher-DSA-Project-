#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TABLE_SIZE 101
#define LOST_FILE "lost_items.txt"
#define FOUND_FILE "found_items.txt"
typedef struct Item
{
    int id;
    char name[50];
    char category[30];
    char color[20];
    char location[50];
    char date[15];
    int status;
    struct Item *next;
} Item;
typedef struct HashNode
{
    int id;
    Item *item;
    struct HashNode *next;
} HashNode;
Item *lostHead = NULL;
Item *foundHead = NULL;
HashNode *hashTable[TABLE_SIZE];
int nextLostID = 1001;
int nextFoundID = 2001;
void removeNewline(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}
void toLowerCase(char str[])
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
        str[i] = (char)tolower((unsigned char)str[i]);
}
int stringEqualIgnoreCase(char a[], char b[])
{
    int i = 0;
    int j = 0;

    /* Ignore spaces at the beginning */
    while (a[i] == ' ')
        i++;

    while (b[j] == ' ')
        j++;

    while (a[i] != '\0' && b[j] != '\0')
    {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[j]))
            return 0;
        i++;
        j++;
    }
    while (a[i] == ' ')
        i++;
    while (b[j] == ' ')
        j++;
    return a[i] == '\0' && b[j] == '\0';
}
int readInteger()
{
    char input[50];
    int number;
    while (1)
    {
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &number) == 1)
            return number;
        printf("Invalid input. Enter a number: ");
    }
}
void insertHash(Item *item)
{
    int index;
    HashNode *newNode;
    index = item->id % TABLE_SIZE;
    newNode = (HashNode *)malloc(sizeof(HashNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    newNode->id = item->id;
    newNode->item = item;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}
Item *hashSearch(int id)
{
    int index;
    HashNode *current;
    index = id % TABLE_SIZE;
    current = hashTable[index];
    while (current != NULL)
    {
        if (current->id == id)
            return current->item;
        current = current->next;
    }
    return NULL;
}
void insertLostItem(Item *newItem)
{
    Item *current;
    if (lostHead == NULL)
    {
        lostHead = newItem;
        return;
    }
    current = lostHead;
    while (current->next != NULL)
        current = current->next;
    current->next = newItem;
}
void insertFoundItem(Item *newItem)
{
    Item *current;
    if (foundHead == NULL)
    {
        foundHead = newItem;
        return;
    }
    current = foundHead;
    while (current->next != NULL)
        current = current->next;
    current->next = newItem;
}
Item *createItem()
{
    Item *newItem;
    newItem = (Item *)malloc(sizeof(Item));
    if (newItem == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    newItem->status = 0;
    newItem->next = NULL;
    printf("Item name: ");
    fgets(newItem->name, sizeof(newItem->name), stdin);
    removeNewline(newItem->name);
    printf("Category: ");
    fgets(newItem->category, sizeof(newItem->category), stdin);
    removeNewline(newItem->category);
    printf("Color: ");
    fgets(newItem->color, sizeof(newItem->color), stdin);
    removeNewline(newItem->color);
    printf("Location: ");
    fgets(newItem->location, sizeof(newItem->location), stdin);
    removeNewline(newItem->location);
    printf("Date (DD/MM/YYYY): ");
    fgets(newItem->date, sizeof(newItem->date), stdin);
    removeNewline(newItem->date);
    return newItem;
}
void reportLostItem()
{
    Item *newItem;
    printf("\n============================================\n");
    printf("              REPORT LOST ITEM\n");
    printf("============================================\n");
    newItem = createItem();
    if (newItem == NULL)
        return;
    newItem->id = nextLostID++;
    insertLostItem(newItem);
    insertHash(newItem);
    printf("\nLost item registered successfully.\n");
    printf("Lost Item ID: %d\n", newItem->id);
}
void reportFoundItem()
{
    Item *newItem;
    printf("\n============================================\n");
    printf("              REPORT FOUND ITEM\n");
    printf("============================================\n");
    newItem = createItem();
    if (newItem == NULL)
        return;
    newItem->id = nextFoundID++;
    insertFoundItem(newItem);
    insertHash(newItem);
    printf("\nFound item registered successfully.\n");
    printf("Found Item ID: %d\n", newItem->id);
}

void displayItem(Item *item)
{
    printf("\n--------------------------------------------\n");
    printf("ID        : %d\n", item->id);
    printf("Item      : %s\n", item->name);
    printf("Category  : %s\n", item->category);
    printf("Color     : %s\n", item->color);
    printf("Location  : %s\n", item->location);
    printf("Date      : %s\n", item->date);

    if (item->status == 0)
        printf("Status    : ACTIVE\n");
    else
        printf("Status    : RESOLVED\n");

    printf("--------------------------------------------\n");
}

void displayLostItems()
{
    Item *current;

    if (lostHead == NULL)
    {
        printf("\nNo lost items available.\n");
        return;
    }

    printf("\n============================================\n");
    printf("                LOST ITEMS\n");
    printf("============================================\n");

    current = lostHead;

    while (current != NULL)
    {
        displayItem(current);
        current = current->next;
    }
}

void displayFoundItems()
{
    Item *current;

    if (foundHead == NULL)
    {
        printf("\nNo found items available.\n");
        return;
    }

    printf("\n============================================\n");
    printf("                FOUND ITEMS\n");
    printf("============================================\n");

    current = foundHead;

    while (current != NULL)
    {
        displayItem(current);
        current = current->next;
    }
}

void searchByName()
{
    char searchText[50];
    Item *current;
    int found = 0;

    printf("\nEnter item name: ");
    fgets(searchText, sizeof(searchText), stdin);
    removeNewline(searchText);

    current = lostHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->name, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    current = foundHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->name, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    if (!found)
        printf("\nNo item found with that name.\n");
}

void searchByCategory()
{
    char searchText[30];
    Item *current;
    int found = 0;

    printf("\nEnter category: ");
    fgets(searchText, sizeof(searchText), stdin);
    removeNewline(searchText);

    current = lostHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->category, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    current = foundHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->category, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    if (!found)
        printf("\nNo items found in that category.\n");
}

void searchByLocation()
{
    char searchText[50];
    Item *current;
    int found = 0;

    printf("\nEnter location: ");
    fgets(searchText, sizeof(searchText), stdin);
    removeNewline(searchText);

    current = lostHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->location, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    current = foundHead;

    while (current != NULL)
    {
        if (stringEqualIgnoreCase(current->location, searchText))
        {
            displayItem(current);
            found = 1;
        }

        current = current->next;
    }

    if (!found)
        printf("\nNo items found at that location.\n");
}

void searchItem()
{
    int choice;
    int id;
    Item *item;

    printf("\n============================================\n");
    printf("                 SEARCH ITEM\n");
    printf("============================================\n");

    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Category\n");
    printf("4. Search by Location\n");
    printf("\nEnter choice: ");

    choice = readInteger();

    switch (choice)
    {
        case 1:
            printf("\nEnter Item ID: ");
            id = readInteger();
            item = hashSearch(id);
            if (item != NULL)
                displayItem(item);
            else
                printf("\nItem not found.\n");
            break;
        case 2:
            searchByName();
            break;
        case 3:
            searchByCategory();
            break;
        case 4:
            searchByLocation();
            break;
        default:
            printf("\nInvalid choice.\n");
    }
}
int calculateMatchScore(Item *lost, Item *found)
{
    int score = 0;
    if (stringEqualIgnoreCase(lost->name, found->name))
        score += 40;
    if (stringEqualIgnoreCase(lost->category, found->category))
        score += 20;
    if (stringEqualIgnoreCase(lost->color, found->color))
        score += 15;
    if (stringEqualIgnoreCase(lost->location, found->location))
        score += 25;
    return score;
}

void findMatches()
{
    int lostID;
    int score;
    int foundMatch = 0;
    Item *lostItem;
    Item *current;
    printf("\n============================================\n");
    printf("             FIND POSSIBLE MATCH\n");
    printf("============================================\n");
    if (lostHead == NULL)
    {
        printf("\nThere are no lost items.\n");
        return;
    }
    if (foundHead == NULL)
    {
        printf("\nThere are no found items.\n");
        return;
    }
    printf("Enter Lost Item ID: ");
    lostID = readInteger();
    lostItem = hashSearch(lostID);
    if (lostItem == NULL || lostID >= 2000)
    {
        printf("\nLost item not found.\n");
        return;
    }
    if (lostItem->status == 1)
    {
        printf("\nThis item has already been resolved.\n");
        return;
    }
    current = foundHead;
    printf("\nSearching found records...\n");
    while (current != NULL)
    {
        if (current->status == 0)
        {
            score = calculateMatchScore(lostItem, current);
            if (score >= 30)
            {
                printf("\n============================================\n");
                printf("              POSSIBLE MATCH\n");
                printf("===========================================\n");
                printf("Lost ID       : %d\n", lostItem->id);
                printf("Found ID      : %d\n", current->id);
                printf("\nLost Item     : %s\n", lostItem->name);
                printf("Found Item    : %s\n", current->name);
                printf("Category      : %s / %s\n",
                       lostItem->category, current->category);
                printf("Color         : %s / %s\n",
                       lostItem->color, current->color);
                printf("Location      : %s / %s\n",
                       lostItem->location, current->location);
                printf("\nMatch Score   : %d%%\n", score);
                if (score >= 80)
                    printf("Match Status : STRONG MATCH\n");
                else if (score >= 50)
                    printf("Match Status : POSSIBLE MATCH\n");
                else
                    printf("Match Status : WEAK MATCH\n");
                foundMatch = 1;
            }
        }
        current = current->next;
    }
    if (!foundMatch)
        printf("\nNo possible matches found.\n");
}
void resolveItem()
{
    int id;
    Item *item;
    printf("\n============================================\n");
    printf("              RESOLVE ITEM\n");
    printf("============================================\n");
    printf("Enter Item ID: ");
    id = readInteger();
    item = hashSearch(id);
    if (item == NULL)
    {
        printf("\nItem not found.\n");
        return;
    }
    if (item->status == 1)
    {
        printf("\nItem is already resolved.\n");
        return;
    }
    item->status = 1;
    printf("\nItem %d marked as RESOLVED.\n", id);
}
void showStatistics()
{
    Item *current;
    int lostTotal = 0;
    int foundTotal = 0;
    int lostActive = 0;
    int foundActive = 0;
    int lostResolved = 0;
    int foundResolved = 0;
    current = lostHead;
    while (current != NULL)
    {
        lostTotal++;
        if (current->status == 0)
            lostActive++;
        else
            lostResolved++;
        current = current->next;
    }
    current = foundHead;
    while (current != NULL)
    {
        foundTotal++;
        if (current->status == 0)
            foundActive++;
        else
            foundResolved++;
        current = current->next;
    }
    printf("\n============================================\n");
    printf("                 STATISTICS\n");
    printf("============================================\n");
    printf("\nLOST ITEMS\n");
    printf("Total      : %d\n", lostTotal);
    printf("Active     : %d\n", lostActive);
    printf("Resolved   : %d\n", lostResolved);
    printf("\nFOUND ITEMS\n");
    printf("Total      : %d\n", foundTotal);
    printf("Active     : %d\n", foundActive);
    printf("Resolved   : %d\n", foundResolved);
    printf("\nOVERALL\n");
    printf("Total Records : %d\n", lostTotal + foundTotal);
    printf("============================================\n");
}
void saveToFile(Item *head, char filename[])
{
    FILE *file;
    Item *current;
    file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("\nError opening %s for writing.\n", filename);
        return;
    }
    current = head;
    while (current != NULL)
    {
        fprintf(file, "%d|%s|%s|%s|%s|%s|%d\n",
                current->id,
                current->name,
                current->category,
                current->color,
                current->location,
                current->date,
                current->status);
        current = current->next;
    }
    fclose(file);
}
void loadFromFile(char filename[], int type)
{
    FILE *file;
    int id;
    int status;
    char name[50];
    char category[30];
    char color[20];
    char location[50];
    char date[15];
    Item *newItem;
    file = fopen(filename, "r");
    if (file == NULL)
        return;
    while (fscanf(file,
                  "%d|%49[^|]|%29[^|]|%19[^|]|%49[^|]|%14[^|]|%d\n",
                  &id,
                  name,
                  category,
                  color,
                  location,
                  date,
                  &status) == 7)
    {
        newItem = (Item *)malloc(sizeof(Item));
        if (newItem == NULL)
        {
            printf("Memory allocation failed while loading data.\n");
            fclose(file);
            return;
        }
        newItem->id = id;
        strcpy(newItem->name, name);
        strcpy(newItem->category, category);
        strcpy(newItem->color, color);
        strcpy(newItem->location, location);
        strcpy(newItem->date, date);
        newItem->status = status;
        newItem->next = NULL;
        if (type == 0)
        {
            insertLostItem(newItem);
            if (id >= nextLostID)
                nextLostID = id + 1;
        }
        else
        {
            insertFoundItem(newItem);
            if (id >= nextFoundID)
                nextFoundID = id + 1;
        }
        insertHash(newItem);
    }
    fclose(file);
}
void freeList(Item *head)
{
    Item *current;
    Item *nextNode;
    current = head;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

void freeHashTable()
{
    int i;
    HashNode *current;
    HashNode *nextNode;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        current = hashTable[i];
        while (current != NULL)
        {
            nextNode = current->next;
            free(current);
            current = nextNode;
        }
        hashTable[i] = NULL;
    }
}
int main()
{
    int choice;
    for (int i = 0; i < TABLE_SIZE; i++)
        hashTable[i] = NULL;
    loadFromFile(LOST_FILE, 0);
    loadFromFile(FOUND_FILE, 1);
    printf("\n==================================================\n");
    printf("          CAMPUS LOST & FOUND MATCHER\n");
    printf("==================================================\n");
    printf("\nPreviously saved data loaded successfully.\n");
    do
    {
        printf("\n\n==================================================\n");
        printf("             CAMPUS LOST & FOUND\n");
        printf("==================================================\n");
        printf("1. Report Lost Item\n");
        printf("2. Report Found Item\n");
        printf("3. View Lost Items\n");
        printf("4. View Found Items\n");
        printf("5. Search Item\n");
        printf("6. Find Possible Match\n");
        printf("7. Resolve Item\n");
        printf("8. View Statistics\n");
        printf("0. Exit\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        choice = readInteger();
        switch (choice)
        {
            case 1:
                reportLostItem();
                saveToFile(lostHead, LOST_FILE);
                saveToFile(foundHead, FOUND_FILE);
                break;
            case 2:
                reportFoundItem();
                saveToFile(lostHead, LOST_FILE);
                saveToFile(foundHead, FOUND_FILE);
                break;
            case 3:
                displayLostItems();
                break;
            case 4:
                displayFoundItems();
                break;
            case 5:
                searchItem();
                break;
            case 6:
                findMatches();
                break;
            case 7:
                resolveItem();
                saveToFile(lostHead, LOST_FILE);
                saveToFile(foundHead, FOUND_FILE);
                break;
            case 8:
                showStatistics();
                break;
            case 0:
                saveToFile(lostHead, LOST_FILE);
                saveToFile(foundHead, FOUND_FILE);
                printf("\nData saved successfully.\n");
                printf("Exiting program...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 0);
    freeList(lostHead);
    freeList(foundHead);
    freeHashTable();
    return 0;
}
