#include <bits/stdc++.h>
using namespace std;

class Contact {
public:
    string name;
    string phone_number;
    string email;

    Contact(const string& n, const string& phone, const string& mail)
        : name(n), phone_number(phone), email(mail) {}
};

bool compareContacts(const Contact& a, const Contact& b) {
    return a.name < b.name;
}

bool isValidPhoneNumber(const string& phoneNumber) {
    return phoneNumber.size() == 10 && all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit);
}

bool isValidEmail(const string& email) {
    const regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4})");
    return regex_match(email, emailRegex);
}

void saveContactsToFile(const vector<Contact>& contacts) {
    ofstream file("contacts.txt");

    for (const Contact& contact : contacts) {
        file << contact.name << ',' << contact.phone_number << ',' << contact.email << '\n';
    }

    file.close();
}

vector<Contact> loadContactsFromFile() {
    vector<Contact> contacts;
    ifstream file("contacts.txt");

    if (!file.is_open()) {
        cerr << "Error: Unable to open the contacts file." << endl;
        return contacts;
    }

    string line;
    while (getline(file, line)) {
        string name, phone_number, email;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
            name = line.substr(0, pos1);
            phone_number = line.substr(pos1 + 1, pos2 - pos1 - 1);
            email = line.substr(pos2 + 1);

            contacts.push_back(Contact(name, phone_number, email));
        }
    }

    file.close();
    return contacts;
}

void displayContacts(const vector<Contact>& contacts) {
    cout << "Contacts:" << endl;
    for (const Contact& contact : contacts) {
        cout << "Name: " << contact.name << ", Phone: " << contact.phone_number << ", Email: " << contact.email << endl;
    }
}

Contact searchContact(const vector<Contact>& contacts, const string& name) {
    auto it = find_if(contacts.begin(), contacts.end(), [&name](const Contact& contact) {
        return contact.name == name;
    });

    if (it != contacts.end()) {
        return *it;
    } else {
        return Contact("", "", "");
    }
}

int main() {
    vector<Contact> contacts = loadContactsFromFile();

    while (true) {
        cout << "Contact Management System" << endl;
        cout << "1. Display sorted contacts" << endl;
        cout << "2. Add a contact" << endl;
        cout << "3. Search for a contact" << endl;
        cout << "4. Update a contact" << endl;
        cout << "5. Exit" << endl;

        int choice;
        cin >> choice;

        if (choice == 1) {
            sort(contacts.begin(), contacts.end(), compareContacts);
            displayContacts(contacts);
        } else if (choice == 2) {
            string name, phoneNumber, email;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);

            do {
                cout << "Enter a 10-digit phone number: ";
                cin >> phoneNumber;
            } while (!isValidPhoneNumber(phoneNumber));

            do {
                cout << "Enter a valid email address: ";
                cin >> email;
            } while (!isValidEmail(email));

            contacts.push_back(Contact(name, phoneNumber, email));
            saveContactsToFile(contacts);
        } else if (choice == 3) {
            string searchName;
            cout << "Enter the name of the contact to search: ";
            cin.ignore();
            getline(cin, searchName);

            Contact foundContact = searchContact(contacts, searchName);
            if (foundContact.name.empty()) {
                cout << "Contact not found." << endl;
            } else {
                cout << "Name: " << foundContact.name << ", Phone: " << foundContact.phone_number << ", Email: " << foundContact.email << endl;
            }
        } else if (choice == 4) {
            string searchName;
            cout << "Enter the name of the contact to update: ";
            cin.ignore();
            getline(cin, searchName);

            auto it = find_if(contacts.begin(), contacts.end(), [&searchName](const Contact& contact) {
                return contact.name == searchName;
            });

            if (it != contacts.end()) {
                string newName, newPhoneNumber, newEmail;
                cout << "Enter new name: ";
                getline(cin, newName);

                do {
                    cout << "Enter a 10-digit phone number: ";
                    cin >> newPhoneNumber;
                } while (!isValidPhoneNumber(newPhoneNumber));

                do {
                    cout << "Enter a valid email address: ";
                    cin >> newEmail;
                } while (!isValidEmail(newEmail));

                it->name = newName;
                it->phone_number = newPhoneNumber;
                it->email = newEmail;
                saveContactsToFile(contacts);
                cout << "Contact updated successfully." << endl;
            } else {
                cerr << "Contact not found." << endl;
            }
        } else if (choice == 5) {
            break;
        } else {
            cerr << "Invalid choice. Please select a valid option." << endl;
        }
    }

    return 0;
}
