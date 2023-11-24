#include "mainwindow.h"
#include <QInputDialog>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QPlainTextEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), authenticated(false), isAdmin(false)
{
    setupUI();
}

void MainWindow::setupUI()
{
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::login);

    adminButton = new QPushButton("Admin Panel", this);
    connect(adminButton, &QPushButton::clicked, this, &MainWindow::showAdminPanel);

    resultsButton = new QPushButton("Show Results", this);
    connect(resultsButton, &QPushButton::clicked, this, &MainWindow::showResults);

    qrCodeButton = new QPushButton("Generate QR Code", this);
    connect(qrCodeButton, &QPushButton::clicked, this, &MainWindow::generateQRCode);

    createUserButton = new QPushButton("Create User", this);
    connect(createUserButton, &QPushButton::clicked, this, &MainWindow::createUser);

    reportIssueButton = new QPushButton("Report Issue", this);
    connect(reportIssueButton, &QPushButton::clicked, this, &MainWindow::reportIssue);

    showUserInfoButton = new QPushButton("Show User Info", this);
    connect(showUserInfoButton, &QPushButton::clicked, this, &MainWindow::showUserInfo);

    editUserInfoButton = new QPushButton("Edit User Info", this);
    connect(editUserInfoButton, &QPushButton::clicked, this, &MainWindow::editUserInfo);

    viewDocumentsButton = new QPushButton("View Documents", this);
    connect(viewDocumentsButton, &QPushButton::clicked, this, &MainWindow::viewUploadedDocuments);
    viewDocumentsButton->setEnabled(false);

    uploadDocumentButton = new QPushButton("Upload Document", this);
    connect(uploadDocumentButton, &QPushButton::clicked, this, &MainWindow::uploadDocument);

    changeStatusButton = new QPushButton("Change Status", this);
    connect(changeStatusButton, &QPushButton::clicked, this, &MainWindow::changeVaccinationStatus);
    changeStatusButton->setEnabled(false);

    layout = new QVBoxLayout;

    layout->addSpacing(10);

    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(usernameLineEdit);
    layout->addSpacing(5);
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passwordLineEdit);
    layout->addSpacing(5);
    layout->addWidget(loginButton);
    layout->addWidget(adminButton);
    layout->addWidget(resultsButton);
    layout->addWidget(qrCodeButton);
    layout->addWidget(createUserButton);
    layout->addWidget(uploadDocumentButton);
    layout->addWidget(reportIssueButton);
    layout->addWidget(showUserInfoButton);
    layout->addWidget(editUserInfoButton);
    layout->addWidget(viewDocumentsButton);

    layout->addSpacing(10);

    layout->addWidget(changeStatusButton);

    changeStatusButton->setEnabled(true);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);

    setCentralWidget(widget);
}

bool MainWindow::authenticate(const QString &username, const QString &password)
{
    if (userCredentials.contains(username) && userCredentials.value(username) == password)
    {
        currentUser = username;
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::login()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (authenticate(username, password))
    {
        authenticated = true;
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
    }
    else
    {
        authenticated = false;
        QMessageBox::warning(this, "Login Failed", "Invalid username or password. Please try again.");
    }
}

void MainWindow::showAdminPanel()
{
    if (authenticated)
    {
        QString adminPassword = QInputDialog::getText(this, "Admin Panel", "Enter admin password:", QLineEdit::Password);

        if (adminPassword == "admin")
        {
            isAdmin = true;
            QMessageBox::information(this, "Admin Panel", "Welcome, Admin!");
            adminButton->setEnabled(false);
            viewDocumentsButton->setEnabled(true);
        }
        else
        {
            isAdmin = false;
            QMessageBox::warning(this, "Access Denied", "Incorrect admin password. Access denied.");
        }
    }
    else
    {
        isAdmin = false;
        QMessageBox::warning(this, "Access Denied", "You need to create a user account to access this feature.");
    }
}

void MainWindow::showUserInfo()
{
    if (isAdmin)
    {
        QString userInfoText = "User Information:\n";

        for (auto it = userInfo.begin(); it != userInfo.end(); ++it)
        {
            currentUser = it.key();
            userInfoText += "Username: " + it.key() + "\n";
            userInfoText += "Covid Result: " + generateRandomCovidResult() + "\n";
            userInfoText += "Vaccine Result: " + generateRandomVaccineResult() + "\n";
            userInfoText += "QR Code: [Generated]\n";
            userInfoText += "-----------------\n";
        }

        QMessageBox::information(this, "User Information", userInfoText);
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need admin privileges to access this feature.");
    }
}

void MainWindow::editUserInfo()
{
    if (isAdmin)
    {
        QString username = QInputDialog::getText(this, "Edit User Info", "Enter username to edit:");
        if (userCredentials.contains(username))
        {
            QString newPassword = QInputDialog::getText(this, "Edit User Info", "Enter new password:");
            userInfo[username] = newPassword;
            QMessageBox::information(this, "User Info Edited", "User info for " + username + " edited successfully.");
        }
        else
        {
            QMessageBox::warning(this, "User Not Found", "User with the specified username does not exist.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need admin privileges to access this feature.");
    }
}

void MainWindow::showResults()
{
    if (authenticated)
    {
        QString covidResult = generateRandomCovidResult();
        QString vaccineResult = generateRandomVaccineResult();

        QString resultMessage = "COVID Result: " + covidResult + "\nVaccine Result: " + vaccineResult;
        QMessageBox::information(this, "Results", resultMessage);
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need to create a user account to access this feature.");
    }
}

void MainWindow::uploadDocument()
{
    if (authenticated && isAdmin)
    {
        QString username = QInputDialog::getText(this, "Upload Document", "Enter username to upload document:");

        if (userCredentials.contains(username))
        {
            QString filePath = QFileDialog::getOpenFileName(this, "Select Document", QDir::homePath(), "All Files (*)");

            if (!filePath.isEmpty())
            {
                // You can save the document path in userInfo or another appropriate data structure
                userInfo[username] = filePath;
                QMessageBox::information(this, "Document Uploaded", "Document uploaded for user " + username);
            }
            else
            {
                QMessageBox::warning(this, "File Selection Cancelled", "Document upload cancelled.");
            }
        }
        else
        {
            QMessageBox::warning(this, "User Not Found", "User with the specified username does not exist.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need admin privileges to access this feature.");
    }
}

void MainWindow::changeVaccinationStatus()
{
    if (authenticated && isAdmin)
    {
        QString username = QInputDialog::getText(this, "Change Status", "Enter username to change vaccination status:");

        if (userCredentials.contains(username))
        {
            QString newStatus = QInputDialog::getItem(this, "Change Status", "Select new vaccination status:", QStringList() << "Not Vaccinated" << "Partially Vaccinated" << "Fully Vaccinated");

            if (!newStatus.isEmpty())
            {
                QMessageBox::information(this, "Status Changed", "Vaccination status changed for user " + username + " to: " + newStatus);
            }
            else
            {
                QMessageBox::warning(this, "Selection Cancelled", "Status change cancelled.");
            }
        }
        else
        {
            QMessageBox::warning(this, "User Not Found", "User with the specified username does not exist.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need admin privileges to access this feature.");
    }
}

void MainWindow::viewUploadedDocuments()
{
    if (isAdmin)
    {
        QString username = QInputDialog::getText(this, "View Documents", "Enter username to view documents:");

        if (userCredentials.contains(username))
        {
            if (userInfo.contains(username))
            {
                QString filePath = userInfo.value(username);

                // Display the document path or open the document with the associated application
                QMessageBox::information(this, "Uploaded Documents", "Document path for user " + username + ": " + filePath);
            }
            else
            {
                QMessageBox::warning(this, "Document Not Found", "No document uploaded for user " + username);
            }
        }
        else
        {
            QMessageBox::warning(this, "User Not Found", "User with the specified username does not exist.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need admin privileges to access this feature.");
    }
}



QString MainWindow::generateRandomCovidResult()
{
    int randomValue = QRandomGenerator::global()->bounded(3);

    if (randomValue == 0)
        return "Positive. Please follow health guidelines.";
    else if (randomValue == 1)
        return "Negative. Stay safe!";
    else
        return "Haven't been tested. Consider getting tested.";
}

QString MainWindow::generateRandomVaccineResult()
{
    int randomValue = QRandomGenerator::global()->bounded(3);

    if (randomValue == 0)
        return "Not Vaccinated";
    else if (randomValue == 1)
        return "Partially Vaccinated";
    else
        return "Fully Vaccinated";
}

void MainWindow::generateQRCode()
{
    if (authenticated)
    {
        if (userCredentials.contains(currentUser))
        {
            QString covidResult = userCredentials.value(currentUser);
            QImage qrCodeImage(200, 200, QImage::Format_RGB32);
            qrCodeImage.fill(Qt::white);

            QPainter painter(&qrCodeImage);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setBrush(Qt::black);

            QString qrData = currentUser + "\n" + covidResult;

            for (int i = 0; i < qrData.size(); ++i)
            {
                if (qrData.at(i) == '1')
                    painter.drawRect(i % qrCodeImage.width(), i / qrCodeImage.width(), 1, 1);
            }

            painter.end();

            QMessageBox msgBox;
            msgBox.setWindowTitle("QR Code");
            msgBox.setIconPixmap(QPixmap::fromImage(qrCodeImage));
            msgBox.exec();
        }
        else
        {
            QMessageBox::warning(this, "User Not Found", "User with the specified username does not exist.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need to log in to access this feature.");
    }
}

void MainWindow::reportIssue()
{
    if (authenticated)
    {
        QPlainTextEdit *textEdit = new QPlainTextEdit(this);
        textEdit->setPlaceholderText("Describe the issue/bug here...");

        QMessageBox reportIssueBox;
        reportIssueBox.setWindowTitle("Report Issue");
        reportIssueBox.setIcon(QMessageBox::Information);
        reportIssueBox.setText("Please describe the issue/bug:");
        reportIssueBox.addButton(QMessageBox::Ok);
        reportIssueBox.addButton(QMessageBox::Cancel);
        reportIssueBox.setDefaultButton(QMessageBox::Ok);

        reportIssueBox.layout()->addWidget(textEdit);

        if (reportIssueBox.exec() == QMessageBox::Ok)
        {
            QString issueDescription = textEdit->toPlainText();
            QMessageBox::information(this, "Issue Reported", "Issue reported:\n" + issueDescription);
        }
    }
    else
    {
        QMessageBox::warning(this, "Access Denied", "You need to create a user account to access this feature.");
    }
}

void MainWindow::createUser()
{
    QString newUsername = QInputDialog::getText(this, "Create User", "Enter new username:");
    QString newPassword = QInputDialog::getText(this, "Create User", "Enter new password:", QLineEdit::Password);

    userCredentials.insert(newUsername, newPassword);
    userInfo.insert(newUsername, generateRandomCovidResult());
    QMessageBox::information(this, "User Created", "User '" + newUsername + "' created successfully!");
}

// MAINWINDOW_CPP

