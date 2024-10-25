function [A,Ad,b] = assemblaCDStabilizzazioneP2(f, Nd, x, y, ele, pivot)
%eps, beta,gamma,Ndof,Nd, geom )
Ndof = max(pivot);
Nt = length(ele);
epsif=0.01;
betaf=@(x,y) [100000,100000];
epsilon=0;
beta=0;
effe=0;
stabilizza1=0;
stabilizza2=0;
stabilizza3=0;
laplace=[4, 0, 4, 0, 0, -8
         0, 0, 4, 4, -4, -4
         0, 4, 4, 0, -8, 0];
gradiente=@(x,y)[4*x-1, 0, -3+4*y+4*x,  4*y, -4*y, 4-8*x-4*y;
            0, 4*y-1, -3+4*y+4*x, 4*x, 4-4*x-8*y, -4*x];
phi=@(x,y)[2*x^2 - x, 2*y^2 - y, 1-3*x-3*y+4*x*y+2*x^2+2*y^2,...
         4*x*y, 4*y-4*x*y-4*y^2, 4*x-4*x^2-4*x*y];
A=zeros(Ndof);
Ad=zeros(Ndof,Nd);
b=zeros(Ndof,1);
deltax=zeros(1,3);
deltay=zeros(1,3);
Nq=7;
[zita, csi, eta, omega]=int_nodes_weights(5);

for e=1:Nt%geom.nelements.nTriangles
    %ve=zeros(1,3);
    %for z=1:3
    %ve(z)=geom.elements.triangles(e,z);
    %end
    
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
    BBB=Bm1*Bmt;
    a3=[x(ele(e, 3)),y(ele(e, 3))]';
    normabeta=0;
    normabetaq=0;
    for q=1:Nq
        nodi=[csi(q), eta(q)]';
        arg=a3+B*nodi;
        normabetaq=normabetaq+omega(q)*betaf(arg(1),arg(2))*(betaf(arg(1),arg(2))')*2*Area;
    end
    normabeta=sqrt(normabetaq);
    he=sqrt(Area);
    PEh=(normabeta*he)/(48*epsif);
    if PEh <= 1
        viscE=Area/(96*epsif);
        0
    else
        viscE= he/(2*normabeta);
        PEh
    end
    for j=1:6
        jj=pivot(ele(e,j));
        if jj>0
            for k=1:6
                kk=pivot(ele(e,k));
                for q=1:Nq
                    nodi=[csi(q), eta(q)]';
                    phivettore=phi(nodi(1),nodi(2));
                    gradienteValutato=gradiente(nodi(1), nodi(2));
                    arg=a3+B*nodi;
                    epsilon=epsilon+omega(q)*epsif*gradienteValutato(:,k)'*...
                    Bm1*Bmt*gradienteValutato(:,j)*2*Area;
                    beta=beta+omega(q)*betaf(arg(1),arg(2))*Bmt*gradienteValutato(:,k)*...
                        phivettore(j)*2*Area;
                    stabilizza1=stabilizza1+omega(q)*(betaf(arg(1),arg(2))*Bmt*gradienteValutato(:,k))*...
                        (betaf(arg(1),arg(2))*Bmt*gradienteValutato(:,j))*2*Area;
                    stabilizza3=stabilizza3+omega(q)*epsif*(BBB(1,1)*laplace(1,k)+...
                        (BBB(2,1)+BBB(1,2))*laplace(2,k)+BBB(2,2)*laplace(3,k))*...
                        (betaf(arg(1),arg(2))*Bmt*gradienteValutato(:,j))*2*Area;
                end
                if kk>0
                    A(jj,kk)=A(jj,kk)+epsilon+beta+viscE*stabilizza1-viscE*stabilizza3;
                    
                else
                    Ad(jj,-kk)=Ad(jj,-kk)+epsilon+beta+viscE*stabilizza1-viscE*stabilizza3;
                end
                epsilon=0;
                beta=0;
                stabilizza1=0;
                stabilizza3=0;
            end
            for q=1:Nq
                nodi=[csi(q), eta(q)]';
                phivettore=phi(nodi(1),nodi(2));
                gradienteValutato=gradiente(nodi(1), nodi(2));
                arg=a3+B*nodi;
                effe=effe+omega(q)*f(arg(1),arg(2))*phivettore(j)*2*Area;
                stabilizza2=stabilizza2+omega(q)*f(arg(1),arg(2))*...
                    (betaf(arg(1),arg(2))*Bmt*gradienteValutato(:,j))*2*Area;
            end
            b(jj)=b(jj)+effe+viscE*stabilizza2;
            effe=0;
            stabilizza2=0;
        end
    end
    
end
end