#pragma once

#include <string>
#include "../database/database.h"

class User {
    public:
        User(Database* database);  // Constructor for guest
        User(Database* database, const std::string& username, const std::string& password, const int& iconColour,
             const int& totalWins, const int& totalLosses, const int& totalGames, const float& winPercentage, 
             const float& hitPercentage);

        void updateStats(const int& totalWins, const int& totalLosses, const int& totalGames, 
                         const float& winPercentage, const int& hits = 0, const int& turns = 0);

        void updateUserIcon(const int& iconColour);

        const std::string& getUsername();
        const int& getIconColour();
        const bool& getIsGuest();
        const int& getTotalWins();
        const int& getTotalLosses();
        const int& getTotalGames();
        const float& getWinPercentage();

    private:
        Database* m_database;
        const bool m_isGuest;
        
        std::string m_username = "GUEST";
        std::string m_password = {};
        int m_iconColour = {};
        int m_totalWins = {};
        int m_totalLosses = {};
        int m_totalGames = {};
        float m_winPercentage = {};
        float m_hitPercentage = {};

};
