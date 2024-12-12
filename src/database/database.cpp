#include "database.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

DatabaseException::DatabaseException(const std::string &errorMessage) {
    message = errorMessage;
}

string DatabaseException::getMessage() const {
    return message;
}

Database::Database() {
    usernames = {};
    passwords = {};
    iconColours = {};
    totalWins = {};
    totalLosses = {};
    totalGames = {};
    winRates = {};
    hitPercentages = {};
}

void Database::load() {
    fstream databaseFile;

    databaseFile.open("database.txt");
    if (!databaseFile.is_open()) {
        throw DatabaseException("Database file not found");
    }

    int iconColour;
    string username, password;
    int totalWin, totalLosse, totalGame;
    float winRate, hitPercentage;

    while (databaseFile >> username >> password >> iconColour >> totalWin >> totalLosse >> totalGame >> winRate >> hitPercentage) {
        usernames.push_back(username);
        passwords.push_back(password);
        iconColours.push_back(static_cast<IconColour>(iconColour));
        totalWins.push_back(static_cast<int>(totalWin));
        totalLosses.push_back(static_cast<int>(totalLosse));
        totalGames.push_back(static_cast<int>(totalGame));
        winRates.push_back(winRate);
        hitPercentages.push_back(hitPercentage);
    }

    databaseFile.close();
}

void Database::pushToFile() {
    addContentToFile();
}

void Database::addContentToFile() {
    fstream databaseFile;

    databaseFile.open("database.txt");
    if (!databaseFile.is_open()) {
        throw DatabaseException("Database file not found");
    }

    for (int i = 0; i < usernames.size(); i++) {
        databaseFile << usernames[i] << " " << passwords[i] << " " << (int)iconColours[i] << " " << (int)totalWins[i] << " " << (int)totalLosses[i] << " " << (int)totalGames[i] << " " << winRates[i] << " " << hitPercentages[i] << endl;
    }

    databaseFile.close();
}

void Database::wipeFile() {
    fstream file;
    file.open("database.txt", ios::out | ios::trunc);
    file.close();
    cout << "Database content deleted" << endl;
}

int Database::findUser(const std::string &username, const std::string &password) {
    for (int i = 0; i < usernames.size(); i++) {
        if (usernames[i] == username && passwords[i] == password) {
            return i;
        }
    }
    return -1;
}

bool Database::checkUser(const std::string &username, const std::string &password) {
    if (findUser(username, password) != -1) {
        return true;
    }
    return false;
}

bool Database::checkUsernameExists(const std::string &username) {
    for (int i = 0; i < usernames.size(); i++) {
        if (usernames[i] == username) 
            return true;
    }
    
    return false;
}

bool Database::addBaseUser(const std::string &username, const std::string &password, enum IconColour iconColour) {
    if (checkUser(username, password)) {
        return false;
    }

    usernames.push_back(username);
    passwords.push_back(password);
    iconColours.push_back(static_cast<IconColour>(iconColour));
    totalWins.push_back(0);
    totalLosses.push_back(0);
    totalGames.push_back(0);
    winRates.push_back(0);
    hitPercentages.push_back(0);

    return true;
}

bool Database::changeUserIcon(const std::string &username, const std::string &password, enum IconColour iconColour) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return false;
    }

    iconColours[userIndex] = iconColour;
    return true;
}

bool Database::changeUserAllStats(const std::string &username, const std::string &password, const int totalWin, const int totalLoss, const int totalGame, const int hits, const int turns) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return false;
    }

    totalWins[userIndex] = totalWin;
    totalLosses[userIndex] = totalLoss;
    totalGames[userIndex] = totalGame;
    winRates[userIndex] = (float)totalWin / (float)totalGame;
    hitPercentages[userIndex] = 0;
    return true;
}

int Database::getUserTotalWins(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return -1;
    }

    return totalWins[userIndex];
}

int Database::getUserTotalLosses(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return -1;
    }

    return totalLosses[userIndex];
}

int Database::getUserTotalGames(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return -1;
    }

    return totalGames[userIndex];
}

float Database::getUserWinRate(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return -1;
    }

    return winRates[userIndex];
}

float Database::getUserHitPercentage(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return -1;
    }

    return hitPercentages[userIndex];
}

IconColour Database::getUserIcon(const std::string &username, const std::string &password) {
    int userIndex = findUser(username, password);
    if (userIndex == -1) {
        return IconColour::BLUE;
    }

    return iconColours[userIndex];
}

std::vector<std::unordered_map<std::string, std::string>> Database::getLeaderboardStats(
 const int statIndex, bool isGuest, std::string username) {

    std::vector<std::unordered_map<std::string, std::string>> result;
    std::vector<int> indices(usernames.size());
    std::iota(indices.begin(), indices.end(), 0);

    if (statIndex == stats::TotalWins) {

        // Create an index list of top totalWin scores
        std::sort(indices.begin(), indices.end(),
                [&](int A, int B) -> bool {
                        return totalWins[A] > totalWins[B];
                    });
    } else if (statIndex == stats::TotalGamesPlayed) {

        // Create an index list of top totalWin scores
        std::sort(indices.begin(), indices.end(),
                [&](int A, int B) -> bool {
                        return totalGames[A] > totalGames[B];
                    });
    }

    if (isGuest) {
        for (int i = 0; i < 5; i++) {
            std::unordered_map<std::string, std::string> userData;

            userData["username"] = usernames[indices[i]];
            userData["iconColour"] = std::to_string(static_cast<int>(iconColours[indices[i]]));
            userData["rankingNumber"] = std::to_string(i + 1);

            if (statIndex == stats::TotalWins) 
                userData["numberToDisplay"] = std::to_string(totalWins[indices[i]]);
            if (statIndex == stats::TotalGamesPlayed) 
                userData["numberToDisplay"] = std::to_string(totalGames[indices[i]]);

            result.push_back(userData);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            std::unordered_map<std::string, std::string> userData;

            userData["username"] = usernames[indices[i]];
            userData["iconColour"] = std::to_string(static_cast<int>(iconColours[indices[i]]));
            userData["rankingNumber"] = std::to_string(i + 1);

            if (statIndex == stats::TotalWins) 
                userData["numberToDisplay"] = std::to_string(totalWins[indices[i]]);
            if (statIndex == stats::TotalGamesPlayed) 
                userData["numberToDisplay"] = std::to_string(totalGames[indices[i]]);

            result.push_back(userData);
        }

        for (int i = 0; i < indices.size(); i++) {
            if (usernames[indices[i]] == username) {
                std::unordered_map<std::string, std::string> userData;

                userData["username"] = usernames[indices[i]];
                userData["iconColour"] = std::to_string(static_cast<int>(iconColours[indices[i]]));
                userData["rankingNumber"] = std::to_string(i + 1);

                if (statIndex == stats::TotalWins) 
                    userData["numberToDisplay"] = std::to_string(totalWins[indices[i]]);
                if (statIndex == stats::TotalGamesPlayed) 
                    userData["numberToDisplay"] = std::to_string(totalGames[indices[i]]);

                result.push_back(userData);
            }
        }
    }

    return result;
}
