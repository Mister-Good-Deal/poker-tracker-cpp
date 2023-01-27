#pragma once

#include <map>
#include <string>

class Card;

class CardFactory {
    public:
        CardFactory();

        Card create(const std::string& cardName);

    private:
        std::map<std::string, Card> cardPrototypes;
};
