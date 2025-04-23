#include <stdlib.h>

class Shop {
    public:
    
    /*
    Initialization of shop using master list of items. 
    (Vector or set that contains all existing items, probably from parsed config)
    */
    Shop();

    /* Called when player returns from dungeon. 
    Restock shop based on config. */
    void restock();

    /* Buys an item. */
    bool buy(Player& player, const std::string& category, int index);
    bool sell(Player& player, Item& item);
    
    /* Display inventory on CLI (for debugging). */
    void getShopCatalogue() const;

    private:
    /* Vector of vector of potions */
    /* Vector of vector of weapons */
    /* Vector of vector of armors */
    /* Vector of vector of pendants */
};