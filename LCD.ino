#include <LiquidCrystal.h>

#define WHEEL_LEN 10
#define REELS 3

#define LCD_BACKLIGHT 10
#define BUTTONS A0

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

uint8_t wheel[] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9};
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
automat mat(lcd, wheel);

void setup() {
    pinMode(A5, INPUT);
    srandom(analogRead(A5));
    pinMode(BUTTONS, INPUT);
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    lcd.begin(16, 2);
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
