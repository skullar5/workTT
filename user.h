#ifndef USER_H
#define USER_H

#include <QString>

class User {
private:
    QString userID;
    QString role;

public:
    User(const QString& id, const QString& role);
    QString getUserID();
    QString getRole();
    bool login();
    void logout();
};

#endif 
