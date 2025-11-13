clc;
clear;

fsw = 200e3;        % Frecuencia de Conmutación/Muestreo (Hz)
Ts = 1 / fsw;       % Periodo de Muestreo (s)
b_coef = [0.005, -0.009, 0.004];   % [b2, b1, b0] - Numerador del PID Discreto
a_coef = [1, -1.99, 0.99];         % [a2, a1, a0] - Denominador del PID Discreto (Debe empezar con 1)
s = tf('s');
Wn = 100;
Zeta = 0.5;
%% Caracteristicas de la planta identificada.
num = [a b c];
den = [d e f];
Gc_z = tf(b_coef, a_coef, Ts);     %% Controlador D-PID

% Lazo Abierto
Gol_z = Gc_z * Gp_z;

% Lazo Cerrado
Gcl_z = feedback(Gol_z, 1);

% Graficas:

figure;
step(Gcl_z);
grid on;
title(['Respuesta al Escalón del Lazo Cerrado Discreto (Ts = ' num2str(Ts*1e6) ' us)']);
xlabel('Tiempo (segundos)');
ylabel('Salida');
