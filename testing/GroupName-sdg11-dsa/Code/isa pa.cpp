#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// Define a Family struct
struct Family {
    string name;
    char type; // S/P/G/N
};

vector<Family> families;                 // Main family list
vector<pair<string, int>> foodPacks;     // Food packs
queue<Family> ayudaQueue;                // Queue now stores Family struct

// Check duplicate family
bool isDuplicate(string name)
{
    for (Family fam : families)
    {
        if (fam.name == name)
            return true;
    }
    return false;
}

// Remove family from the main list
void removeFamilyFromList(string name)
{
    for (int i = 0; i < families.size(); i++)
    {
        if (families[i].name == name)
        {
            families.erase(families.begin() + i);
            break;
        }
    }
}

// Register Family and add to queue instantly if priority
void registerFamily()
{
    string name;
    cout << "\nEnter family name: ";
    cin.ignore();
    getline(cin, name);

    if (isDuplicate(name))
    {
        cout << "Family already exists! Duplicate not allowed.\n";
        return;
    }

    char type;
    cout << "Is this family (S)enior / (P)WD / (G)Pregnant / (N)ormal? ";
    cin >> type;

    Family fam = {name, type};
    families.push_back(fam);

    // If priority, insert at front
    if (type == 'S' || type == 'P' || type == 'G' ||
        type == 's' || type == 'p' || type == 'g')
    {
        queue<Family> temp;
        temp.push(fam);               // priority family goes first
        while (!ayudaQueue.empty()) {
            temp.push(ayudaQueue.front());
            ayudaQueue.pop();
        }
        ayudaQueue = temp;
    }
    else
    {
        ayudaQueue.push(fam);         // normal family goes to back
    }

    // Keep main family list sorted alphabetically
    sort(families.begin(), families.end(), [](Family a, Family b){ return a.name < b.name; });

    cout << "Family registered successfully and added to queue!\n";
}

// Add Food Packs
void addFoodPack()
{
    string food;
    int qty;

    cout << "\nEnter food pack name: ";
    cin >> food;
    cout << "Enter quantity: ";
    cin >> qty;

    foodPacks.push_back({food, qty});

    // Simple insertion sort by quantity
    for (int i = 0; i < foodPacks.size(); i++)
    {
        for (int j = i + 1; j < foodPacks.size(); j++)
        {
            if (foodPacks[i].second > foodPacks[j].second)
                swap(foodPacks[i], foodPacks[j]);
        }
    }

    cout << "Food pack added!\n";
}

// Distribute Ayuda
void distribute()
{
    if (ayudaQueue.empty())
    {
        cout << "\nNo families in the queue.\n";
        return;
    }

    if (foodPacks.empty())
    {
        cout << "\nNo food packs in inventory.\n";
        return;
    }

    cout << "\n--- DISTRIBUTION STARTED ---\n";

    while (!ayudaQueue.empty() && !foodPacks.empty())
    {
        Family currentFamily = ayudaQueue.front();
        ayudaQueue.pop();

        cout << "Gave 1 food pack to: " << currentFamily.name << endl;

        removeFamilyFromList(currentFamily.name);

        // Deduct from first food pack
        foodPacks[0].second--;
        if (foodPacks[0].second == 0)
            foodPacks.erase(foodPacks.begin());
    }

    cout << "\n--- DISTRIBUTION FINISHED ---\n";
}

// Search Family
void searchFamily()
{
    string name;
    cout << "\nEnter family name to search: ";
    cin.ignore();
    getline(cin, name);

    if (isDuplicate(name))
        cout << "Family is REGISTERED in the system.\n";
    else
        cout << "Family NOT FOUND (possibly already served).\n";
}

// Display Families
void displayFamilies()
{
    cout << "\nREGISTERED FAMILIES:\n";

    if (families.empty())
    {
        cout << "No families left (All served).\n";
        return;
    }

    for (Family fam : families)
        cout << "- " << fam.name << endl;
}

// Display Food Packs
void displayFood()
{
    cout << "\nFOOD INVENTORY:\n";

    if (foodPacks.empty())
    {
        cout << "No food packs left.\n";
        return;
    }

    for (auto f : foodPacks)
        cout << f.first << " = " << f.second << endl;
}

// Display Queue with Priority Info
void displayQueue()
{
    if (ayudaQueue.empty())
    {
        cout << "\nQueue is empty.\n";
        return;
    }

    cout << "\nCURRENT AYUDA QUEUE:\n";
    queue<Family> temp = ayudaQueue;

    while (!temp.empty())
    {
        Family f = temp.front();
        temp.pop();

        string priorityStatus;
        if (f.type == 'S' || f.type == 'P' || f.type == 'G' ||
            f.type == 's' || f.type == 'p' || f.type == 'g')
            priorityStatus = "PRIORITY";
        else
            priorityStatus = "NORMAL";

        cout << "- " << f.name << " (" << priorityStatus << ")" << endl;
    }
}

// Pre-load Data (5 families + 10 food packs) and add to queue with priority
void loadSampleData()
{
    vector<Family> sample = {
        {"Navarro Family", 'S'},
        {"Garcia Family", 'N'},
        {"Reyes Family", 'P'},
        {"Cruz Family", 'G'},
        {"Santos Family", 'N'}
    };

    for (auto f : sample)
    {
        families.push_back(f);

        // Add to queue immediately
        if (f.type == 'S' || f.type == 'P' || f.type == 'G')
        {
            queue<Family> temp;
            temp.push(f);
            while (!ayudaQueue.empty()) {
                temp.push(ayudaQueue.front());
                ayudaQueue.pop();
            }
            ayudaQueue = temp;
        }
        else
        {
            ayudaQueue.push(f);
        }
    }

    sort(families.begin(), families.end(), [](Family a, Family b){ return a.name < b.name; });

    foodPacks.push_back({"Food Pack", 10});
}

// Main Menu
int main()
{
    loadSampleData();

    int choice;

    do
    {
        cout << "\n===== BARANGAY AYUDA SYSTEM =====\n";
        cout << "1. Register Family\n";
        cout << "2. Add Food Packs\n";
        cout << "3. Distribute Ayuda\n";
        cout << "4. Search Family\n";
        cout << "5. Show Families\n";
        cout << "6. Show Food Packs\n";
        cout << "7. Show Queue\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: registerFamily(); break;
        case 2: addFoodPack(); break;
        case 3: distribute(); break;
        case 4: searchFamily(); break;
        case 5: displayFamilies(); break;
        case 6: displayFood(); break;
        case 7: displayQueue(); break;
        case 8: cout << "System closed.\n"; break;
        default: cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}
