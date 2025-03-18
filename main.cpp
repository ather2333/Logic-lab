#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <ctime>
using namespace std;

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// ANSI escape codes for color formatting
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

struct Request {
    string first_name, cnic, reason, gun_name, status;
};

map<string, string> users = { {"admin", "admin123"} };

void logAction(const string& action, const string& cnic) {
    ofstream logFile("log.txt", ios::app);
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back(); 
    logFile << "[" << dt << "] " << action << " (CNIC: " << cnic << ")\n";
    logFile.close();
}

bool authenticate() {
    string username, password;
    cout << "\nEnter Username: "; cin >> username;
    cout << "Enter Password: "; cin >> password;
    if (users.find(username) != users.end() && users[username] == password) {
        cout << GREEN << "\n? Authentication Successful!" << RESET << "\n";
        return true;
    }
    cout << RED << "\n? Invalid Credentials!" << RESET << "\n";
    return false;
}

void enterDetails();
void checkStatus();
void viewRequests();
void approveRejectRequests();

void displayUserMenu() {
    system(CLEAR_SCREEN);
    cout << BLUE << "\n=====================================" << RESET;
    cout << BLUE << "\n       User Menu" << RESET;
    cout << BLUE << "\n=====================================" << RESET;
    cout << CYAN << "\n1??  Check Status" << RESET;
    cout << CYAN << "\n2??  Enter Details" << RESET;
    cout << CYAN << "\n3??  Back" << RESET;
    cout << BLUE << "\n=====================================" << RESET;
    cout << "\n?? Enter your choice: ";
}

void displayAdminMenu() {
    system(CLEAR_SCREEN);
    cout << BLUE << "\n=====================================" << RESET;
    cout << BLUE << "\n       Admin Menu" << RESET;
    cout << BLUE << "\n=====================================" << RESET;
    cout << CYAN << "\n1??  View Requests" << RESET;
    cout << CYAN << "\n2??  Approve/Reject Requests" << RESET;
    cout << CYAN << "\n3??  Back" << RESET;
    cout << BLUE << "\n=====================================" << RESET;
    cout << "\n?? Enter your choice: ";
}

void enterDetails() {
    system(CLEAR_SCREEN);
    ofstream file("requests.txt", ios::app);
    Request req;
    cout << YELLOW << "\nEnter First Name: " << RESET; cin >> req.first_name;
    cout << YELLOW << "Enter CNIC (without dashes): " << RESET; cin >> req.cnic;
    cout << YELLOW << "Enter Reason: " << RESET; cin.ignore(); getline(cin, req.reason);
    cout << YELLOW << "Enter Gun Name: " << RESET; getline(cin, req.gun_name);
    req.status = "Pending";
    
    file << req.first_name << "," << req.cnic << "," << req.reason << "," << req.gun_name << "," << req.status << "\n";
    file.close();
    logAction("Request Submitted", req.cnic);
    cout << GREEN << "\n? Request submitted successfully!" << RESET << "\n";
    system("pause");
}

void checkStatus() {
    system(CLEAR_SCREEN);
    ifstream file("requests.txt");
    string cnic, line;
    cout << YELLOW << "\nEnter CNIC to check status: " << RESET; cin >> cnic;
    bool found = false;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string first_name, stored_cnic, reason, gun_name, status;
        getline(ss, first_name, ',');
        getline(ss, stored_cnic, ',');
        getline(ss, reason, ',');
        getline(ss, gun_name, ',');
        getline(ss, status, ',');
        
        if (stored_cnic == cnic) {
            cout << GREEN << "\n? Status: " << status << RESET << "\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << RED << "\n? CNIC not found!" << RESET << "\n";
    system("pause");
}

void viewRequests() {
    system(CLEAR_SCREEN);
    ifstream file("requests.txt");
    string line;
    
    // Check if the file is empty, and add headers if needed
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << RED << "\n? No requests found!" << RESET << "\n";
    } else {
        cout << YELLOW << "\n================ License Requests ================" << RESET;
        cout << left << setw(15) << "First Name" << setw(20) << "CNIC" << setw(25) << "Reason" << setw(15) << "Gun Name" << setw(10) << "Status" << endl;
        cout << "--------------------------------------------------------\n";
        
        while (getline(file, line)) {
            stringstream ss(line);
            string first_name, cnic, reason, gun_name, status;
            getline(ss, first_name, ',');
            getline(ss, cnic, ',');
            getline(ss, reason, ',');
            getline(ss, gun_name, ',');
            getline(ss, status, ',');
            
            cout << left << setw(15) << first_name << setw(20) << cnic << setw(25) << reason << setw(15) << gun_name << setw(10) << status << endl;
        }
    }
    file.close();
    system("pause");
}

void approveRejectRequests() {
    system(CLEAR_SCREEN);
    ifstream file("requests.txt");
    vector<Request> requests;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        Request req;
        getline(ss, req.first_name, ',');
        getline(ss, req.cnic, ',');
        getline(ss, req.reason, ',');
        getline(ss, req.gun_name, ',');
        getline(ss, req.status, ',');
        requests.push_back(req);
    }
    file.close();

    cout << YELLOW << "\n================ Approve/Reject Requests ================" << RESET;
    cout << "\nIndex  CNIC             Name            Status\n";
    cout << "--------------------------------------------------------\n";
    for (int i = 0; i < requests.size(); ++i) {
        cout << i+1 << "     " << requests[i].cnic << "     " << requests[i].first_name << "       " << requests[i].status << endl;
    }

    int index;
    cout << YELLOW << "\nEnter request index to approve/reject (0 to cancel): " << RESET;
    cin >> index;

    if (index > 0 && index <= requests.size()) {
        string decision;
        cout << YELLOW << "Enter 'approve' to approve or 'reject' to reject: " << RESET;
        cin >> decision;

        if (decision == "approve") {
            requests[index - 1].status = "Approved";
            logAction("Request Approved", requests[index - 1].cnic);
        } else if (decision == "reject") {
            requests[index - 1].status = "Rejected";
            logAction("Request Rejected", requests[index - 1].cnic);
        }

        // Update file with new statuses
        ofstream outFile("requests.txt");
        outFile << left << setw(15) << "First Name" << setw(20) << "CNIC" << setw(25) << "Reason" << setw(15) << "Gun Name" << setw(10) << "Status" << endl;
        outFile << "--------------------------------------------------------\n";
        for (const auto& req : requests) {
            outFile << left << setw(15) << req.first_name << setw(20) << req.cnic << setw(25) << req.reason << setw(15) << req.gun_name << setw(10) << req.status << endl;
        }
        outFile.close();
        cout << GREEN << "\n? Status updated successfully!" << RESET << "\n";
    } else {
        cout << RED << "\n? Invalid index. Going back..." << RESET << "\n";
    }
    system("pause");
}

int main() {
    while (true) {
        system(CLEAR_SCREEN);
        cout << BLUE << "\n=====================================" << RESET;
        cout << BLUE << "\n       Gun License System" << RESET;
        cout << BLUE << "\n=====================================" << RESET;
        cout << CYAN << "\n1??  User" << RESET;
        cout << CYAN << "\n2??  Admin" << RESET;
        cout << CYAN << "\n3??  Exit" << RESET;
        cout << BLUE << "\n=====================================" << RESET;
        cout << "\n?? Enter your choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            while (true) {
                displayUserMenu();
                int userChoice;
                cin >> userChoice;
                if (userChoice == 1) checkStatus();
                else if (userChoice == 2) enterDetails();
                else break;
            }
        } else if (choice == 2) {
            if (authenticate()) {
                while (true) {
                    displayAdminMenu();
                    int adminChoice;
                    cin >> adminChoice;
                    if (adminChoice == 1) viewRequests();
                    else if (adminChoice == 2) approveRejectRequests();
                    else break;
                }
            }
        } else if (choice == 3) {
            cout << RED << "\n?? Exiting system..." << RESET << "\n";
            break;
        }
    }
    return 0;
}
