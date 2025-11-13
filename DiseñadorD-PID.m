%% Todo este código esta basado en la guía "Apply digital Tech PWM".

clc;
clear;

Kp_cont = 0.5;          % Ganancia Proporcional Continua
Ki_cont = 500;          % Ganancia Integral Continua
Kd_cont = 0.00005;      % Ganancia Derivativa Continua
fsw = 200e3;            % Frecuencia de Conmutación/Muestreo (ej. 200 kHz)
Ts = 1 / fsw;           % Periodo de Muestreo (segundos)
t_comp = 1.0 * 1e-6;    % Retraso Computacional (ej. 1.0 us)
Tt = Ts + t_comp;       % Retraso Total Aproximado 

%% DISCRETIZACIÓN Y FUNCIÓN DE TRANSFERENCIA Gc(z) mediante metodo de Backward Difference:
s = tf('s');
z = tf('z', Ts);
Gc_s = Kp_cont + Ki_cont/s + Kd_cont*s;

%% Aplicación del metodo Backward Difference:
Gc_z_backward = Kp_cont + (Ki_cont * Ts * z) / (z - 1) + (Kd_cont * (z - 1)) / (z * Ts);

%% COEFICIENTES PARA LA FORMA DE POSICIÓN (ESTÁNDAR)

[num_z, den_z] = tfdata(minreal(Gc_z_backward), 'v');
a0_norm = den_z(1); 
den_norm = den_z / a0_norm;
num_norm = num_z / a0_norm;
b_coef = num_norm;
a_coef_feedback = den_norm(2:end); % Los términos de realimentación (a1, a2, ...)

disp(a_coef_feedback);

%% Ganancias discretizadas:
Kp_prime = Kp_cont;
Ki_prime = Ki_cont * Ts; % Ganancia integral se multiplica por Ts
Kd_prime = Kd_cont / Ts; % Ganancia derivativa se divide por Ts
