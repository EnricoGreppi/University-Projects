if(~exist('bbtr30'))
     addpath('/Users/enricogreppi/Desktop/MNperEDP/triangolatore/Long/bbtr30')
     disp('../bbtr30 added to the path')
end

%----------------------------------------------------------------------------
%
% Triangolazione di un dominio quadrata
% con condizioni di Dirichlet sul bordo
%
%----------------------------------------------------------------------------
%
%  Autore: Stefano Berrone
%  Politecnico di Torino
%
%----------------------------------------------------------------------------

%clc
%clear all

% -------------------------------
% Inserimento dei vertici
% -------------------------------

Domain.InputVertex = [ 0 0
                       1 0
                       1 1
                       0 1];


% ---------------------------------------------
% Definizione del dominio a partire dai Vertici
% ---------------------------------------------

% Dichiaro le variabili per delimitare il dominio
Domain.Boundary.Values = 1:4;
% lato di bordo 1 dal nodo 1 al nodo 2
% lato di bordo 2 dal nodo 2 al nodo 3
% lato di bordo 3 dal nodo 3 al nodo 4
% lato di bordo 4 dal nodo 4 al nodo 1

Domain.Holes.Hole = [];       % non ci sono buchi nel dominio
Domain.Segments.Segment = []; % non ci sono lati forzati nel dominio

% --------------------------------------------------
% Definizione delle condizioni al contorno a partire
% dai Vertici e dai lati di bordo
% --------------------------------------------------

% valori numerici per le condizioni al contorno
BC.Values = [0.0 12.0 0.0 14.0 0.0 16.0 0.0 0.0 0.0];

% marker delle condizioni al contorno sui bordi del dominio
% dispari -> Dirichlet; pari -> Neumann
BC.Boundary.Values = [1 2 3 4];
% marker dei Vertici iniziali
BC.InputVertexValues = [1 1 1 1];
% Questi indici posso essere anche indici ai valori numerici
% contenuti nel vettore BC.Values

BC.Holes.Hole = [];
BC.Segments.Segment = [];



% --------------------------------------------
% Inserimento dei parametri di triangolazione
% --------------------------------------------

RefiningOptions.CheckArea  = 'Y';
RefiningOptions.CheckAngle = 'N';
RefiningOptions.AreaValue  = 0.02;
RefiningOptions.AngleValue = [];
RefiningOptions.Subregions = [];


% --------------------------------------------
% Creazione della triangolazione e plottaggio
% --------------------------------------------

[geom] = bbtr30(Domain,BC,RefiningOptions);
draw_grid (geom,1);

% --------------------------------------------------
% --------------------------------------------------


% --------------------------------------------------
% Rielaborazione dei prodotti del triangolatore
% per un piu` agevole trattamento delle condizioni
% al contorno
% --------------------------------------------------

geom.elements.coordinates = geom.elements.coordinates(...
				1:geom.nelements.nVertexes,:);
geom.elements.triangles = geom.elements.triangles(...
				1:geom.nelements.nTriangles,:);
geom.elements.borders = geom.elements.borders(...
				1:geom.nelements.nBorders,:);
geom.elements.neighbourhood = geom.elements.neighbourhood(...
				1:geom.nelements.nTriangles,:);

% --------------------------------------------------

j  = 1;
Dj = 1;
for i=1:size(geom.pivot.nodelist)
     if geom.pivot.nodelist(i)==0
        geom.pivot.pivot(i)=j;
        j = j+1;
     else
        geom.pivot.pivot(i)=-Dj;
        Dj = Dj + 1;
     end
end

% --------------------------------------------------

geom.pivot.pivot = transpose(geom.pivot.pivot);

% --------------------------------------------------

% geom.pivot.Di dopo le operazioni seguenti contiene l`indice dei nodi
% di Dirichlet e il corrispondente marker

[X,I] = sort(geom.pivot.Di(:,1));
geom.pivot.Di = geom.pivot.Di(I,:);

clear X I;
global geom;
f2=@(x,y) -2*exp(x+y); %termine forzante
gn2=@(x,y) x*exp(x+y)- (1-x)*exp(x+y); %condizione Neumann
gd2=@(x,y) exp(x+y); %condizione Dirichlet
Uesatta= @(x,y) exp(x+y); %soluzione esatta
Ndof=max(geom.pivot.pivot); %gradi di libertà (No condiz Dirichlet)
A2=zeros(Ndof); %matrice di rigidezza
[Nd,bo]=size(geom.pivot.Di); %Nd dimensione rilevamento Dirichlet
Ad2=zeros (Ndof,Nd); % matrice relativa a Dirichlet
ud2=zeros(Nd,1); % relativo ad Ad 
b2=zeros(Ndof,1);
bn2=zeros(Ndof,1);
%A*u0=b-Ad*ud+bn
deltax=zeros(1,3);
deltay=zeros(1,3);
%per calcolare trasformazione da elemento di riferimento a elemento
%qualsiasi
eps=1;
beta=zeros(2,1);
gamma=0;
%coefficienti del problema

for e=1:geom.nelements.nTriangles
    ve=zeros(1,3);
    for z=1:3
    ve(z)=geom.elements.triangles(e,z);
    end
    deltax(1)=geom.elements.coordinates(ve(3),1)-...
        geom.elements.coordinates(ve(2),1);
    deltax(2)=geom.elements.coordinates(ve(1),1)-...
        geom.elements.coordinates(ve(3),1);
    deltax(3)=geom.elements.coordinates(ve(2),1)-...
        geom.elements.coordinates(ve(1),1);
    deltay(1)=geom.elements.coordinates(ve(2),2)-...
        geom.elements.coordinates(ve(3),2);
    deltay(2)=geom.elements.coordinates(ve(3),2)-...
        geom.elements.coordinates(ve(1),2);
    deltay(3)=geom.elements.coordinates(ve(1),2)-...
        geom.elements.coordinates(ve(2),2);
    B=[deltax(2), -deltax(1);
        -deltay(2) deltay(1)]; %per trasformazione el rif el qual
    Area=0.5*abs(det(B));
    %B^-T; B^-1 ??
    for j=1:3
        jj=geom.pivot.pivot(geom.elements.triangles(e,j));%indici negativi per Dirichlet
        if jj>0
            for k=1:3
                kk=geom.pivot.pivot(geom.elements.triangles(e,k));%indici negativi per Dirichlet
                if kk>0
                    A2(jj,kk)=A2(jj,kk)+eps*(deltay(k)*deltay(j)+deltax(k)*...
                        deltax(j))/(4*Area)+(beta(1)*deltay(k) + beta(2)*...
                        deltax(k))/6 +(gamma*Area/12)*(1+(j==k));
                else
                    Ad2(jj,-kk)=Ad2(jj,-kk)+eps*(deltay(k)*deltay(j)+deltax(k)*...
                        deltax(j))/(4*Area)+(beta(1)*deltay(k) + beta(2)*...
                        deltax(k))/6 +(gamma*Area/12)*(1+(j==k));
                end
            end
            b2(jj)=b2(jj)+f2(geom.elements.coordinates(ve(j),1),...
                geom.elements.coordinates(ve(j),2))*Area/3;
        end
    end
end





for e=1:size(geom.pivot.Ne)  %indici globali nodi su Neumann
    l=geom.pivot.Ne(e);
    ib=geom.elements.borders(l,1); %vertice partenza
    %geom.elements.borders per ogni lato contiene 
    % [v1, v2, T1, T2] ((-1) se tocca dominio)
    %T1 T2 si riferiscono a posizione in geom.elements.triangle??
    ie=geom.elements.borders(l,2);%vertice arrivo
    xb=geom.elements.coordinates(ib,1); %coordinata x ib
    yb=geom.elements.coordinates(ib,2);%%coordinata y ib
    xe=geom.elements.coordinates(ie,1);%coordinata x ie
    ye=geom.elements.coordinates(ie,2);%coordinata y ib
    iib=geom.pivot.pivot(ib);
    iie=geom.pivot.pivot(ie);
    mislato=sqrt((xe-xb)^2+(ye-yb)^2); %lunghezza lato
    if iib>0
        bn2(iib)=bn2(iib)+ (2*gn2(xb, yb)+gn2(xe, ye))*mislato/6;
    end
    if iie>0
        bn2(iie)=bn2(iie)+(2*gn2(xb, yb)+gn2(xe, ye))*mislato/6;
    end
end

for i=1:Nd %indici globali nodi su Dirichlet
    diri=geom.pivot.Di(i,1);
    xdiri=geom.elements.coordinates(diri,1);
    ydiri=geom.elements.coordinates(diri,2);
    ud2(i)=gd2(xdiri,ydiri);
end
u=zeros(geom.nelements.nVertexes,1);
uex=zeros(geom.nelements.nVertexes,1);
u0=A2\(b2-Ad2*ud2+bn2);
for j=1:geom.nelements.nVertexes
    jj=geom.pivot.pivot(j);
    uex(j)=Uesatta(geom.elements.coordinates(j,1),geom.elements.coordinates(j,2));
    u(j)=0;
    if jj>0
        u(j)=u0(jj);
    else
        u(j)=ud2(-jj);
    end
end
uex-u
figure;
trisurf(geom.elements.triangles,geom.elements.coordinates(:,1),...
    geom.elements.coordinates(:,2), u)


