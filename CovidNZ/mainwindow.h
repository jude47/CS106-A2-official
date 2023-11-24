// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QRandomGenerator>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void login();
    void showAdminPanel();
    void showUserInfo();
    void editUserInfo();
    void showResults();
    void uploadDocument();
    void viewUploadedDocuments();
    void changeVaccinationStatus();
    void generateQRCode();
    void reportIssue();
    void createUser();

private:
    void setupUI();
    bool authenticate(const QString &username, const QString &password);
    QString generateRandomCovidResult();
    QString generateRandomVaccineResult();

    // Member variables
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *adminButton;
    QPushButton *resultsButton;
    QPushButton *qrCodeButton;
    QPushButton *createUserButton;
    QPushButton *reportIssueButton;
    QPushButton *showUserInfoButton;
    QPushButton *editUserInfoButton;
    QPushButton *uploadDocumentButton;
    QPushButton *viewDocumentsButton;
    QPushButton *changeStatusButton;

    QMap<QString, QString> userCredentials;
    QMap<QString, QString> userInfo;
    QString currentUser;
    bool authenticated;
    bool isAdmin;

    QVBoxLayout *layout;
};

#endif // MAINWINDOW_H
