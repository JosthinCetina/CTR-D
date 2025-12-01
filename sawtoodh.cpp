// Pines de la ESP32
const int PULSE_INPUT_PIN = 4;   // Pin de entrada del tren de pulsos (Señal amarilla)
const int SAWTOOTH_DAC_PIN = 25; // Pin de salida del DAC (GPIO25 o GPIO26)

// Parámetros de la Rampa Fija
const int DAC_MAX_VALUE = 255;      // Valor máximo del DAC (8 bits)
const int DAC_INCREMENT_STEP = 1;   // Paso fijo de 1 unidad digital

// Frecuencia de Actualización del DAC (Controla la velocidad de subida/pendiente)
// CÁLCULO: (1 / 30600 segundos) * 1,000,000 microsegundos/segundo ≈ 32.68 µs
// Usamos 33 µs para la implementación con enteros, asegurando la pendiente deseada.
const unsigned int DAC_UPDATE_INTERVAL_US = 33; 

// Variables Globales Volátiles
volatile int current_sawtooth_value = 0; 

// ----------------------------------------------------
// RUTINA DE SERVICIO DE INTERRUPCIÓN (ISR)
// Sincronización: Reinicio a 0V en cada flanco.
// ----------------------------------------------------
void IRAM_ATTR pulse_ISR() {
    // 1. Reiniciar el valor digital de la sierra a 0.
    // Esto es vital para la sincronización con cada flanco.
    current_sawtooth_value = 0; 
    
    // 2. Escribir 0V inmediatamente al DAC.
    dacWrite(SAWTOOTH_DAC_PIN, 0); 
}

// ----------------------------------------------------
void setup() {
    Serial.begin(115200);
    
    // Configurar el Pin de Entrada y la Interrupción para detectar CAMBIOS (Subida o Bajada).
    pinMode(PULSE_INPUT_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PULSE_INPUT_PIN), pulse_ISR, CHANGE); 

    // Configurar el DAC
    dacWrite(SAWTOOTH_DAC_PIN, 0); 
    
    Serial.println("Generador de Diente de Sierra (Pendiente 30600/s) Iniciado.");
}

// ----------------------------------------------------
// Bucle Principal (loop)
// Generación: Incremento constante de la pendiente.
// ----------------------------------------------------
void loop() {
    static unsigned long last_dac_update_us = 0;
    unsigned long current_time_us = micros();
    
    // Solo actualizar si ha pasado el intervalo de tiempo calculado para la pendiente.
    if (current_time_us - last_dac_update_us >= DAC_UPDATE_INTERVAL_US) {
        
        // 1. Incrementar el valor de la sierra (Pendiente constante)
        current_sawtooth_value += DAC_INCREMENT_STEP;
        
        // 2. Limitar el valor máximo a 255.
        if (current_sawtooth_value >= DAC_MAX_VALUE) {
            current_sawtooth_value = DAC_MAX_VALUE;
        }

        // 3. Escribir el valor al DAC
        // La rampa se reinicia a 0 en la ISR cuando se detecta un flanco.
        dacWrite(SAWTOOTH_DAC_PIN, current_sawtooth_value);
        
        last_dac_update_us = current_time_us;
    }
}
