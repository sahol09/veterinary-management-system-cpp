#pragma once
#include <string>
#include <map>

enum class Role { ADMIN, VET, STAFF, INVALID };

class LoginSystem {
private:
    std::map<std::string, std::pair<std::string, Role>> credentials;
public:
    LoginSystem();
    Role authenticate(const std::string& username, const std::string& password);
};

