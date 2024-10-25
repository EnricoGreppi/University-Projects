function [bn] = bordiNeumannParabolica(Ne,pivot, x, y, borders, gn, modalita, deltat,T)
    Ndof = max(pivot);
    Ntempo=T/deltat;
    bn= zeros(Ndof,Ntempo+1);
    if modalita ==1 
        for n=0:Ntempo
            temp=n*deltat*T;
            for e=1:size(Ne)  %indici globali nodi su Neumann
                l=Ne(e);
                ib=borders(l,1); %vertice partenza
                %geom.elements.borders per ogni lato contiene 
                % [v1, v2, T1, T2] ((-1) se tocca dominio)
                %T1 T2 si riferiscono a posizione in geom.elements.triangle??
                ie=borders(l,2);%vertice arrivo
                xb=x(ib); %coordinata x ib
                yb=y(ib);%%coordinata y ib
                xe=x(ie);%coordinata x ie
                ye=y(ie);%coordinata y ib
                iib=pivot(ib);
                iie=pivot(ie);
                mislato=sqrt((xe-xb)^2+(ye-yb)^2); %lunghezza lato
                if iib>0
                    bn(iib,n+1)= bn(iib,n+1)+(2*gn(xb, yb,temp)+gn(xe, ye,temp))*mislato/6;
                end
                if iie>0
                    bn(iie,n+1)= bn(iie,n+1)+(gn(xb, yb,temp)+2*gn(xe, ye,temp))*mislato/6;
                end
            end
        end
    end
    if modalita==2
        for n=0:Ntempo
            temp=n*deltat*T;
            for e=1:size(Ne)  %indici globali nodi su Neumann
                l=Ne(e);
                ib=borders(l,1); %vertice partenza
                %geom.elements.borders per ogni lato contiene 
                % [v1, v2, T1, T2] ((-1) se tocca dominio)
                %T1 T2 si riferiscono a posizione in geom.elements.triangle??
                ie=borders(l,2);%vertice arrivo
                im=borders(l,5);
                xb=x(ib); %coordinata x ib
                yb=y(ib);%%coordinata y ib
                xe=x(ie);%coordinata x ie
                ye=y(ie);%coordinata y ib
                xm=x(im); %coordinata x ib
                ym=y(im);%%coordinata y ib
                iib=pivot(ib);
                iie=pivot(ie);
                iim=pivot(im);
                mislato=sqrt((xe-xb)^2+(ye-yb)^2); %lunghezza lato
                if iib>0
                    bn(iib,n+1)=bn(iib,n+1)+ gn(xb, yb,temp)*mislato/6;
                end
                if iie>0
                    bn(iie,n+1)=bn(iie,n+1)+gn(xe, ye,temp)*mislato/6;
                end
                if iim>0
                    bn(iim,n+1)=bn(iim,n+1)+ 4*gn(xm,ym,temp)*mislato/6;
                end
            end
        end
    end
end

