#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
#include <limits>
#include <cstdlib>

// Issue structure
struct Issue {
    std::string date;
    std::string description;
};

// Record structure
struct Record {
    std::string type;
    std::string date;
};

struct User {
    std::string username;
    std::string password;
    std::string fullName;
    std::vector<Record> records;
    std::vector<Issue> issues;
    std::vector<std::string> userDocuments;
    std::vector<std::string> covidTestResults;  //
    std::string vaccinationStatus;  //
};


struct Administrator {
    std::string username;
    std::string password;
};

struct UserSession {
    User* user;
};


std::map<std::string, User> users;
std::map<std::string, Administrator> admins = {
    {"Admin", {"Admin", "administrator"}}
};

// User sessions
std::map<std::string, UserSession> userSessions;

// Logging functions
void logMessage(const std::string& message, const std::string& fileName) {
    std::ofstream logFile(fileName, std::ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.close();
    } else {
        std::cerr << "Error opening log file: " << fileName << std::endl;
    }
}

void logCertificateUpload(const std::string& username, const std::string& date) {
    std::string message = "Certificate uploaded by " + username + " on " + date;
    logMessage(message, "certificate_log.txt");
}

void logTestResultsUpload(const std::string& username, const std::string& date) {
    std::string message = "Test results uploaded by " + username + " on " + date;
    logMessage(message, "test_results_log.txt");
}

void logIssue(const Issue& issue) {
    std::string message = "Issue reported on " + issue.date + ":\n" + issue.description + "\n";
    logMessage(message, "issue_log.txt");
}

// Function to get the current date and time as a string
std::string getCurrentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

// Function to create log files if they don't exist
void createLogFiles() {
    std::ofstream certificateLogFile("certificate_log.txt");
    std::ofstream testResultsLogFile("test_results_log.txt");
    std::ofstream issueLogFile("issue_log.txt");
}


// Function to create a new account
void createUser() {
    User newUser;
    std::string username, password, fullName;

    std::cout << "Enter new username: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        std::cout << "Username already exists. Account creation failed.\n";
        return;
    }

    std::cout << "Enter new password: ";
    std::cin >> password;
    std::cout << "Enter full name: ";
    std::cin.ignore();
    std::getline(std::cin, fullName);

    newUser.username = username;
    newUser.password = password;
    newUser.fullName = fullName;

    users[username] = newUser;

    std::cout << "Account created successfully!\n";
}

// User login
bool loginUser(std::string username, std::string password) {
    if (users.find(username) != users.end() && users[username].password == password) {
        return true;
    }
    return false;
}

// Administrator login
bool loginAdmin(std::string username, std::string password) {
    if (admins.find(username) != admins.end() && admins[username].password == password) {
        return true;
    }
    return false;
}

// Function to view and modify users' information
void viewAndModifyUsers() {
    std::cout << "View and Modify Users' Information\n";

}

void viewAndModifyDocuments(User& user) {
    std::cout << "View and Modify Documents:\n";
    std::cout << "User Documents:\n";

    for (const std::string& document : user.userDocuments) {
        std::cout << document << "\n";
    }

    if (admins.find(user.username) != admins.end()) {
        std::cout << "Administrator Options:\n";
        std::cout << "1. Edit Test Result\n";
        std::cout << "2. View COVID-19 Test Results\n";
        std::cout << "3. View Vaccination Status\n";
        std::cout << "4. Back to Admin Menu\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {

        } else if (choice == 2) {
            std::cout << "COVID-19 Test Results:\n";
            for (const std::string& testResult : user.covidTestResults) {
                std::cout << testResult << "\n";
            }
        } else if (choice == 3) {
            std::cout << "Vaccination Status: " << user.vaccinationStatus << "\n";
        } else if (choice == 4) {
            return;
        } else {
            std::cout << "Invalid choice. Please select a valid option.\n";
        }
    } else {
        std::cout << "4. Back to User Menu\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 4) {
            return;
        } else {
            std::cout << "Invalid choice. Please select a valid option.\n";
        }
    }
}




// Function to upload and maintain users' documents
void uploadAndMaintainDocuments(User& user) {
    std::cout << "Upload and Maintain Users' Documents:\n";
    std::string documentName;
    std::cout << "Enter the name of the document to upload: ";
    std::cin.ignore();
    std::getline(std::cin, documentName);

    if (documentName.find("COVID-19 Test Result:") != std::string::npos) {
        std::cout << "Enter the test result (Negative/Positive): ";
        std::string testResult;
        std::cin >> testResult;
        std::string resultMessage = "COVID-19 Test Result: " + testResult;
        user.userDocuments.push_back(resultMessage);
    }

    // Append the document name to the user's documents
    user.userDocuments.push_back(documentName);

    std::cout << "Document uploaded successfully!\n";
}

// Function to generate a random COVID-19 test result
std::string generateRandomTestResult() {
    int randomResult = std::rand() % 2; // 0 for negative, 1 for positive
    return (randomResult == 0) ? "Negative" : "Positive";
}

// Function to generate a random number of vaccines taken
int generateRandomVaccines() {
    return std::rand() % 3; // 0, 1, or 2 vaccines
}

// Function to view COVID-19 tests and vaccine records
void viewRecords(User& user) {
    std::cout << "COVID-19 Tests and Vaccine Records:\n";

    bool hasTestResult = false;
    for (const std::string& document : user.userDocuments) {
        if (document.find("COVID-19 Test Result:") != std::string::npos) {
            hasTestResult = true;
            break;
        }
    }
    if (!hasTestResult) {
        std::string randomTestResult = "COVID-19 Test Result: " + generateRandomTestResult();
        user.userDocuments.push_back(randomTestResult);
        std::cout << "Random COVID-19 test result generated: " << randomTestResult << "\n";
    }

    // Generate a random number of vaccines taken if not present
    bool hasVaccineRecord = false;
    for (const Record& record : user.records) {
        if (record.type == "Vaccine") {
            hasVaccineRecord = true;
            break;
        }
    }
    if (!hasVaccineRecord) {
        int numVaccines = generateRandomVaccines();
        std::string vaccineRecord = "Vaccine Record: " + std::to_string(numVaccines) + " vaccines taken";
        user.records.push_back({ "Vaccine", getCurrentDateTime() });
        std::cout << "Random vaccine record generated: " << vaccineRecord << "\n";
    }

    // Display COVID-19 test results
    std::cout << "COVID-19 Test Results:\n";
    for (const std::string& document : user.userDocuments) {
        if (document.find("COVID-19 Test Result:") != std::string::npos) {
            std::cout << document << "\n";
        }
    }

    // Display vaccine records
    std::cout << "Vaccine Records:\n";
    for (const Record& record : user.records) {
        if (record.type == "Vaccine") {
            std::cout << "Date: " << record.date << "\n";
        }
    }
}


void uploadQRCode(User& user) {
    std::cout << "Uploading a Unique QR Code\n";
}

// Function for users to report issues
void reportIssue(User& user) {
    std::cout << "Reporting an Issue\n";
    Issue newIssue;
    newIssue.date = getCurrentDateTime();
    std::cout << "Describe the issue: ";
    std::cin.ignore();
    std::getline(std::cin, newIssue.description);
    user.issues.push_back(newIssue);
    logIssue(newIssue);

    std::cout << "Issue reported successfully!\n";
}

// Function to start a user session
void startUserSession(User& user) {
    UserSession session;
    session.user = &user;
    userSessions[user.username] = session;
}

// Function to check if a user is logged in
bool isUserLoggedIn(const std::string& username) {
    return userSessions.find(username) != userSessions.end();
}

void userMenu(User& user) {
    startUserSession(user);

    int userChoice;

    while (true) {
        std::cout << "User Menu:\n";
        std::cout << "1. View and Modify Documents\n";
        std::cout << "2. View COVID-19 Tests and Vaccine Records\n";
        std::cout << "3. Upload QR Code\n";
        std::cout << "4. Report Issue\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> userChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (userChoice == 1) {
            viewAndModifyDocuments(user);
        } else if (userChoice == 2) {
            viewRecords(user);
        } else if (userChoice == 3) {
            uploadQRCode(user);
        } else if (userChoice == 4) {
            reportIssue(user);
        } else if (userChoice == 5) {
            std::cout << "Logging out...\n";
            userSessions.erase(user.username);
            return;
        } else {
            std::cout << "Invalid choice. Please select a valid option.\n";
        }
    }
}

// Function to view a user's reports
void viewUserReports(const User& user) {
    std::cout << "User Reports for " << user.username << ":\n";
    for (const Issue& issue : user.issues) {
        std::cout << "Date: " << issue.date << "\n";
        std::cout << "Description: " << issue.description << "\n";
        std::cout << "\n";
    }
}

// Function to view COVID-19 test results for a user
void viewUserTestResults(const User& user) {
    std::cout << "COVID-19 Test Results for " << user.username << ":\n";
    for (const std::string& testResult : user.covidTestResults) {
        std::cout << testResult << "\n";
    }
}

// Function for the admin to view a user's information
void viewUserInformation() {
    std::cout << "View User Information:\n";
    std::string username;
    std::cout << "Enter the username to view information: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "User Information for " << user.username << ":\n";
        std::cout << "Full Name: " << user.fullName << "\n";
        std::cout << "Vaccination Status: " << user.vaccinationStatus << "\n";

        if (admins.find(username) != admins.end()) {
            std::cout << "Administrator Options:\n";
            std::cout << "1. View Reports\n";
            std::cout << "2. View COVID-19 Test Results\n";
            std::cout << "3. Back to Admin Menu\n";
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    viewUserReports(user);
                    break;
                case 2:
                    viewUserTestResults(user);
                    break;
                case 3:
                    return;
                default:
                    std::cout << "Invalid choice. Please select a valid option.\n";
            }
        } else {
            std::cout << "1. Back to Admin Menu\n";
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                return;
            } else {
                std::cout << "Invalid choice. Please select a valid option.\n";
            }
        }
    } else {
        std::cout << "User not found.\n";
    }
}

void viewAndModifyDocumentsForUser() {
    std::string username;
    std::cout << "Enter the username of the user whose documents you want to view and modify: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "View and Modify Documents for User: " << user.username << "\n";
        std::cout << "User Documents:\n";

        for (const std::string& document : user.userDocuments) {
            std::cout << document << "\n";
        }

    } else {
        std::cout << "User not found.\n";
    }
}

void viewUserReportsForUser() {
    std::string username;
    std::cout << "Enter the username of the user whose reports you want to view: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "User Reports for " << user.username << ":\n";
        for (const Issue& issue : user.issues) {
            std::cout << "Date: " << issue.date << "\n";
            std::cout << "Description: " << issue.description << "\n";
            std::cout << "\n";
        }
    } else {
        std::cout << "User not found.\n";
    }
}

// Function to view COVID-19 test results for a specific user
void viewCOVIDTestResultsForUser() {
    std::string username;
    std::cout << "Enter the username of the user whose COVID-19 test results you want to view: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "COVID-19 Test Results for User: " << user.username << "\n";

        for (const std::string& testResult : user.covidTestResults) {
            std::cout << testResult << "\n";
        }
    } else {
        std::cout << "User not found.\n";
    }
}

// Function to view vaccination status for a specific user
void viewVaccinationStatusForUser() {
    std::string username;
    std::cout << "Enter the username of the user whose vaccination status you want to view: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "Vaccination Status for User: " << user.username << "\n";
        std::cout << "Vaccination Status: " << user.vaccinationStatus << "\n";
    } else {
        std::cout << "User not found.\n";
    }
}

// Function to view QR code for a specific user
void viewQRCodeForUser() {
    std::string username;
    std::cout << "Enter the username of the user for whom you want to view the QR code: ";
    std::cin >> username;

    if (users.find(username) != users.end()) {
        User& user = users[username];

        std::cout << "QR Code for User: " << user.username << "\n";

    } else {
        std::cout << "User not found.\n";
    }
}

void adminMenu() {
    int adminChoice;

    while (true) {
        std::cout << "Admin Menu:\n";
        std::cout << "1. View and Modify Users' Information\n";
        std::cout << "2. View and Modify Documents for User\n";
        std::cout << "3. View User Reports\n";
        std::cout << "4. View COVID-19 Test Results for User\n";
        std::cout << "5. View Vaccination Status for User\n";
        std::cout << "6. View QR Code for User\n";
        std::cout << "7. Log Out\n";
        std::cout << "Enter your choice: ";
        std::cin >> adminChoice;
        std::cin.ignore();

        switch (adminChoice) {
            case 1:
                viewAndModifyUsers();
                break;
            case 2:
                viewAndModifyDocumentsForUser();
                break;
            case 3:
                viewUserReportsForUser();
                break;
            case 4:
                viewCOVIDTestResultsForUser();
                break;
            case 5:
                viewVaccinationStatusForUser();
                break;
            case 6:
                viewQRCodeForUser();
                break;
            case 7:
                std::cout << "Logging out...\n";
                return;
            default:
                std::cout << "Invalid choice. Please select a valid option.\n";
        }
    }
}

int main() {
    createLogFiles();

    int choice;
    std::string username, password;

    while (true) {
        std::cout << "1. Create Account\n2. Log In\n3. Admin Login\n4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::cin.ignore();

        if (choice == 1) {
            createUser();
        } else if (choice == 2) {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;

            if (loginUser(username, password)) {
                User& loggedInUser = users[username];
                userMenu(loggedInUser);
            } else {
                std::cout << "Login failed. Invalid username or password.\n";
            }
        } else if (choice == 3) {
            std::cout << "Enter admin username: ";
            std::getline(std::cin, username);
            std::cout << "Enter admin password: ";
            std::getline(std::cin, password);

            if (loginAdmin(username, password)) {
                adminMenu();
            } else {
                std::cout << "Admin login failed. Invalid username or password.\n";
            }
        } else if (choice == 4) {
            std::cout << "Exiting...\n";
            break;
        } else {
            std::cout << "Invalid choice. Please select a valid option.\n";
        }
    }

    return 0;
}
