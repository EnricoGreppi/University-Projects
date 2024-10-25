function [CapireErrore1,u,erroreL2,erroreH10]=solver(x, y, nVert, pivot,ele,  A, Ad, b, bn, Uesatta, ud,gradienteUesatta)
    u=zeros(nVert,1);
    uex=zeros(nVert,1);
    u0=A\(b-Ad*ud+bn);
    Nt = length(ele);
    CapireErrore1=zeros(nVert,3);
    for j=1:nVert
        jj=pivot(j);
        uex(j)=Uesatta(x(j),y(j));
        u(j)=0;
        if jj>0
            u(j)=u0(jj);
        else
            u(j)=ud(-jj);
        end
        CapireErrore1(j,:)=[x(j), y(j), uex(j)-u(j)];
    end
    figure;
    max(abs(uex-u))
    trisurf(ele, x, y, u)
    figure;
    trisurf(ele, x, y, uex)
    erroreL2=0;
    erroreH10=0;
    phi=@(x,y)[x, y, 1-x-y];
    gradiente=[1, 0, -1;
            0, 1, -1];
    for e=1:Nt
        Ge=[ele(e,1),ele(e,2),ele(e,3)];
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
                arg=a3+B*nodi;
                uel=0;
                graduel=[0,0]';
                for k=1:3
                    uel=uel+u(Ge(k))*phivettore(k);
                    graduel=graduel+u(Ge(k))*gradiente(:,k);
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