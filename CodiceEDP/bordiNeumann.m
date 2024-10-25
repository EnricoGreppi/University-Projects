function [bn] = bordiNeumann(Ne,pivot, x, y, borders, gn, modalita)
    Ndof = max(pivot);
    bn=zeros(Ndof,1);
    if modalita ==1 
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
                bn(iib)=bn(iib)+ (2*gn(xb, yb)+gn(xe, ye))*mislato/6;
            end
            if iie>0
                bn(iie)=bn(iie)+(gn(xb, yb)+2*gn(xe, ye))*mislato/6;
            end
        end
    end
    if modalita==2
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
                bn(iib)=bn(iib)+ gn(xb, yb)*mislato/6;
            end
            if iie>0
                bn(iie)=bn(iie)+gn(xe, ye)*mislato/6;
            end
            if iim>0
                bn(iim)=bn(iim)+ 4*gn(xm,ym)*mislato/6;
            end
        end
    end
end

