#pragma once

#include <map>
#include <string>

class Card;

class CardFactory {
    public:
        CardFactory();

        Card createCard(const std::string& cardName);

    private:
        std::map<std::string, Card> cardPrototypes;
};
