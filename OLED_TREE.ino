#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int frameDelay = 350;

// ---------- ОГОНЬКИ ----------
struct Light {
  uint8_t x, y, r;
};

struct Snow {
  uint8_t x;
  int8_t y;
  uint8_t speed;
};

Light lights[] = {
  //{ 64, 10, 3 },
  { 59, 16, 2 },
  { 69, 16, 2 },

  { 64, 16, 2 },
  { 54, 22, 2 },
  { 74, 22, 2 },

  { 64, 22, 2 },
  { 49, 28, 2 },
  { 79, 28, 2 },

  { 64, 28, 2 },
  { 44, 34, 2 },
  { 84, 34, 2 },

  { 64, 34, 2 },
  { 39, 40, 2 },
  { 89, 40, 2 },

  { 64, 40, 2 },
  { 34, 46, 2 },
  { 94, 46, 2 }
};

const int lightCount = sizeof(lights) / sizeof(lights[0]);

const int snowCount = 20;
Snow snow[snowCount];

// ---------- ЁЛКА ----------
void drawTreeVector(int cx, int top) {
  int levels = 6;
  int h = 6;

  for (int i = 0; i < levels; i++) {
    int w = 10 + i * 10;  // <<< ШИРИНА ЁЛКИ
    int y = top + i * h;

    display.fillTriangle(
      cx, y,
      cx - w / 2, y + h,
      cx + w / 2, y + h,
      WHITE);
  }

  // ствол
  display.fillRect(cx - 3, top + levels * h, 6, 8, WHITE);
}

// ---------- ЗВЕЗДА ----------
void drawStar() {
  int y = 6;
  display.drawLine(64, y - 3, 64, y + 3, WHITE);
  display.drawLine(61, y, 67, y, WHITE);

  // Диагональ: из верхнего левого в нижний правый угол
  display.drawLine(62, y - 2, 66, y + 2, WHITE);
  // Диагональ: из нижнего левого в верхний правый угол
  display.drawLine(62, y + 2, 66, y - 2, WHITE);
}

// ---------- ОГОНЬКИ ----------
void drawLights(bool on) {
  for (int i = 0; i < lightCount; i++) {
    bool isOn = ((i + on) % 2 == 0);

    if (isOn) {
      display.fillCircle(
        lights[i].x,
        lights[i].y,
        lights[i].r,
        WHITE
      );
    }
  }
}

void drawSnow() {
  for (int i = 0; i < snowCount; i++) {
    display.drawPixel(snow[i].x, snow[i].y, WHITE);

    snow[i].y += snow[i].speed;

    if (snow[i].y >= SCREEN_HEIGHT) {
      snow[i].y = 0;
      snow[i].x = random(0, SCREEN_WIDTH);
      snow[i].speed = random(1, 3);
    }
  }
}

void drawGround() {
  display.fillRect(0, 54, 128, 10, WHITE);
}

void drawSNowMan() {
  display.fillRect(13, 46, 9, 9, WHITE);
  display.fillRect(14, 41, 7, 7, WHITE);
  display.fillRect(15, 36, 5, 5, WHITE);

  display.drawLine(10, 41, 24, 41, WHITE);
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  randomSeed(analogRead(0));

  for (int i = 0; i < snowCount; i++) {
    snow[i].x = random(0, SCREEN_WIDTH);
    snow[i].y = random(0, SCREEN_HEIGHT);
    snow[i].speed = random(1, 10);  // скорость падения
  }
}

void loop() {
  static bool state = false;

  display.clearDisplay();

  drawTreeVector(64, 10);
  drawStar();
  drawGround();
  drawSNowMan();
  drawLights(state);
  display.display();
  drawSnow();

  display.display();

  state = !state;
  delay(frameDelay);
}
