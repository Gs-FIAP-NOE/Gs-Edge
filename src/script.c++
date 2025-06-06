#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 9
#define ECHO_PIN 8
#define RED_LED 7
#define YELLOW_LED 6
#define GREEN_LED 5
#define BUZZER 4

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD

void setup()
{
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sistema NOE");
    lcd.setCursor(0, 1);
    lcd.print("Monitorando...");
    delay(2000);
    lcd.clear();
}

void loop()
{
    // Medição com sensor ultrassônico
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distancia = duration * 0.034 / 2; // em cm
    float metros = distancia * 0.01;

    Serial.print("Distancia: ");
    Serial.print(metros);
    Serial.println(" m");

    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(metros, 2);
    lcd.print("m     ");

    if (metros <= 1.0)
    {
        // CRÍTICO: água muito próxima da borda
        digitalWrite(RED_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        tone(BUZZER, 2000);
        lcd.setCursor(0, 1);
        lcd.print("Estado: CRITICO  ");
    }
    else if (metros <= 3.0)
    {
        // ALERTA: distância intermediária
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        noTone(BUZZER);
        lcd.setCursor(0, 1);
        lcd.print("Estado: ALERTA   ");
    }
    else
    {
        // NORMAL: água distante da borda
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        noTone(BUZZER);
        lcd.setCursor(0, 1);
        lcd.print("Estado: NORMAL   ");
    }

    delay(2000);
}