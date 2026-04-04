#include <iostream>
using namespace std;

// Function declarations (you will implement these)
void cityAMenu();
void cityBMenu();
void cityCMenu();
void compareCities();

void displayMainMenu()
{
    cout << "\n============================================\n";
    cout << "     CITYVILLE CARBON ANALYSIS SYSTEM\n";
    cout << "           (LINKED LIST VERSION)\n";
    cout << "============================================\n";

    cout << "\nSelect an option:\n";
    cout << "1. City A (Metropolitan City)\n";
    cout << "2. City B (University Town)\n";
    cout << "3. City C (Suburban / Rural Area)\n";
    cout << "\n4. Compare All Cities (A, B, C)\n";
    cout << "0. Exit\n";

    cout << "\nEnter your choice: ";
}

int main()
{
    int choice;

    do
    {
        displayMainMenu();
        cin >> choice;

        switch(choice)
        {
            case 1:
                cityAMenu();
                break;

            case 2:
                cityBMenu();
                break;

            case 3:
                cityCMenu();
                break;

            case 4:
                compareCities();
                break;

            case 0:
                cout << "\nExiting program. Goodbye!\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while(choice != 0);

    return 0;
}


void cityAMenu()


{
    int choice;

    do
    {
        cout << "\n============================================\n";
        cout << "              CITY A MENU\n";
        cout << "============================================\n";

        cout << "1. View Data\n";
        cout << "2. Add Resident\n";
        cout << "3. Sort Data\n";
        cout << "4. Search Data\n";
        cout << "5. Carbon Analysis\n";
        cout << "6. Age Group Analysis\n";

        cout << "9. Back to Main Menu\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                // cityA.display();
                break;

            case 2:
                // cityA.insert(...);
                break;

            case 3:
                // sorting function
                break;

            case 4:
                // search function
                break;

            case 5:
                // emission analysis
                break;

            case 6:
                // age analysis
                break;

            case 9:
                return;

            case 0:
                exit(0);

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 9);
}

void cityBMenu()
{
    int choice;

    do
    {
        cout << "\n============================================\n";
        cout << "              CITY B MENU\n";
        cout << "============================================\n";

        cout << "1. View Data\n";
        cout << "2. Add Resident\n";
        cout << "3. Sort Data\n";
        cout << "4. Search Data\n";
        cout << "5. Carbon Analysis\n";
        cout << "6. Age Group Analysis\n";

        cout << "9. Back to Main Menu\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                // cityA.display();
                break;

            case 2:
                // cityA.insert(...);
                break;

            case 3:
                // sorting function
                break;

            case 4:
                // search function
                break;

            case 5:
                // emission analysis
                break;

            case 6:
                // age analysis
                break;

            case 9:
                return;

            case 0:
                exit(0);

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 9);
}

void cityCMenu()
{
    int choice;

    do
    {
        cout << "\n============================================\n";
        cout << "              CITY C MENU\n";
        cout << "============================================\n";

        cout << "1. View Data\n";
        cout << "2. Add Resident\n";
        cout << "3. Sort Data\n";
        cout << "4. Search Data\n";
        cout << "5. Carbon Analysis\n";
        cout << "6. Age Group Analysis\n";

        cout << "9. Back to Main Menu\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                // cityA.display();
                break;

            case 2:
                // cityA.insert(...);
                break;

            case 3:
                // sorting function
                break;

            case 4:
                // search function
                break;

            case 5:
                // emission analysis
                break;

            case 6:
                // age analysis
                break;

            case 9:
                return;

            case 0:
                exit(0);

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 9);
}

void compareCities()
{
    cout << "Compare Cities not implemented yet.\n";
}