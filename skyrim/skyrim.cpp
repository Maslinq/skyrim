
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

enum ItemType {
    POTION,
    WEAPON,
    ARMOR,
    OTHER
};


struct Item {
    string name;
    string description;
    ItemType type;
    int value;
    int price;
    int bonus;

    Item(string n, string desc, ItemType t, int v, int p, int b = 0)
        : name(n), description(desc), type(t), value(v), price(p), bonus(b) {
    }
};


class Inventory {
private:
    vector<Item> items;
    int capacity;

public:
    Inventory(int cap = 10) : capacity(cap) {}


    bool addItem(const Item& item) {
        if (items.size() < capacity) {
            items.push_back(item);
            return true;
        }
        return false;
    }


    bool removeItem(int index) {
        if (index >= 0 && index < items.size()) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }


    Item useItem(int index) {
        if (index >= 0 && index < items.size()) {
            Item usedItem = items[index];
            items.erase(items.begin() + index);
            return usedItem;
        }
        return Item("", "", OTHER, 0, 0);
    }


    void display() const {
        if (items.empty()) {
            cout << "Инвентарь пуст" << endl;
            return;
        }

        cout << "\n=== ИНВЕНТАРЬ ===" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". " << items[i].name;
            cout << " - " << items[i].description;
            cout << " (" << items[i].value << " ед.)";
            cout << " - " << getTypeName(items[i].type) << endl;
        }
        cout << "Свободно: " << (capacity - items.size()) << "/" << capacity << " слотов" << endl;
    }


    Item getItem(int index) const {
        if (index >= 0 && index < items.size()) {
            return items[index];
        }
        return Item("", "", OTHER, 0, 0);
    }


    size_t getSize() const { return items.size(); }


    vector<int> findItemsByType(ItemType type) const {
        vector<int> indices;
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].type == type) {
                indices.push_back(i);
            }
        }
        return indices;
    }

private:
    string getTypeName(ItemType type) const {
        switch (type) {
        case POTION: return "Зелье";
        case WEAPON: return "Оружие";
        case ARMOR: return "Броня";
        default: return "Прочее";
        }
    }
};


class Player {
private:
    string name;
    int health;
    int maxHealth;
    int level;
    int experience;
    int gold;
    int skillPoints;


    int strength;
    int defense;
    int vitality;


    Item currentWeapon;
    Item currentArmor;

    Inventory inventory;

public:
    Player(const string& playerName)
        : name(playerName), health(100), maxHealth(100), level(1), experience(0),
        gold(100), skillPoints(0), strength(5), defense(5), vitality(5),
        currentWeapon("Кулаки", "Ваши голые руки", WEAPON, 0, 0, 2),
        currentArmor("Простая одежда", "Обычная одежда", ARMOR, 0, 0, 1)
    {

        inventory.addItem(Item("Зелье здоровья", "Восстанавливает 30 HP", POTION, 30, 15));
    }


    void displayStatus() const {
        cout << "\n=== СТАТУС ИГРОКА ===" << endl;
        cout << "Имя: " << name << endl;
        cout << "Уровень: " << level << endl;
        cout << "Здоровье: " << health << "/" << maxHealth << endl;
        cout << "Опыт: " << experience << "/" << (level * 100) << endl;
        cout << "Золото: " << gold << " монет" << endl;
        cout << "Очки навыков: " << skillPoints << endl;
        cout << "\n=== ХАРАКТЕРИСТИКИ ===" << endl;
        cout << "Сила: " << strength << " (+" << getAttack() - 2 << " к атаке)" << endl;
        cout << "Защита: " << defense << " (+" << getDefense() - 1 << " к защите)" << endl;
        cout << "Живучесть: " << vitality << " (+" << getMaxHealth() - 100 << " к здоровью)" << endl;
        cout << "\n=== ЭКИПИРОВКА ===" << endl;
        cout << "Оружие: " << currentWeapon.name << " (+" << currentWeapon.bonus << " атаки)" << endl;
        cout << "Броня: " << currentArmor.name << " (+" << currentArmor.bonus << " защиты)" << endl;
    }


    int getAttack() const {
        return strength + currentWeapon.bonus;
    }

    int getDefense() const {
        return defense + currentArmor.bonus;
    }

    int getMaxHealth() const {
        return maxHealth + (vitality * 5);
    }


    void takeDamage(int damage) {
        int actualDamage = damage - getDefense() / 2;
        if (actualDamage < 1) actualDamage = 1;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > getMaxHealth()) health = getMaxHealth();
    }

    void addExperience(int exp) {
        experience += exp;

        while (experience >= level * 100) {
            experience -= level * 100;
            levelUp();
        }
    }

    void levelUp() {
        level++;
        skillPoints++;
        maxHealth = 100 + (vitality * 5);
        health = maxHealth;
        cout << "\n Поздравляем! Вы достигли " << level << " уровня!" << endl;
        cout << "Получено 1 очко навыка!" << endl;
    }


    void upgradeStrength() {
        if (skillPoints > 0) {
            strength++;
            skillPoints--;
            cout << "Сила увеличена до " << strength << "!" << endl;
        }
    }

    void upgradeDefense() {
        if (skillPoints > 0) {
            defense++;
            skillPoints--;
            cout << "Защита увеличена до " << defense << "!" << endl;
        }
    }

    void upgradeVitality() {
        if (skillPoints > 0) {
            vitality++;
            int oldMaxHealth = maxHealth;
            maxHealth = 100 + (vitality * 5);
            health += (maxHealth - oldMaxHealth);
            skillPoints--;
            cout << "Живучесть увеличена до " << vitality << "!" << endl;
        }
    }


    void showInventory() {
        inventory.display();
    }

    bool addItemToInventory(const Item& item) {
        return inventory.addItem(item);
    }

    void useItemFromInventory() {
        if (inventory.getSize() == 0) {
            cout << "Инвентарь пуст!" << endl;
            return;
        }

        showInventory();
        cout << "Выберите предмет для использования (0 - отмена): ";
        int choice;
        cin >> choice;

        if (choice == 0) return;

        if (choice > 0 && choice <= inventory.getSize()) {
            Item usedItem = inventory.useItem(choice - 1);

            switch (usedItem.type) {
            case POTION:
                heal(usedItem.value);
                cout << "Использовано " << usedItem.name << "! +" << usedItem.value << " HP" << endl;
                break;
            case WEAPON:
                currentWeapon = usedItem;
                cout << "Экипировано: " << usedItem.name << endl;
                break;
            case ARMOR:
                currentArmor = usedItem;
                cout << "Экипировано: " << usedItem.name << endl;
                break;
            default:
                cout << "Предмет использован: " << usedItem.name << endl;
                break;
            }
        }
        else {
            cout << "Неверный выбор!" << endl;
        }
    }


    string getName() const { return name; }
    int getHealth() const { return health; }
    int getLevel() const { return level; }
    int getGold() const { return gold; }
    int getSkillPoints() const { return skillPoints; }
    bool isAlive() const { return health > 0; }

    void addGold(int amount) { gold += amount; }
    bool spendGold(int amount) {
        if (gold >= amount) {
            gold -= amount;
            return true;
        }
        return false;
    }
};

class Location {
protected:
    string name;
    string description;

public:
    Location(const string& locName, const string& locDesc)
        : name(locName), description(locDesc) {
    }

    virtual ~Location() = default;

    virtual void enter() = 0;
    virtual void explore() = 0;

    string getName() const { return name; }
    string getDescription() const { return description; }
};


class City : public Location {
private:
    vector<Item> shopItems;

public:
    City() : Location("Город", "Многолюдный город с различными заведениями") {

        shopItems.push_back(Item("Зелье здоровья", "Восстанавливает 30 HP", POTION, 30, 15));
        shopItems.push_back(Item("Большое зелье здоровья", "Восстанавливает 60 HP", POTION, 60, 30));
        shopItems.push_back(Item("Железный меч", "Простой железный меч", WEAPON, 0, 50, 5));
        shopItems.push_back(Item("Стальной меч", "Качественный стальной меч", WEAPON, 0, 100, 8));
        shopItems.push_back(Item("Кожаная броня", "Легкая кожаная броня", ARMOR, 0, 40, 3));
        shopItems.push_back(Item("Кольчуга", "Надежная кольчуга", ARMOR, 0, 80, 6));
    }

    void enter() override {
        cout << "\n  Вы входите в величественный город Вайтран." << endl;
        cout << "Перед вами оживленные улицы, полные торговцев и горожан." << endl;
    }

    void explore() override {
        cout << "\nКуда вы хотите пойти?" << endl;
        cout << "1. Магазин" << endl;
        cout << "2. Гарцующая кобыла (восстановление здоровья)" << endl;
        cout << "3. Тренировочный зал (прокачка характеристик)" << endl;
        cout << "4. Вернуться к выбору локации" << endl;
    }

    void visitShop(Player& player) {
        cout << "\n Добро пожаловать в магазин!" << endl;
        cout << "Ваше золото: " << player.getGold() << " монет" << endl;

        bool shopping = true;
        while (shopping) {
            cout << "\n=== МАГАЗИН ===" << endl;
            for (size_t i = 0; i < shopItems.size(); i++) {
                cout << i + 1 << ". " << shopItems[i].name;
                cout << " - " << shopItems[i].description;
                cout << " - " << shopItems[i].price << " золота" << endl;
            }
            cout << "0. Выйти из магазина" << endl;
            cout << "Выберите товар для покупки: ";

            int choice;
            cin >> choice;

            if (choice == 0) {
                shopping = false;
            }
            else if (choice > 0 && choice <= shopItems.size()) {
                Item item = shopItems[choice - 1];
                if (player.spendGold(item.price)) {
                    if (player.addItemToInventory(item)) {
                        cout << "Вы купили: " << item.name << "!" << endl;
                    }
                    else {
                        cout << "Недостаточно места в инвентаре!" << endl;
                        player.addGold(item.price);
                    }
                }
                else {
                    cout << "Недостаточно золота!" << endl;
                }
            }
            else {
                cout << "Неверный выбор!" << endl;
            }
        }
    }

    void visitInn(Player& player) {
        cout << "\n Вы заходите в гостиницу 'Гарцующая кобыла'." << endl;
        cout << "Хозяин предлагает отдохнуть за 20 золота." << endl;

        cout << "Отдохнуть? (1 - да, 0 - нет): ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (player.spendGold(20)) {
                player.heal(1000);
                cout << "Вы хорошо отдохнули и полностью восстановили здоровье!" << endl;
            }
            else {
                cout << "У вас недостаточно золота!" << endl;
            }
        }
    }

    void visitTrainingHall(Player& player) {
        cout << "\n Вы приходите в тренировочный зал." << endl;

        bool training = true;
        while (training && player.getSkillPoints() > 0) {
            cout << "\nОчки навыков: " << player.getSkillPoints() << endl;
            cout << "Что будем прокачивать?" << endl;
            cout << "1. Сила (увеличивает атаку)" << endl;
            cout << "2. Защита (увеличивает защиту)" << endl;
            cout << "3. Живучесть (увеличивает здоровье)" << endl;
            cout << "0. Выйти" << endl;
            cout << "Выберите: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                player.upgradeStrength();
                break;
            case 2:
                player.upgradeDefense();
                break;
            case 3:
                player.upgradeVitality();
                break;
            case 0:
                training = false;
                break;
            default:
                cout << "Неверный выбор!" << endl;
            }

            if (player.getSkillPoints() == 0) {
                cout << "Очки навыков закончились!" << endl;
            }
        }

        if (player.getSkillPoints() == 0) {
            cout << "У вас нет очков навыков для прокачки." << endl;
        }
    }
};


class Dungeon : public Location {
private:
    int difficulty;
    bool inDungeon;

public:
    Dungeon(int diff = 1)
        : Location("Подземелье", "Мрачное и опасное подземелье с монстрами"),
        difficulty(diff), inDungeon(false) {
    }

    void enter() override {
        cout << "\n Вы спускаетесь в мрачное подземелье." << endl;
        cout << "Воздух наполнен запахом плесени и опасности..." << endl;
        inDungeon = true;
    }

    void explore() override {
        cout << "\nЧто вы хотите сделать?" << endl;
        cout << "1. Исследовать дальше" << endl;
        cout << "2. Искать сокровища" << endl;
        cout << "3. Использовать предмет" << endl;
        cout << "4. Вернуться на поверхность" << endl;
    }

    bool isInDungeon() const { return inDungeon; }
    void leaveDungeon() { inDungeon = false; }

    void exploreFurther(Player& player) {
        cout << "\nВы осторожно продвигаетесь глубже в подземелье..." << endl;


        int event = rand() % 3;

        switch (event) {
        case 0:
            cout << " Вы встретили скелета!" << endl;
            fightMonster(player, "Скелет", 15, 20);
            break;
        case 1:
            cout << "  Вы наткнулись на гигантского паука!" << endl;
            fightMonster(player, "Гигантский паук", 20, 30);
            break;
        case 2:
            cout << " Вы нашли заброшенную комнату с сундуком!" << endl;
            findTreasure(player);
            break;
        }
    }

    void searchForTreasure(Player& player) {
        cout << "\nВы тщательно обыскиваете помещение..." << endl;

        if (rand() % 100 < 60) {
            int goldFound = (rand() % 30 + 10) * difficulty;
            int expFound = (rand() % 20 + 5) * difficulty;

            cout << " Вы нашли сундук с сокровищами!" << endl;
            cout << "Получено: " << goldFound << " золота и " << expFound << " опыта!" << endl;

            player.addGold(goldFound);
            player.addExperience(expFound);
        }
        else {
            cout << "К сожалению, вы ничего не нашли." << endl;
        }
    }

private:
    void fightMonster(Player& player, const string& monsterName, int monsterDamage, int expReward) {
        cout << "Начинается бой с " << monsterName << "!" << endl;


        int damage = (monsterDamage * difficulty) - (player.getDefense() / 3);
        if (damage < 5) damage = 5;

        player.takeDamage(damage);
        cout << monsterName << " наносит вам " << damage << " урона!" << endl;

        if (player.isAlive()) {
            cout << " Вы побеждаете " << monsterName << "!" << endl;
            int goldReward = (rand() % 25 + 10) * difficulty;
            cout << "Награда: " << goldReward << " золота и " << expReward * difficulty << " опыта!" << endl;

            player.addGold(goldReward);
            player.addExperience(expReward * difficulty);
        }
        else {
            cout << " Вы пали в бою..." << endl;
        }
    }

    void findTreasure(Player& player) {
        int goldAmount = (rand() % 50 + 25) * difficulty;
        int expAmount = (rand() % 15 + 10) * difficulty;

        cout << " В сундуке вы находите:" << endl;
        cout << "- " << goldAmount << " золотых монет" << endl;
        cout << "- " << expAmount << " единиц опыта" << endl;

        player.addGold(goldAmount);
        player.addExperience(expAmount);
    }
};


class Game {
private:
    Player player;
    vector<Location*> locations;
    bool gameRunning;

public:
    Game(const string& playerName) : player(playerName), gameRunning(true) {

        locations.push_back(new City());
        locations.push_back(new Dungeon(1));

        srand(time(0));
    }

    ~Game() {
        for (auto location : locations) {
            delete location;
        }
    }

    void run() {
        cout << " Добро пожаловать в игру!" << endl;
        cout << "========================" << endl;

        while (gameRunning && player.isAlive()) {
            player.displayStatus();
            showMainMenu();
        }

        if (!player.isAlive()) {
            cout << "\n Игра окончена! Вы погибли..." << endl;
        }
        else {
            cout << "\n Спасибо за игру!" << endl;
        }
    }

private:
    void showMainMenu() {
        cout << "\n ГЛАВНОЕ МЕНЮ" << endl;
        cout << "1. Отправиться в локацию" << endl;
        cout << "2. Инвентарь" << endl;
        cout << "3. Использовать предмет" << endl;
        cout << "4. Выйти из игры" << endl;
        cout << "Выберите вариант: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            showLocationMenu();
            break;
        case 2:
            player.showInventory();
            break;
        case 3:
            player.useItemFromInventory();
            break;
        case 4:
            gameRunning = false;
            cout << "До свидания!" << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
        }
    }

    void showLocationMenu() {
        cout << "\n Куда вы хотите отправиться?" << endl;
        cout << "1. Город" << endl;
        cout << "2. Подземелье" << endl;
        cout << "3. Вернуться в главное меню" << endl;
        cout << "Выберите вариант: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            visitCity();
            break;
        case 2:
            visitDungeon();
            break;
        case 3:
            return;
        default:
            cout << "Неверный выбор!" << endl;
        }
    }

    void visitCity() {
        City* city = dynamic_cast<City*>(locations[0]);
        city->enter();

        bool inCity = true;
        while (inCity && player.isAlive()) {
            city->explore();
            cout << "Выберите действие: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                city->visitShop(player);
                break;
            case 2:
                city->visitInn(player);
                break;
            case 3:
                city->visitTrainingHall(player);
                break;
            case 4:
                inCity = false;
                break;
            default:
                cout << "Неверный выбор!" << endl;
            }
        }
    }

    void visitDungeon() {
        Dungeon* dungeon = dynamic_cast<Dungeon*>(locations[1]);
        dungeon->enter();

        bool inDungeon = true;
        while (inDungeon && player.isAlive()) {
            dungeon->explore();
            cout << "Выберите действие: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                dungeon->exploreFurther(player);
                break;
            case 2:
                dungeon->searchForTreasure(player);
                break;
            case 3:
                player.useItemFromInventory();
                break;
            case 4:
                dungeon->leaveDungeon();
                inDungeon = false;
                cout << "Вы благополучно выбрались из подземелья." << endl;
                break;
            default:
                cout << "Неверный выбор!" << endl;
            }

            if (inDungeon) {
                player.displayStatus();
            }
        }
    }
};


int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Введите имя вашего персонажа: ";
    string playerName;
    getline(cin, playerName);

    Game game(playerName);
    game.run();

    return 0;
}
