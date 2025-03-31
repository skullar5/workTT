#include "user.h"

User::User(const QString& id, const QString& role) : userID(id), role(role) {}

QString User::getUserID() { return userID; }
QString User::getRole() { return role; }

bool User::login() {
    return !userID.isEmpty();
}

void User::logout() {
    userID = "";
}
