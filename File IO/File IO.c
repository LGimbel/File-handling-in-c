#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void initializeList();
void printInventory();
void saveToFile();
void saveBackup();
bool inventoryExists();
void searchItem(char*);
void addItem();
void purchaseItem(char*);

struct item {
	char itemName[20];
	int itemQuantity;
	float itemUnitPrice;
	struct item* next;
};
typedef struct item item;
item* listPTR;
item* head; 
int main() {
	listPTR = NULL;
	head = NULL;
	bool exitFlag = false;
	int userChoice = 0;
	printf("Welcome to the inventory managment application\n\n\n.");
	system("pause");
	while (!exitFlag)
	{
		system("cls");
		printf("Please enter the number that coresponds to your desired action\n");
		printf("1:Initialize Inventory(Truncates current inventory)\n2:Add item\n3:View all items and inventory value\n4:Search for an item\n5:Purcase an item\n6:Quit\n");
		scanf("%d", &userChoice);
		switch (userChoice)
		{
		case 1: {
			initializeList();
		}
		case 2: {
			addItem();
			break;
		}
		case 3: {
			printInventory();
			break;

		}
		case 4: {
			printf("please enter the name of the item you would like to search for.");
			char targetItem[20];
			scanf("%s", targetItem);
			searchItem(targetItem);
			break;
		}
		case 5: {
			printf("please enter the name of the item you would like to purchase.");
			char targetItem[20];
			scanf("%s", targetItem);
			purchaseItem(targetItem);
			break;
			
		}
		case 6: {
			printf("goodbye!");
			system("pause");
			exitFlag = true;
			break;
		}
		default:
			printf("please try again.");
			system("pause");
			break;
		}

	}
}
void printInventory() {
	listPTR = head;
	double totalValue = 0;
	if (listPTR != NULL) {
		printf("ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);
		totalValue += (listPTR->itemUnitPrice * listPTR->itemQuantity);

		while (listPTR->next != NULL)
		{	listPTR = listPTR->next;
			printf("ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);
			totalValue += (listPTR->itemUnitPrice * listPTR->itemQuantity);
			

		}
		printf("The total value of the inventory is $%.2lf\n", totalValue);
	}
	else
	{
		printf("no items were added\n");
	}
	system("pause");
}
void initializeList() {
	int choice = 1;
	int counter = 0;
	item* newItem = NULL;

	
	while (choice != 0) {
		printf("Enter 1 to add an Item, 0 to stop: ");
		scanf("%d", &choice);

		if (choice == 1) {
			counter++;
			newItem = malloc(sizeof(item));

			if (counter == 1) {
				head = newItem;
				newItem->next = NULL;
			}
			else {
				newItem->next = head;
				head = newItem;
			}

			printf("Enter item name, price, current quantity: ");
			scanf("%s%f%d", newItem->itemName, &newItem->itemUnitPrice, &newItem->itemQuantity);
		}
	}
	saveToFile();
}
void saveToFile() {
	
	if (inventoryExists())
	{
		saveBackup();
	}
	FILE* inventory;
	const char* inventoryFilename = "inventory.txt";
	inventory = fopen(inventoryFilename, "w");
	listPTR = head;
	if (listPTR != NULL) {
		fprintf(inventory,"ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);


		while (listPTR->next != NULL)
		{
			listPTR = listPTR->next;
			fprintf(inventory,"ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);
			


		}
	}
	else
	{
		fprintf(inventory, "no items were added\n");
	}
	
	fclose(inventory);

}
void saveBackup() {
	FILE *inventory, *inventoryBackup;
	char IObuffer[1024];
	const char* inventoryFilename = "inventory.txt";
	const char* backupFilename = "inventoryBackup.txt";
	size_t bytesRead;
	inventory = fopen(inventoryFilename, "r");

	if (inventory == NULL) {
		printf("Error opening new inventory file.\n");
		return 1;
	}


	inventoryBackup = fopen(backupFilename, "w");
	if (inventory == NULL) {
		printf("Error opening backup file.\n");
		fclose(inventory);
		return 1;
	}
	while ((bytesRead = fread(IObuffer, sizeof(char), sizeof(IObuffer), inventory)) > 0) {
		fwrite(IObuffer, sizeof(char), bytesRead, inventoryBackup);
	};
	fclose(inventory);
	fclose(inventoryBackup);
}
bool inventoryExists() {
	FILE* inventory;
	const char* inventoryFilename = "inventory.txt";
	inventory = fopen(inventoryFilename, "r");
	bool exists = inventory != NULL;
	if (exists) {
		fclose(inventory);
	}
	return exists;
}
void searchItem(char* target) {
	listPTR = head;
	bool targetFound = false;
	if (listPTR != NULL) {
		
		targetFound = (strcmp(target, listPTR->itemName) == 0);

		while (listPTR->next != NULL && !targetFound)
		{
			listPTR = listPTR->next;
			targetFound = (strcmp(target, listPTR->itemName) == 0);
			


		}
		
	}
	else
	{
		printf("Item not found due to the inventory being empty\n");
	}
	if (targetFound) {
		printf("Target found\n");
		printf("ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);
	}
	else
	{
		printf("target not found");
	}
	system("pause");

}
void addItem() {
	item* newItem = NULL;
	newItem = malloc(sizeof(item));
	newItem->next = head;
	head = newItem;
	printf("Enter item name, price, current quantity: ");
	scanf("%s%f%d", newItem->itemName, &newItem->itemUnitPrice, &newItem->itemQuantity);
	printf("Item:%s added.", newItem->itemName);
	saveToFile();
} 
void purchaseItem(char* target) {
	listPTR = head;
	const char* notEnoughProductMessage = "The amount requested is more than the product has in stoct.";
	const char* purchaseMessage = "The total for your purchase will be $";
	bool targetFound = false;
	int purchaseAmount = 0;
	if (listPTR != NULL) {

		targetFound = (strcmp(target, listPTR->itemName) == 0);

		while (listPTR->next != NULL && !targetFound)
		{
			listPTR = listPTR->next;
			targetFound = (strcmp(target, listPTR->itemName) == 0);



		}

	}
	else
	{
		printf("Item not found due to the inventory being empty\n");
	}
	if (targetFound) {
		printf("Target found\n");
		printf("ItemName:%s\nPrice:%.2f\nQuantity:%d\n", listPTR->itemName, listPTR->itemUnitPrice, listPTR->itemQuantity);
		printf("How many would you like to purchase, max%d\n", listPTR->itemQuantity);
		purchaseAmount = scanf("%d", &purchaseAmount);
		printf(purchaseAmount > listPTR->itemQuantity ? notEnoughProductMessage : purchaseMessage);
		if (!purchaseAmount > listPTR->itemQuantity) {
			printf("%d", purchaseAmount * listPTR->itemUnitPrice);
			system("pause");
			listPTR->itemQuantity -= purchaseAmount;
			saveToFile();
		}
		else
		{
			system("pause");
		}

	}
	else
	{
		printf("target not found");
	}

}

