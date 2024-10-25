function [CapireErrore1,u,erroreL2,erroreH10,u0]=solverP2Parabolica(x, y, pivot,ele,  A, Ad,Bpar,Bd, F, bn, Uesatta, ud,udprimo,gradienteUesatta,uzero,deltat,T)
    Ntempo=T/deltat;
    nNod=length(x);
    u=zeros(nNod,Ntempo+1);
    Ndof=max(pivot);
    u0=zeros(Ndof, Ntempo+1);
   % F=@(t) b*ft(t)+bn*gnt(t)- Ad*ud * gdt(t)- Bd*udprimo*gdprimot(t);
    Nt = length(ele);
    for i=1:nNod
        u(i,1)=uzero(x(i),y(i));
        ii=pivot(i);
        if ii>0
          u0(ii,1)=uzero(x(i),y(i));
        end
    end
    for n=1:(Ntempo)
      u0(:,n+1)=(Bpar+0.5*deltat*A)\((Bpar*u0(:,n))-(Bd*(ud(:,n+1)-ud(:,n)))...
          -0.5*deltat*A*u0(:,n)-deltat*0.5*Ad*(ud(:,n+1)+ud(:,n))+0.5*deltat*...
          (F(:,n+1)+F(:,n))+deltat*0.5*(bn(:,n)+bn(:,n+1)));
       for j=1:nNod
            jj=pivot(j);
            u(j,n+1)=0;
            if jj>0
                u(j,n+1)=u0(jj,n+1);
            else
                u(j,n+1)=ud(-jj,n+1);
            end
        end
    end
    uex=zeros(nNod,1);
    uex2=zeros(nNod,1);
    CapireErrore1=zeros(nNod,3);
    for j=1:nNod
        uex(j)=Uesatta(x(j),y(j),T);
        uex2(j)=Uesatta(x(j),y(j),3*deltat);
        CapireErrore1(j,:)=[x(j), y(j),abs( uex(j)-u(j,Ntempo+1))];
        CapireErrore2(j,:)=[x(j), y(j),abs( uex2(j)-u(j,2))];
    end
    CapireErrore1;
    CapireErrore2;
    figure;
    infinito=max(abs(u(:,Ntempo+1)-uex))
    nuovaMesh=NuovaTriangolazione(ele);
    trisurf(nuovaMesh, x, y, u(:,Ntempo+1))
    figure;
    trisurf(nuovaMesh, x,y,uex)
    erroreL2=0;
    erroreH10=0;
    gradiente=@(x,y)[4*x-1, 0, -3+4*y+4*x,  4*y, -4*y, 4-8*x-4*y;
                0, 4*y-1, -3+4*y+4*x, 4*x, 4-4*x-8*y, -4*x];
    phi=@(x,y)[2*x^2 - x, 2*y^2 - y, 1-3*x-3*y+4*x*y+2*x^2+2*y^2,...
             4*x*y, 4*y-4*x*y-4*y^2, 4*x-4*x^2-4*x*y];
    for e=1:Nt
        Ge=[ele(e,1),ele(e,2),ele(e,3),ele(e,4), ele(e,5), ele(e,6)];
            deltax(1)=x(ele(e, 3))- x(ele(e, 2));
            deltax(2)=x(ele(e, 1))- x(ele(e, 3));
            deltax(3)=x(ele(e, 2))- x(ele(e, 1));
            deltay(1)=y(ele(e, 2))- y(ele(e, 3));
            deltay(2)=y(ele(e, 3))- y(ele(e, 1));
            deltay(3)=y(ele(e, 1))- y(ele(e, 2));
            B=[deltax(2), -deltax(1);
                -deltay(2) deltay(1)];
            Area=0.5*abs(det(B));

            Bmt=(1/(2*Area))*[deltay(1), deltay(2);
                             deltax(1), deltax(2)];
            Bm1=Bmt';
            a3=[x(ele(e, 3)),y(ele(e, 3))]';
            Nq=7;
            [zita, csi, eta, omega]=int_nodes_weights(5);
            for q=1:Nq
                nodi=[csi(q), eta(q)]';
                phivettore=phi(nodi(1),nodi(2));
                gradienteVettore=gradiente(nodi(1),nodi(2));
                arg=a3+B*nodi;
                uel=0;
                graduel=[0,0]';
                for k=1:6
                    uel=uel+u(Ge(k),Ntempo+1)*phivettore(k);
                    graduel=graduel+u(Ge(k),Ntempo+1)*gradienteVettore(:,k);
                end
                erroreL2=erroreL2+omega(q)*(Uesatta(arg(1),arg(2),T)-...
                    uel)^2*2*Area;
                erroreH10=erroreH10+omega(q)*(gradienteUesatta(arg(1),arg(2),T)...
                    -Bmt*graduel)'*(gradienteUesatta(arg(1),arg(2),T)...
                    -Bmt*graduel)*2*Area;
            end
    end
    sqrt(erroreL2);
    sqrt(erroreH10);
end