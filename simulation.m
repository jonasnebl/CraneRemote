%Zeiten
dt=1e-5;
dtstep=0.01;
tges=90;  

%Positionen
dxstep=0.015;
tol=5*dxstep;
X=zeros(round(tges/dtstep),1);
Xmesswert=zeros(round(tges/dtstep),1);
Xlist=zeros(round(tges/dtstep),1); %Istposition der Laufkatze
Xg=[zeros(20/dtstep,1);(0:30/(20/dtstep):30)';30+zeros(50/dtstep,1)]; %Sollposition der Laufkatze
Xl=zeros(round(tges/dtstep),1);%Position der Hakenflasche im KOSY der Laufkatze
Vl=zeros(round(tges/dtstep),1);%Geschwindigkeit der Hakenflasche im KOSY der Laufkatze

%globales Koordinatensystem

V=zeros(round(tges/dtstep),1);
A=zeros(round(tges/dtstep),1);
L=10;
vs=0;
i=1;
Amesswert=zeros(round(tges/dtstep),1);

Vmesswert=zeros(round(tges/dtstep),1);

w=waitbar(0,'0%');
s0=round(dtstep/dt); %Semaphore
s=0;
s2=0;
korrektur=0;

for k=1:1:tges/dt
%Simulation

    alpha=(X(i)-(Xg(round((k*dt)/dtstep)+1)+Xlist(i)))/L;
    if s==s0
        A(i+1)=9.81*alpha;
        V(i+1)=V(i)+A(i+1)*dt;
        X(i+1)=X(i)-0.5*A(i+1)*dt^2-V(i+1)*dt;
    else
        A(i)=9.81*alpha;   
        V(i)=V(i)+A(i)*dt;
        X(i)=X(i)-0.5*A(i)*dt^2-V(i)*dt;
    end

%Pendeldämpfungssoftware
    if s==s0 %Pendelsoftware wird nur in Intervallen von dtstep aktiviert
       Amesswert(i+1)=floor(A(i+1)*15+0.5)/15;
       %doppelte numerische Integration
       Vmesswert(i+1)=Vmesswert(i)+0.5*(Amesswert(i+1)+Amesswert(i))*dtstep;
       Xmesswert(i+1)=Xmesswert(i)-0.5*(Vmesswert(i+1)+Vmesswert(i))*dtstep;
       %Korrektur des Nullniveaus
       if Amesswert(i+1)*Amesswert(i)<=0
           Xmesswert(i+1)=Xg(i)+Xlist(i);
       end

       Xl(i+1)=Xmesswert(i+1)-Xg(i+1); %Hilfskoordinatensystem der Laufkatze
       Vl(i+1)=Vmesswert(i+1)+(Xg(i+1)-Xg(i))/dtstep; %Hilfsgeschwindigkeit der Laufkatze
       
            %Korrektur in negative Richtung
            if Vl(i+1)*Vl(i)<=0 && Xl(i+1)-Xlist(i)>tol
                korrektur=1;    
            end       
            if abs(Xlist(i)) > abs(Xlist(i)-Xl(i)) && korrektur==1
                 korrektur=0;
            end
            
            %Korrektur in positive Richtung
            if Vl(i+1)*Vl(i)<=0 && Xl(i+1)-Xlist(i)<tol
                korrektur=-1;    
            end       
            if abs(Xlist(i)) > abs(Xlist(i)-Xl(i)) && korrektur==-1
                 korrektur=0;
            end
            
            %Ausführen der Korrektur
            if korrektur==1
                Xlist(i+1)=Xlist(i)+dxstep;
            elseif korrektur==-1
                Xlist(i+1)=Xlist(i)-dxstep;
            else
                Xlist(i+1)=Xlist(i);
            end
                        
       s=0;
       i=i+1;
    else    
       s=s+1;
    
    end
            
    

%Waitbar
    if i*dt==round(i*dt)
        waitbar((i*dt)/tges,w,append(string(round((i*dt)/tges,2)*100),'%'));
    end

end

close(w);
hold off
% plot(A)
% hold on
% plot(V)
plot(Xl)
hold on
% % plot(V)
% % plot(Amesswert)
% % plot(Vmesswert)
% plot(Xmesswert)
% plot(Xg)
% % plot(Xl);
plot(Vl)
plot(Xlist)
plot(X)