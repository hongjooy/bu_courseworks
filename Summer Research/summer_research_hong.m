%% Summer 2015 Jooyoun Hong

N_SYMS = 10000; 	% Number of Symbols
N_BITS = 50; 		% Number of Bits

CFO_CORR = 1;
N_USER = 2;


NOISE = 'CFO'; % FREE, AWGN, CFO

N_SAMPLE = floor(N_SYMS/N_BITS); % Number of Samples per Bits

carr_freq = 4 * N_BITS;


% Random Bit Generation

tx_data_1 = randi(2, 1, N_BITS) - 1;	
tx_data_2 = randi(2, 1, N_BITS) - 1;
if (N_USER == 4)
    tx_data_3 = randi(2, 1, N_BITS) - 1;    
    tx_data_4 = randi(2, 1, N_BITS) - 1;
end

% Upsampling
perfect_filter = ones(1,N_SAMPLE); % square one
filtered_data_1 = kron(tx_data_1, perfect_filter);
filtered_data_2 = kron(tx_data_2, perfect_filter);
if (N_USER == 4)
   filtered_data_3 = kron(tx_data_3, perfect_filter);
   filtered_data_4 = kron(tx_data_4, perfect_filter); 
end


Nb = N_SAMPLE * N_BITS; 
t = 1:N_SYMS;
bi_sampled_data_1 = zeros(1, N_SYMS); 
bi_sampled_data_1(1:Nb) = filtered_data_1(1:Nb);
bi_sampled_data_2 = zeros(1, N_SYMS); 
bi_sampled_data_2(1:Nb) = filtered_data_2(1:Nb);

if(N_USER == 4)
    bi_sampled_data_3 = zeros(1, N_SYMS); 
    bi_sampled_data_3(1:Nb) = filtered_data_3(1:Nb);
    bi_sampled_data_4 = zeros(1, N_SYMS); 
    bi_sampled_data_4(1:Nb) = filtered_data_4(1:Nb);
end

%% BPSK

modvec_bpsk   =  (1/sqrt(2))  .* [-1 1];
mod_fcn_bpsk  = @(x) complex(modvec_bpsk(1+x),0);


% Map the data values on to complex symbols
% BPSK
sampled_data_1 = arrayfun(mod_fcn_bpsk, bi_sampled_data_1);
sampled_data_2 = arrayfun(mod_fcn_bpsk, bi_sampled_data_2);
if (N_USER == 4)
   sampled_data_3 = arrayfun(mod_fcn_bpsk, bi_sampled_data_3);
   sampled_data_4 = arrayfun(mod_fcn_bpsk, bi_sampled_data_4); 
end


signal_1 = sampled_data_1.*exp(i*2*pi*carr_freq/N_SYMS*t);
signal_2 = sampled_data_2.*exp(i*2*pi*carr_freq/N_SYMS*t);

if (N_USER == 4)
    signal_3 = sampled_data_3.*exp(i*2*pi*carr_freq/N_SYMS*t);
    signal_4 = sampled_data_4.*exp(i*2*pi*carr_freq/N_SYMS*t);
end
%%---------------------------------------- NOISE SELECTION -----------------------------------------------

switch (NOISE)
case 'FREE'
	tx_payload_1 = signal_1;
	tx_payload_2 = signal_2;

    if (N_USER == 4)
        tx_payload_3 = signal_3;
        tx_payload_4 = signal_4;
    end

case 'CFO'
	payload_cfo_1 = 2e-5;
	payload_cfo_2 = 4e-5;
	tx_payload_1 = signal_1 .* exp(i*2*pi*payload_cfo_1*t);  
	tx_payload_2 = signal_2 .* exp(i*2*pi*payload_cfo_2*t);

    if(N_USER == 4)
        payload_cfo_3 = 3e-5;
        payload_cfo_4 = 4e-4;
        tx_payload_3 = signal_3 .* exp(i*2*pi*payload_cfo_3*t);  
        tx_payload_4 = signal_4 .* exp(i*2*pi*payload_cfo_4*t); 
    end

end


%%--------------------------------------------------- RX -------------------------------------------------
rx_signal_original = tx_payload_1 + tx_payload_2;
if (N_USER == 4)
    rx_signal_original = rx_signal_original + tx_payload_3 + tx_payload_4;
end


if (CFO_CORR == 1)
    if(N_USER == 2)
        cfo_ave = mean([payload_cfo_1;payload_cfo_2]);
    elseif (N_USER == 4)
        cfo_ave = mean([payload_cfo_1;payload_cfo_2;payload_cfo_3;payload_cfo_4]);
    end
    
rx_signal_original = rx_signal_original.*exp(-1i*2*pi*cfo_ave*t); 
end
		
rx_signal = rx_signal_original.*exp(-1i*2*pi*carr_freq/N_SYMS*t); % undo signal frequency

%% --------------------------------------------- DOWNSAMPLING -------------------------------------------------
count = 0;
rx_data_1 = zeros(1,N_BITS);

for count = 0:N_BITS-1
	rx_data_1(count + 1) = mean(rx_signal((1+(count*floor(N_SYMS/N_BITS))):(floor(N_SYMS/N_BITS)+(count*floor(N_SYMS/N_BITS)))));
	count = count + 1;
end

%%---------------------------------------------- Decode -------------------------------------------------------

if(N_USER == 2)
    a1 = 1;
    a2 = 1;
elseif(N_USER == 4)
    a1 = 0;
    a2 = 0;
end

demod_fcn_bpsk = @(x) double(mod(round(( real(x)/(sqrt(2)) + (a1+a2)/2)), 2) );
rx_data = arrayfun(demod_fcn_bpsk, rx_data_1);
 
if(N_USER == 2)
    tx_data = bitxor(tx_data_1, tx_data_2);
elseif(N_USER == 4)
    tx_data_xor_1 = bitxor(tx_data_1, tx_data_2);
    tx_data_xor_2 = bitxor(tx_data_3, tx_data_4);
    tx_data = bitxor(tx_data_xor_1, tx_data_xor_2);
end

errorsum = sum (rx_data ~= tx_data)



cf = 0;

if (N_USER == 2)
    cf = cf + 1;
    figure(cf); clf;
    subplot(3,1,1);
    plot(t, real(signal_1), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_1, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(3,1,2);
    plot(t, real(signal_2), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_2, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(3,1,3)
    plot(t, real(rx_signal_original), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, rx_signal, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');
elseif (N_USER == 4)
    cf = cf + 1;
    figure(cf); clf;
    subplot(5,1,1);
    plot(t, real(signal_1), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_1, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(5,1,2);
    plot(t, real(signal_2), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_2, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(5,1,3);
    plot(t, real(signal_3), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_3, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(5,1,4);
    plot(t, real(signal_4), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, sampled_data_4, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');

    subplot(5,1,5)
    plot(t, real(rx_signal_original), 'linewidth', 2, 'color','b');
    hold on;
    plot(t, rx_signal, 'linewidth', 1, 'color','r');
    axis([1,N_SYMS,-2.5,2.5]);
    xlabel('n');
    legend('BPSK','Binary Message');
end
        



 