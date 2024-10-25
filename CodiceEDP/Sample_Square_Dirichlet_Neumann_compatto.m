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
BC.Boundary.Values = [1 5 3 7];
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

global geom;
% --------------------------------------------
% Creazione della triangolazione e plottaggio
% --------------------------------------------
numeroGiri=1;
Are=0.02;
Areavett=zeros(numeroGiri,1);
err1=zeros(numeroGiri,1);
err0=zeros(numeroGiri,1);
numcond=zeros(numeroGiri,1);
PI=1;
stabilizza=0;
for nG=1:numeroGiri
    RefiningOptions.AreaValue=Are;
    Areavett(nG)=Are;
    Are=Are*0.5;
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
    if PI== 2
        P2;
    end
    f=@(x,y) -x*(exp(x)+exp(y)) ; %termine forzante
    gn=@(x,y) (x==1)* exp(x); %condizione Neumann
    gd=@(x,y) exp(x)+exp(y); %condizione Dirichlet
    Uesatta= @(x,y) exp(x)+exp(y); %soluzione esatta
    gradienteUesatta= @(x,y) [exp(x),exp(y)]';
    Ndof=max(geom.pivot.pivot); %gradi di libertà (No condiz Dirichlet)
    %A=zeros(Ndof); %matrice di rigidezza
    [Nd,bo]=size(geom.pivot.Di); %Nd dimensione rilevamento Dirichlet
    %Ad=zeros (Ndof,Nd); % matrice relativa a Dirichlet
    ud=zeros(Nd,1); % relativo ad Ad 
    %b=zeros(Ndof,1);
    %bn=zeros(Ndof,1);
    %A*u0=b-Ad*ud+bn
    %per calcolare trasformazione da elemento di riferimento a elemento
    %qualsiasi
    if PI==2
        if stabilizza==0
            [A,Ad,b]=assemblatoreP2(f, Nd, geom.elements.coordinates(:,1),...
                geom.elements.coordinates(:,2), geom.elements.triangles,...
                geom.pivot.pivot); %eps, beta,gamma, Ndof,Nd, geom);
        end
        if stabilizza==1
            [A,Ad,b]=assemblaCDStabilizzazioneP2(f, Nd, geom.elements.coordinates(:,1),...
                geom.elements.coordinates(:,2), geom.elements.triangles,...
                geom.pivot.pivot);
        end
        bn=bordiNeumann(geom.pivot.Ne, geom.pivot.pivot, ...
            geom.elements.coordinates(:,1), geom.elements.coordinates(:,2), ...
            geom.elements.borders, gn,2);
    end
    if PI == 1
        if stabilizza==0
            [A,Ad,b]=assemblatore2(f, Nd, geom.elements.coordinates(:,1),...
                geom.elements.coordinates(:,2), geom.elements.triangles,...
                geom.pivot.pivot); %eps, beta,gamma, Ndof,Nd, geom);
        end
        if stabilizza==1
            [A,Ad,b]= assemblaCDStabilizzazione(f, Nd, geom.elements.coordinates(:,1),...
                geom.elements.coordinates(:,2), geom.elements.triangles,...
                geom.pivot.pivot);
        end
        bn=bordiNeumann(geom.pivot.Ne, geom.pivot.pivot, ...
            geom.elements.coordinates(:,1), geom.elements.coordinates(:,2), ...
            geom.elements.borders, gn,1);
    end

    for i=1:Nd %indici globali nodi su Dirichlet
        diri=geom.pivot.Di(i,1);
        xdiri=geom.elements.coordinates(diri,1);
        ydiri=geom.elements.coordinates(diri,2);
        ud(i)=gd(xdiri,ydiri);
    end
    
  if PI==1
      [CapireErrore1,solvet1,err0(nG),err1(nG)]=solver(geom.elements.coordinates(:,1), geom.elements.coordinates(:,2), ...
           geom.nelements.nVertexes, geom.pivot.pivot, geom.elements.triangles,...
            A, Ad, b, bn, Uesatta, ud,gradienteUesatta)
  end
 if PI==2
     [CapireErrore2,solvet2,err0(nG),err1(nG)]=solverP2(geom.elements.coordinates(:,1), geom.elements.coordinates(:,2), ...
            geom.pivot.pivot, geom.elements.triangles,...
            A, Ad, b, bn, Uesatta, ud,gradienteUesatta);
 end
    numcond(nG)=cond(A);
end
p=polyfit(log (Areavett), log(err0),1)
p=polyfit(log (Areavett), log(err1),1)
figure;
loglog(Areavett, err0)
xlabel("Area dei triangoli")
ylabel("errore in norma L2")

figure;
loglog(Areavett, err1)
xlabel("Area dei triangoli")
ylabel("errore in norma H1")

figure;
loglog(Areavett, numcond)
xlabel("Area dei triangoli")
ylabel("Numero di condizionamento")