#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>

#define PIN 2 // пин, на котором висит матрица

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +    // ВЕРХ + ЛЕВО
                            NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,   // СТРОКИ, соединены ЗИГЗАГОМ
                            NEO_GRB            + NEO_KHZ800);         // порядок цветов, частота шины

// массив цветов: красный, зелёный, синий
const uint16_t colors[] =
{
  matrix.Color(255, 0, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 0, 255)
};

int x    = matrix.width();
int y    = matrix.height();
int pass = 0;

const char text[] = "HELLO FROM GOTCHAU";
int16_t text_width;

void setup()
{
  matrix.begin();                 // инициализация матрицы
  matrix.setBrightness(125);      // яркость
  matrix.setTextWrap(false);      // запрещаем перенос строки
  matrix.setFont(&TomThumb);      // устанавливаем компактный шрифт
  matrix.setTextColor(colors[0]); // задаём цвет текста

  // получаем размер выводимого текста в пикселях
  int16_t  d1;
  uint16_t d2;
  matrix.getTextBounds(text, 0, 0, &d1, &d1, &text_width, &d2); 

void loop()
{
  matrix.fillScreen(0);     // очистка экрана
  matrix.setCursor(x, y);   // ставим курсор
  matrix.print(text);       // отрисовка текста в буффер

  // код ниже отвечает за скролл текста
  if (--x < -text_width)
  {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();          // вывод буффера
  delay(100);             // задержка
}
