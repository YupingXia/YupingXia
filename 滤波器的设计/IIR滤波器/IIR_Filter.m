clear;
[z1s,fs]=audioread('D:\xia.wav');     %读取音频文件
N=size(z1s);                        %计算音频序列长度
for i=1:(N)/5
    z2(i)=z1s(5*i);
end                                 %对序列进行取样
   %sound(z2,fs/5);
    n=1:N/5;
    %noise1=randn((N)/5,1);
    noise2=0.1*sin(2*pi*2000*n/fs);
    z3=z2+noise2;                   %噪声产生
   % pause(5);
   %sound(z3,fs/5);
   % pause(5);
   %**********滤波器设计***************%
   fc1=1000;fr1=1200;fs1=fs/5;rs=40;ap=1; %低通
   Wc1=2*fs1*tan(2*pi*fc1/(2*fs1));       %低通通带截止频率
   Wr1=2*fs1*tan(2*pi*fr1/(2*fs1));       %低通阻带截止频率
   [N1,Wn1]=buttord(Wc1,Wr1,ap,rs,'s');
   [B1,A1]=butter(N1,Wn1,'s');
   [num1,den1]=bilinear(B1,A1,fs1);
   [h1,w1]=freqz(num1,den1);
   f1=w1/(pi*2)*fs1;
   subplot(2,2,1);
   plot(f1,(abs(h1)));
   title('滤波器频率响应');
   xlabel('频率f');
   ylabel('幅度/dB');
   axis([0 4000 0 2]);
   grid;
   
   fc2=1000;fr2=800;
   Wc2=2*fs1*tan(2*pi*fc2/(2*fs1));  %高通通带截止频率
   Wr2=2*fs1*tan(2*pi*fr2/(2*fs1));  %高通通带截止频率
   [N2,Wn2]=buttord(Wc2,Wr2,ap,rs,'s');
   [B2,A2]=butter(N2,Wn2,'high','s');
   [num2,den2]=bilinear(B2,A2,fs1);
   [h2,w2]=freqz(num2,den2);
   f2=w2/(pi*2)*fs1;
   subplot(2,2,2);
   plot(f2,(abs(h2)));
   title('滤波器频率响应');
   xlabel('频率f');
   ylabel('幅度/dB');
   axis([0 4000 0 2]);
   grid;
 
   f1=1800;f2=2200;fr2=2400;
   W1=2*fs1*tan(2*pi*f1/(2*fs1));  %带通通带截止频率
   W2=2*fs1*tan(2*pi*f2/(2*fs1));  %带通通带截止频率
   Wr3=2*fs1*tan(2*pi*fr2/(2*fs1));%带通阻带截止频率
   [N3,Wn3]=buttord([W1,W2],[1,Wr3],ap,rs,'s');
   [B3,A3]=butter(N3,Wn3,'s');
   [num3,den3]=bilinear(B3,A3,fs1);
   [h3,w3]=freqz(num3,den3);
   f3=w3/(pi*2)*fs1;
   subplot(2,2,3);
   plot(f3,(abs(h3)));
   title('滤波器频率响应');
   xlabel('频率f');
   ylabel('幅度/dB');
   axis([0 4000 0 2]);
   grid;   
   f3=1800;f4=2200;f5=1900;f6=2100;
   W3=2*fs1*tan(2*pi*f3/(2*fs1));  %带阻通带截止频率
   W4=2*fs1*tan(2*pi*f4/(2*fs1));  %带阻通带截止频率
   W5=2*fs1*tan(2*pi*f5/(2*fs1));
   W6=2*fs1*tan(2*pi*f6/(2*fs1));
   [N4,Wn4]=buttord([W3 W4],[W5 W6],ap,rs,'s');
   [B4,A4]=butter(N4,Wn4,'stop','s');
   [num4,den4]=bilinear(B4,A4,fs1);
   [h4,w4]=freqz(num4,den4);
   f4=w4/(pi*2)*fs1;
   subplot(2,2,4);
   plot(f4,(abs(h4)));
   title('滤波器频率响应');
   xlabel('频率f');
   ylabel('幅度/dB');
   axis([0 4000 0 2]);
   grid;
   %**********滤波器设计***************%

   
