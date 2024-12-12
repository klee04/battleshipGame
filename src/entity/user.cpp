#include "user.hpp"
#include <cmath>

User::User(Database* database) : m_database(database), m_isGuest(true) {}

User::User(Database* database, const std::string& username, const std::string& password, const int& iconColour, 
 const int& totalWins, const int& totalLosses, const int& totalGames, const float& winPercentage, const float& hitPercentage)
 : m_database(database), m_isGuest(false), m_username(username), m_password(password), m_iconColour(iconColour),
 m_totalWins(totalWins), m_totalLosses(totalLosses), m_totalGames(totalGames), 
 m_winPercentage(winPercentage), m_hitPercentage(hitPercentage) {}

void User::updateStats(const int& totalWins, const int& totalLosses, const int& totalGames, 
 const float& winPercentage, const int& hits, const int& turns) {
    if (m_isGuest) return;

    m_totalWins = totalWins;
    m_totalLosses = totalLosses;
    m_totalGames = totalGames;
    m_winPercentage = winPercentage;
    m_hitPercentage = 0;

    m_database->changeUserAllStats(m_username, m_password, m_totalWins, m_totalLosses, m_totalGames, 0, 0);
    m_database->pushToFile();
}

void User::updateUserIcon(const int& iconColour) {
    m_iconColour = iconColour;

    m_database->changeUserIcon(m_username, m_password, static_cast<IconColour>(m_iconColour));
    m_database->pushToFile();
}

const std::string& User::getUsername() { return m_username; }
const int& User::getIconColour() { return m_iconColour; }
const bool& User::getIsGuest() { return m_isGuest; }
const int& User::getTotalWins() { return m_totalWins; }
const int& User::getTotalLosses() { return m_totalLosses; }
const int& User::getTotalGames() { return m_totalGames; }
const float& User::getWinPercentage() { return m_winPercentage; }
