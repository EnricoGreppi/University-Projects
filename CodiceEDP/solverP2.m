function [CapireErrore2,u,erroreL2,erroreH10]=solverP2(x, y, pivot,ele,  A, Ad, b, bn, Uesatta, ud,gradienteUesatta)
    nNod=length(x);
    u=zeros(nNod,1);
    uex=zeros(nNod,1);
    u0=A\(b-Ad*ud+bn);
    Nt = length(ele);
    CapireErrore2=zeros(nNod,3);
    for j=1:nNod
        jj=pivot(j);
        uex(j)=Uesatta(x(j),y(j));
        u(j)=0;
        if jj>0
            u(j)=u0(jj);
        else
            u(j)=ud(-jj);
        end
        CapireErrore2(j,:)=[x(j), y(j), uex(j)-u(j)];
    end
    CapireErrore2
    figure;
    nuovaMesh=NuovaTriangolazione(ele);
    trisurf(nuovaMesh, x, y, u)
    figure;
    trisurf(nuovaMesh, x,y,uex)
    max(abs(uex-u))
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
                    uel=uel+u(Ge(k))*phivettore(k);
                    graduel=graduel+u(Ge(k))*gradienteVettore(:,k);
                end
                erroreL2=erroreL2+omega(q)*(Uesatta(arg(1),arg(2))-...
                    uel)^2*2*Area;
                erroreH10=erroreH10+omega(q)*(gradienteUesatta(arg(1),arg(2))...
                    -Bmt*graduel)'*(gradienteUesatta(arg(1),arg(2))...
                    -Bmt*graduel)*2*Area;
            end
    end
    sqrt(erroreL2);
    sqrt(erroreH10);
end