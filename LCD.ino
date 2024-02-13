#include <LiquidCrystal.h>

#define WHEEL_LEN 16
#define REELS 3
#define BASE_MULTIPLIER 1  // how much to multiply win

#define LCD_BACKLIGHT 10
#define BUTTONS A0
#define RANDOM_PIN A5

enum buttons {
    NO_BUTTON,
    SELECT,
    LEFT,
    DOWN,
    UP,
    RIGHT
};
enum fruits {
    TWO,
    DINO,
    TREE,
    HEAD,
    CHERRY,
    APPLE,
    PEAR,
    BANANA
};
const char dino[8] PROGMEM = {0x06, 0x0D, 0x0F, 0x0E, 0x1F, 0x1E, 0x0A, 0x0A};
const char tree[8] PROGMEM = {0x04, 0x1F, 0x0E, 0x04, 0x1F, 0x0E, 0x04, 0x04};
const char head[8] PROGMEM = {0x00, 0x0E, 0x15, 0x1F, 0x0E, 0x00, 0x0E, 0x00};
const char two[8] PROGMEM = {0x07, 0x01, 0x01, 0x17, 0x04, 0x04, 0x07, 0x00};
const char cherry[8] PROGMEM = {0x10, 0x08, 0x0C, 0x0B, 0x0B, 0x08, 0x0C, 0x0C};
const char apple[8] PROGMEM = {0x04, 0x04, 0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 0x00};
const char pear[8] PROGMEM = {0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x1F, 0x0E};
const char banana[8] PROGMEM = {0x02, 0x04, 0x0C, 0x0C, 0x18, 0x1C, 0x0E, 0x06};
const char* const fruitsChars[8] PROGMEM = {two, dino, tree, head, cherry, apple, pear, banana};

buttons getButton() {
    int val = analogRead(BUTTONS);
    if (val > 1000) return NO_BUTTON;
    if (val < 50) return RIGHT;
    if (val < 195) return UP;
    if (val < 380) return DOWN;
    if (val < 555) return LEFT;
    if (val < 790) return SELECT;
    return NO_BUTTON;
}

class automat {
   private:
    uint16_t price(void) {
        Serial.println("Price");
        if (wheel[spinnedState[0]] == fruits::DINO && wheel[spinnedState[1]] == fruits::TREE && wheel[spinnedState[2]] == fruits::TREE) {  // dino tree tree
            return BASE_MULTIPLIER * 8;
        }
        uint8_t sortedState[REELS];  // sorted state, translate wheels
        for (uint8_t i = 0; i < REELS; i++) {
            sortedState[i] = wheel[spinnedState[i]];
        }
        sortState(sortedState);
        Serial.println(String(sortedState[0]) + " " + String(sortedState[1]) + " " + String(sortedState[2]));
        if (sortedState[0] == fruits::HEAD && sortedState[2] == fruits::HEAD) {  // all heads
            return BASE_MULTIPLIER * 16;
        }
        for (uint8_t i = 0; i < REELS; i++) {  // head
            if (sortedState[i] == fruits::HEAD) {
                return 0;
            }
        }
        if (sortedState[0] == fruits::TWO && sortedState[2] == fruits::TWO) {  // all twos
            return BASE_MULTIPLIER * 16;
        }
        if (sortedState[0] == fruits::TWO && sortedState[1] == fruits::TWO) {  // two two
            return BASE_MULTIPLIER * 4;
        }
        if (sortedState[0] == fruits::TWO) {  // one two
            return BASE_MULTIPLIER * 2;
        }
        if (sortedState[0] == fruits::CHERRY && sortedState[2] == fruits::CHERRY) {  // all cherries
            return BASE_MULTIPLIER * 8;
        }
        if (sortedState[0] == fruits::PEAR && sortedState[2] == fruits::PEAR) {  // all pears
            return BASE_MULTIPLIER * 8;
        }
        if (sortedState[0] == sortedState[2]) {  // all the same
            return BASE_MULTIPLIER * 8;
        }
        if (isFruit(sortedState[0]) && isFruit(sortedState[1]) && isFruit(sortedState[2])) {  // all fruits
            return BASE_MULTIPLIER * 2;
        }
        return 0;
    }
    void sortState(uint8_t* sortedState) {
        for (uint8_t i = 0; i < REELS; i++) {
            for (uint8_t j = i + 1; j < REELS; j++) {
                if (sortedState[i] > sortedState[j]) {
                    uint8_t temp = sortedState[i];
                    sortedState[i] = sortedState[j];
                    sortedState[j] = temp;
                }
            }
        }
    }
    bool isFruit(uint8_t fruit) {
        switch (fruit) {
            case fruits::CHERRY:
            case fruits::APPLE:
            case fruits::PEAR:
            case fruits::BANANA:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    void spin(void) {
        for (size_t i = 0; i < REELS; i++) {
            spinnedState[i] = random(WHEEL_LEN);
        }
        lastWin = price() * bet;
    }
    void showState(void) {
        lcd.setCursor(13, 0);
        for (size_t i = 0; i < REELS; i++) {
            lcd.write(wheel[state[i]]);
        }
    }
    bool isFinalState() {
        for (uint8_t i = 0; i < REELS; i++) {
            if (spinnedState[i] != state[i]) {
                return false;
            }
        }
        return true;
    }
    void showSpin(void) {
        for (size_t i = 0; i < WHEEL_LEN; i++) {
            showState();
            for (size_t i = 0; i < REELS; i++) {
                state[i]++;
                state[i] %= WHEEL_LEN;
            }
            delay(100);
        }
        while (true) {
            if (isFinalState()) {
                break;
            }
            for (size_t i = 0; i < REELS; i++) {
                if (state[i] != spinnedState[i]) {
                    state[i]++;
                    state[i] %= WHEEL_LEN;
                }
            }
            showState();
            delay(150);
        }
    }
    void showWin(void) {
        lcd.setCursor(5, 1);
        lcd.print(lastWin);
    }
    void showMat(void) {
        lcd.clear();
        showBet();
        lcd.setCursor(0, 1);
        lcd.print("Win: ");
        showState();
    }
    void showBet(void) {
        lcd.setCursor(5, 0);
        lcd.print(bet);
    }
    void clearBet(void) {
        lcd.setCursor(5, 0);
        lcd.print("           ");
    }
    void showOrder(uint32_t order) {
        lcd.setCursor(7, 1);
        lcd.print(order);
    }
    void clearOrder(void) {
        lcd.setCursor(7, 1);
        lcd.print("         ");
    }
    void showSelect(void) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bet: ");
        lcd.setCursor(0, 1);
        lcd.print("Order: ");
    }

    LiquidCrystal& lcd;
    uint8_t* wheel;
    uint8_t state[REELS] = {0, 0, 0};
    uint8_t spinnedState[REELS];
    uint32_t lastWin = 0;
    uint32_t bet = 1;

   public:
    automat(LiquidCrystal& lcd, uint8_t* wheel) : lcd(lcd), wheel(wheel) {}
    void shuffleWheel(void) {  // call after srandom
        for (uint8_t i = WHEEL_LEN - 1; i > 0; --i) {
            uint8_t j = random(i + 1);
            uint8_t temp = wheel[i];
            wheel[i] = wheel[j];
            wheel[j] = temp;
        }
    }
    void play(void) {
        showMat();
        spin();
        Serial.println("Spin");
        Serial.println(String(spinnedState[0]) + " " + String(spinnedState[1]) + " " + String(spinnedState[2]));
        Serial.println(String(wheel[spinnedState[0]]) + " " + String(wheel[spinnedState[1]]) + " " + String(wheel[spinnedState[2]]));
        Serial.println("Win: " + String(lastWin));
        showSpin();
        showWin();
        while (true) {  // wait for button
            buttons button = getButton();
            if (button != buttons::NO_BUTTON) {
                break;
            }
        }
    }
    void selectBet(void) {
        buttons button;
        uint32_t order = 1;  // change, change no only power
        bool select = false;
        bool pushed = true;  // pushed from previous run
        // fast select
        button = getButton();
        if (button == buttons::SELECT) {
            return;
        }
        showSelect();
        while (true) {
            showBet();
            showOrder(order);
            button = getButton();
            // debounce
            if (button != buttons::NO_BUTTON && !pushed) {
                pushed = true;
            } else if (button != buttons::NO_BUTTON && pushed) {
                continue;
            } else if (button == buttons::NO_BUTTON && pushed) {
                pushed = false;
            }
            switch (button) {
                case buttons::UP:
                    bet += order;
                    clearBet();
                    break;
                case buttons::DOWN:
                    if (bet >= order) {
                        bet -= order;
                        clearBet();
                    }
                    break;
                case buttons::LEFT:
                    order *= 10;
                    clearOrder();
                    break;
                case buttons::RIGHT:
                    if (order > 1) {
                        order /= 10;
                        clearOrder();
                    }
                    break;
                case buttons::SELECT:
                    select = true;
                    break;
                default:
                    break;
            }
            if (select) {
                break;
            }
        }
    }
};

uint8_t wheel[] = {fruits::TWO, fruits::CHERRY, fruits::CHERRY, fruits::CHERRY, fruits::APPLE, fruits::APPLE, fruits::PEAR, fruits::PEAR, fruits::PEAR, fruits::BANANA, fruits::BANANA, fruits::TREE, fruits::TREE, fruits::DINO, fruits::DINO, fruits::HEAD};
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
automat mat(lcd, wheel);

void customCharacterLoad(LiquidCrystal& lcd, const char* data, byte addr) {
    uint8_t buffer[8];
    for (uint8_t i = 0; i < 8; i++) {
        buffer[i] = pgm_read_byte(data + i);
    }
    lcd.createChar(addr, buffer);
}

void setup() {
    pinMode(RANDOM_PIN, INPUT);
    srandom(analogRead(RANDOM_PIN));
    pinMode(BUTTONS, INPUT);
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    lcd.begin(16, 2);
    // custom characters
    for (uint8_t i = 0; i < 8; i++) {
        const char* fruit = (const char*)pgm_read_word(&(fruitsChars[i]));
        customCharacterLoad(lcd, fruit, i);
    }
    // mat init
    mat.shuffleWheel();

    Serial.begin(9600);
    Serial.println("Start");
}

void loop() {
    mat.selectBet();
    mat.play();
}
