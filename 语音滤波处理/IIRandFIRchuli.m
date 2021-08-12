clear;
[z1s,fs]=audioread('D:\xia.wav');     
sound(z1s,fs);
N=size(z1s);                      
for i=1:(N)/5
    z2(i)=z1s(5*i);
end                             
   pause(4);
    sound(z2,fs/5);            
   %**********IIR滤波器设计***************%
   fc=1000;fr=1200;fs1=fs/5;rs=40;ap=1;
   Wc=2*fs1*tan(2*pi*fc/(2*fs1));    
   Wr=2*fs1*tan(2*pi*fr/(2*fs1));    
   [N1,Wn]=buttord(Wc,Wr,ap,rs,'s');
   [B,A]=butter(N1,Wn,'s');
   [B2,A2]=bilinear(B,A,fs1);
   [h,w]=freqz(B2,A2);
   %**********IIR滤波器设计***************%
   %**********FIR滤波器设计***************%
   fc1=1000;fr1=1200;fs1=fs/5;
   M=145;
   f1=fir1(M-1,(fc1*2)/fs1,'low',hanning(M));  
   F1=fft(f1);
   [h1,w1]=freqz(f1,1);
   %**********FIR滤波器设计***************%
   %********序列截断处理及运算************%
   x1=filter(f1,1,z2);        %音频序列通过FIR低通滤波器处理
   %sound(x1,fs/5);
   %pause(5);
   x2=filter(B2,A2,z2);        %音频序列通过IIR低通滤波器处理
   %sound(x2,fs1);
   n2=7000:15191;
   y1=z2(7000:15191);         %对取样序列进行截断
   y2=x1(7000:15191);          %对处理序列进行截断
   y3=x2(7000:15191);          
   Y1=fft(y1);
   Y2=fft(y2);                %进行fft计算
   Y3=fft(y3);
   
   subplot(3,2,1);
   plot(n2,y1);
   title('原始抽样信号的时域波形');
   xlabel('时间');
   ylabel('幅度');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,2);
   plot(abs(Y1));
   title('原始抽样信号的频域波形');
   xlabel('频域');
   ylabel('幅度');
   axis([0 8000 0 150]);
   grid;
   subplot(3,2,3);
   plot(n2,y2);
   title('FIR滤波后抽样信号的时域波形');
   xlabel('时间');
   ylabel('幅度');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,4);
   plot(abs(Y2));
   title('FIR滤波后抽样信号的频域波形');
   xlabel('频域');
   ylabel('幅度');
   axis([0 8000 0 150]);
   grid;
   subplot(3,2,5);
   plot(n2,y3);
   title('IIR滤波后抽样信号的时域波形');
   xlabel('时间');
   ylabel('幅度');
   axis([7000 15200 -0.5 0.5]);
   grid;
   subplot(3,2,6);
   plot(abs(Y3));
   title('IIR滤波后抽样信号的频域波形');
   xlabel('频域');
   ylabel('幅度');
   axis([0 8000 0 150]);
   grid;
