#include "LoginSystem.h"

LoginSystem::LoginSystem() {
    // username -> {password, role}
    credentials["admin"] = { "admin123", Role::ADMIN };
    credentials["vet"] = { "vet123", Role::VET };
    credentials["staff"] = { "staff123", Role::STAFF };
}

Role LoginSystem::authenticate(const std::string& username, const std::string& password) {
    if (credentials.find(username) != credentials.end()) {
        if (credentials[username].first == password)
            return credentials[username].second;
    }
    return Role::INVALID;
}
