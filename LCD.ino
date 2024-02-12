#include <LiquidCrystal.h>

#define WHEEL_LEN 8
#define REELS 3

#define LCD_BACKLIGHT 10
#define BUTTONS A0

enum fruits {
    DINO,
    TREE,
    HEAD,
    TWO,
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
const char* const fruitsChars[8] PROGMEM = {dino, tree, head, two, cherry, apple, pear, banana};

class automat {
   private:
    uint32_t price(void) {
        if (spinnedState[0] == spinnedState[1] && spinnedState[1] == spinnedState[2]) {
            return 20;
        } else if (spinnedState[0] == spinnedState[1] || spinnedState[1] == spinnedState[2] || spinnedState[0] == spinnedState[2]) {
            return 2;
        } else {
            return 0;
        }
    }

    LiquidCrystal& lcd;
    const uint8_t* wheel;
    uint8_t state[REELS];
    uint8_t spinnedState[REELS];
    uint32_t lastWin;
    uint32_t bet;

   public:
    automat(LiquidCrystal& lcd, const uint8_t* wheel) : lcd(lcd), wheel(wheel) {}
    void setBet(uint32_t bet) {
        this->bet = bet;
    }
    uint32_t spin(void) {
        for (size_t i = 0; i < REELS; i++) {
            spinnedState[i] = random(WHEEL_LEN);
        }
        lastWin = price() * bet;
        return lastWin;
    }
    void showState(void) {
        lcd.setCursor(13, 0);
        for (size_t i = 0; i < REELS; i++) {
            lcd.write(wheel[state[i]]);
        }
    }
    bool finalState() {
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
            if (finalState()) {
                break;
            }
            for (size_t i = 0; i < REELS; i++) {
                if (state[i] != spinnedState[i]) {
                    state[i]++;
                    state[i] %= WHEEL_LEN;
                }
            }
            showState();
            delay(200);
        }
    }
    void showWin(void) {
        lcd.setCursor(0, 1);
        lcd.print("Win: ");
        lcd.print(lastWin);
    }
    void clearWin(void) {
        lcd.setCursor(0, 1);
        lcd.print("Win: ");
        lcd.print("           ");
    }
    void play(void) {
        clearWin();
        showState();
        spin();
        showSpin();
        showWin();
    }
};

uint8_t wheel[] = {fruits::DINO, fruits::TREE, fruits::HEAD, fruits::TWO, fruits::CHERRY, fruits::APPLE, fruits::PEAR, fruits::BANANA, fruits::DINO, fruits::TREE};
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
    pinMode(A5, INPUT);
    srandom(analogRead(A5));
    pinMode(BUTTONS, INPUT);
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    lcd.begin(16, 2);
    // custom characters
    for (uint8_t i = 0; i < 8; i++) {
        const char* fruit = (const char*)pgm_read_word(&(fruitsChars[i]));
        customCharacterLoad(lcd, fruit, i);
    }
    lcd.home();
    lcd.print("Bet: 10");
    mat.setBet(10);
    mat.showState();
}

void loop() {
    if (analogRead(BUTTONS) < 1000) {
        mat.play();
    }
}
