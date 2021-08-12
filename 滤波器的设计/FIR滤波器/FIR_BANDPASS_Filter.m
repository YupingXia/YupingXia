%**********FIR�˲������***************%
   fc1=1000;fr1=1500;fs1=8820;         %���ָ��
   M=145;                              %FIR�˲�������
   f1=fir1(M-1,[(fc1*2)/fs1 (fr1*2)/fs1],'bandpass',hanning(M));  
   F1=fft(f1);
   [h1,w1]=freqz(f1,1);
   n1=0:M-1;
   subplot(2,2,1);
   stem(n1,f1);
   axis([0 150 -0.1 0.3]);
   grid;
   xlabel('n1');
   ylabel('f1');
   title('FIR��ͨ�˲���ʱ��ͼ');
   subplot(2,2,2);
   plot((fs1/2)*w1/pi,20*log10(abs(h1)));
   axis([0 fs1/2 -120 10]);
   grid;
   xlabel('Ƶ��');
   ylabel('����/dB');
   title('FIR��ͨ�˲���Ƶ��ͼ');
   subplot(2,2,3);
   plot(w1/pi,angle(h1));
   xlabel('Ƶ��\omega');
   ylabel('\phi(\omega)');
   title('FIR��ͨ�˲�����λͼ');
   %**********FIR�˲������***************%