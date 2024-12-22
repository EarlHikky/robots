#define TWO_SEC 2000
#define FIVE_SEC 5000
#define MIN_SPEED 0
#define MAX_SPEED 255

const int motorPin1 = 2; // IN1 драйвера
const int motorPin2 = 3; // IN2 драйвера
const int enablePin = 5; // ENA драйвера (ШИМ)

void acceleration(unsigned char direction, unsigned char range);
void smooth_acceleration(int target_speed);
void smooth_stop(int current_speed);
void hard_stop(void);
void move_forward(unsigned char speed);
void move_backward(unsigned char speed);

/**
 * @brief Мягкое ускорение/торможение до range
 * @param positive_direction 1 - ускорение, 0 - торможение
 * @param range Скорость, до которой происходит ускорение/торможение
 */
void acceleration(unsigned char direction, unsigned char range) {
  if (!direction) { // TODO возможность реверивного хода, дублирование кода
    for (unsigned char i = range; i > MIN_SPEED; i--) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      analogWrite(enablePin, i);
      delay(50);
    }
  } else {
    for (unsigned char i = MIN_SPEED; i < range; i++) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      analogWrite(enablePin, i);
      delay(50);
    }
  }
}

/**
 * @brief Мягкое торможение мотора
 *
 * Мотор тормозится с помощью ускорения в обратном направлении
 * до остановки.
 *
 * @param current_speed Текущая скорость мотора
 */
void smooth_stop(int current_speed) {
  if (current_speed >= MIN_SPEED)
    acceleration(0, current_speed);
}

/**
 * @brief Мягкое ускорение мотора
 *
 * Мотор ускоряется до target_speed с помощью увеличения ШИМ
 * сигнала на драйвере.
 *
 * @param target_speed Скорость, до которой происходит ускорение
 */
void smooth_acceleration(int target_speed) {
  if (target_speed <= MAX_SPEED)
    acceleration(1, target_speed);
}

/**
 * @brief Остановка мотора с полной отсечкой питания
 *
 * Мотор останавливается с помощью полной отсечки питания, что
 * может привести к проскальзыванию мотора.
 */
void hard_stop(void) {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, MIN_SPEED);
}

void move_forward(unsigned char speed) {
  if (speed <= MAX_SPEED) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, speed);
  }
}

void move_backward(unsigned char speed) {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, speed);
}

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Остановить мотор при запуске
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, MIN_SPEED);
}

void loop() {

  move_forward(200);
  hard_stop();
  delay(TWO_SEC);

  move_backward(150);
  smooth_stop(150);
  delay(TWO_SEC);

  smooth_acceleration(255);
}
