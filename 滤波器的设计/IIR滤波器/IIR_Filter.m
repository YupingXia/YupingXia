clear;
[z1s,fs]=audioread('D:\xia.wav');     %��ȡ��Ƶ�ļ�
N=size(z1s);                        %������Ƶ���г���
for i=1:(N)/5
    z2(i)=z1s(5*i);
end                                 %�����н���ȡ��
   %sound(z2,fs/5);
    n=1:N/5;
    %noise1=randn((N)/5,1);
    noise2=0.1*sin(2*pi*2000*n/fs);
    z3=z2+noise2;                   %��������
   % pause(5);
   %sound(z3,fs/5);
   % pause(5);
   %**********�˲������***************%
   fc1=1000;fr1=1200;fs1=fs/5;rs=40;ap=1; %��ͨ
   Wc1=2*fs1*tan(2*pi*fc1/(2*fs1));       %��ͨͨ����ֹƵ��
   Wr1=2*fs1*tan(2*pi*fr1/(2*fs1));       %��ͨ�����ֹƵ��
   [N1,Wn1]=buttord(Wc1,Wr1,ap,rs,'s');
   [B1,A1]=butter(N1,Wn1,'s');
   [num1,den1]=bilinear(B1,A1,fs1);
   [h1,w1]=freqz(num1,den1);
   f1=w1/(pi*2)*fs1;
   subplot(2,2,1);
   plot(f1,(abs(h1)));
   title('�˲���Ƶ����Ӧ');
   xlabel('Ƶ��f');
   ylabel('����/dB');
   axis([0 4000 0 2]);
   grid;
   
   fc2=1000;fr2=800;
   Wc2=2*fs1*tan(2*pi*fc2/(2*fs1));  %��ͨͨ����ֹƵ��
   Wr2=2*fs1*tan(2*pi*fr2/(2*fs1));  %��ͨͨ����ֹƵ��
   [N2,Wn2]=buttord(Wc2,Wr2,ap,rs,'s');
   [B2,A2]=butter(N2,Wn2,'high','s');
   [num2,den2]=bilinear(B2,A2,fs1);
   [h2,w2]=freqz(num2,den2);
   f2=w2/(pi*2)*fs1;
   subplot(2,2,2);
   plot(f2,(abs(h2)));
   title('�˲���Ƶ����Ӧ');
   xlabel('Ƶ��f');
   ylabel('����/dB');
   axis([0 4000 0 2]);
   grid;
 
   f1=1800;f2=2200;fr2=2400;
   W1=2*fs1*tan(2*pi*f1/(2*fs1));  %��ͨͨ����ֹƵ��
   W2=2*fs1*tan(2*pi*f2/(2*fs1));  %��ͨͨ����ֹƵ��
   Wr3=2*fs1*tan(2*pi*fr2/(2*fs1));%��ͨ�����ֹƵ��
   [N3,Wn3]=buttord([W1,W2],[1,Wr3],ap,rs,'s');
   [B3,A3]=butter(N3,Wn3,'s');
   [num3,den3]=bilinear(B3,A3,fs1);
   [h3,w3]=freqz(num3,den3);
   f3=w3/(pi*2)*fs1;
   subplot(2,2,3);
   plot(f3,(abs(h3)));
   title('�˲���Ƶ����Ӧ');
   xlabel('Ƶ��f');
   ylabel('����/dB');
   axis([0 4000 0 2]);
   grid;   
   f3=1800;f4=2200;f5=1900;f6=2100;
   W3=2*fs1*tan(2*pi*f3/(2*fs1));  %����ͨ����ֹƵ��
   W4=2*fs1*tan(2*pi*f4/(2*fs1));  %����ͨ����ֹƵ��
   W5=2*fs1*tan(2*pi*f5/(2*fs1));
   W6=2*fs1*tan(2*pi*f6/(2*fs1));
   [N4,Wn4]=buttord([W3 W4],[W5 W6],ap,rs,'s');
   [B4,A4]=butter(N4,Wn4,'stop','s');
   [num4,den4]=bilinear(B4,A4,fs1);
   [h4,w4]=freqz(num4,den4);
   f4=w4/(pi*2)*fs1;
   subplot(2,2,4);
   plot(f4,(abs(h4)));
   title('�˲���Ƶ����Ӧ');
   xlabel('Ƶ��f');
   ylabel('����/dB');
   axis([0 4000 0 2]);
   grid;
   %**********�˲������***************%

   
