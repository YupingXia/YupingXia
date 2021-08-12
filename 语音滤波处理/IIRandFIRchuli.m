clear;
[z1s,fs]=audioread('D:\xia.wav');     
sound(z1s,fs);
N=size(z1s);                      
for i=1:(N)/5
    z2(i)=z1s(5*i);
end                             
   pause(4);
    sound(z2,fs/5);            
   %**********IIR�˲������***************%
   fc=1000;fr=1200;fs1=fs/5;rs=40;ap=1;
   Wc=2*fs1*tan(2*pi*fc/(2*fs1));    
   Wr=2*fs1*tan(2*pi*fr/(2*fs1));    
   [N1,Wn]=buttord(Wc,Wr,ap,rs,'s');
   [B,A]=butter(N1,Wn,'s');
   [B2,A2]=bilinear(B,A,fs1);
   [h,w]=freqz(B2,A2);
   %**********IIR�˲������***************%
   %**********FIR�˲������***************%
   fc1=1000;fr1=1200;fs1=fs/5;
   M=145;
   f1=fir1(M-1,(fc1*2)/fs1,'low',hanning(M));  
   F1=fft(f1);
   [h1,w1]=freqz(f1,1);
   %**********FIR�˲������***************%
   %********���нضϴ�������************%
   x1=filter(f1,1,z2);        %��Ƶ����ͨ��FIR��ͨ�˲�������
   %sound(x1,fs/5);
   %pause(5);
   x2=filter(B2,A2,z2);        %��Ƶ����ͨ��IIR��ͨ�˲�������
   %sound(x2,fs1);
   n2=7000:15191;
   y1=z2(7000:15191);         %��ȡ�����н��нض�
   y2=x1(7000:15191);          %�Դ������н��нض�
   y3=x2(7000:15191);          
   Y1=fft(y1);
   Y2=fft(y2);                %����fft����
   Y3=fft(y3);
   
   subplot(3,2,1);
   plot(n2,y1);
   title('ԭʼ�����źŵ�ʱ����');
   xlabel('ʱ��');
   ylabel('����');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,2);
   plot(abs(Y1));
   title('ԭʼ�����źŵ�Ƶ����');
   xlabel('Ƶ��');
   ylabel('����');
   axis([0 8000 0 150]);
   grid;
   subplot(3,2,3);
   plot(n2,y2);
   title('FIR�˲�������źŵ�ʱ����');
   xlabel('ʱ��');
   ylabel('����');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,4);
   plot(abs(Y2));
   title('FIR�˲�������źŵ�Ƶ����');
   xlabel('Ƶ��');
   ylabel('����');
   axis([0 8000 0 150]);
   grid;
   subplot(3,2,5);
   plot(n2,y3);
   title('IIR�˲�������źŵ�ʱ����');
   xlabel('ʱ��');
   ylabel('����');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,6);
   plot(abs(Y3));
   title('IIR�˲�������źŵ�Ƶ����');
   xlabel('Ƶ��');
   ylabel('����');
   axis([0 8000 0 150]);
   grid;
