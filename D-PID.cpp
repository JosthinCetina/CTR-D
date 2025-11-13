const int PIN_PV_INPUT = A0;    // Pin Analógico para leer la Variable de Proceso (PV)
const int PIN_PWM_OUTPUT = 9;   // Pin PWM para enviar la señal de control u(k) (Debe ser un pin PWM ~)
const float SP_REFERENCE_RAW = 512.0; // Valor de referencia interno (ej. la mitad del rango del ADC)
const unsigned long TS_MS = 5; // Periodo de Muestreo en milisegundos (ej. 5 ms -> fs = 200 Hz)
const float Kp_prime = 0.5;   // Coeficiente Proporcional
const float Ki_prime = 0.5;   // Coeficiente Integral (ya incluye la multiplicación por Ts)
const float Kd_prime = 0.2;   // Coeficiente Derivativo (ya incluye la división por Ts)
const int PWM_MIN = 0;      // Mínimo ciclo de trabajo PWM (0%)
const int PWM_MAX = 255;    // Máximo ciclo de trabajo PWM (100% en Arduino de 8 bits)
float e_k = 0.0;     // Error actual: e(k)
float e_k_1 = 0.0;   // Error anterior: e(k-1)
float e_k_2 = 0.0;   // Error dos veces anterior: e(k-2)
float u_k_1 = 0.0;   // Salida de control anterior: u(k-1)
unsigned long tiempo_anterior = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(PIN_PWM_OUTPUT, OUTPUT);
  Serial.println("--- D-PID Incremental Iniciado ---");
  Serial.print("Referencia (SP): "); Serial.println(SP_REFERENCE_RAW);
  Serial.print("Kp': "); Serial.print(Kp_prime);
  Serial.print(", Ki': "); Serial.print(Ki_prime);
  Serial.print(", Kd': "); Serial.println(Kd_prime);
  analogWrite(PIN_PWM_OUTPUT, PWM_MIN);
}

void loop() {
  unsigned long tiempo_actual = millis();
  if (tiempo_actual - tiempo_anterior >= TS_MS) {
    tiempo_anterior = tiempo_actual; // Actualiza el tiempo para el siguiente ciclo
    float PV_k = (float)analogRead(PIN_PV_INPUT);
    e_k = SP_REFERENCE_RAW - PV_k;
    float Delta_P = Kp_prime * (e_k - e_k_1);
    float Delta_I = Ki_prime * e_k;
    float Delta_D = Kd_prime * (e_k - 2 * e_k_1 + e_k_2);
    float Delta_u = Delta_P + Delta_I + Delta_D;
    float u_k = u_k_1 + Delta_u;
    if (u_k > PWM_MAX) {
      u_k = PWM_MAX;
    } else if (u_k < PWM_MIN) {
      u_k = PWM_MIN;
    }
    analogWrite(PIN_PWM_OUTPUT, (int)u_k);
    e_k_2 = e_k_1;
    e_k_1 = e_k;
    u_k_1 = u_k;

    // MONITOREO (Descomentar para ver los valores)
    /*
    Serial.print("PV: "); Serial.print(PV_k);
    Serial.print("\te(k): "); Serial.print(e_k);
    Serial.print("\tDelta_u: "); Serial.print(Delta_u);
    Serial.print("\tu(k): "); Serial.println(u_k);
    */
  } 
