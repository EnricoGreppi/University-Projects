function [A,Ad,b] = assemblatore2(f, Nd, x, y, ele, pivot)
%eps, beta,gamma,Ndof,Nd, geom )
Ndof = max(pivot);
Nt = length(ele);
epsif=@(x,y) x;
betaf=@(x,y) [1, 0];
gammaf=@(x,y) 0;
epsilon=0;
beta=0;
gamma=0;
effe=0;
gradiente=[1, 0, -1;
            0, 1, -1];
phi=@(x,y)[x, y, 1-x-y];
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
    a3=[x(ele(e, 3)),y(ele(e, 3))]';
    for j=1:3
        jj=pivot(ele(e,j));
        if jj>0
            for k=1:3
                kk=pivot(ele(e,k));
                for q=1:Nq
                    nodi=[csi(q), eta(q)]';
                    phivettore=phi(nodi(1),nodi(2));
                    arg=a3+B*nodi;
                    epsilon=epsilon+omega(q)*epsif(arg(1), arg(2))*gradiente(:,k)'*...
                    Bm1*Bmt*gradiente(:,j)*2*Area;
                    beta=beta+omega(q)*betaf(arg(1),arg(2))*Bmt*gradiente(:,k)*...
                        phivettore(j)*2*Area;
                    gamma=gamma+omega(q)*gammaf(arg(1),arg(2))*...
                        phivettore(k)*phivettore(j)*2*Area;
                end
                if kk>0
                    A(jj,kk)=A(jj,kk)+epsilon+beta+gamma;
                    
                else
                    Ad(jj,-kk)=Ad(jj,-kk)+epsilon+beta+gamma;
                end
                epsilon=0;
                beta=0;
                gamma=0;
            end
            for q=1:Nq
                nodi=[csi(q), eta(q)]';
                phivettore=phi(nodi(1),nodi(2));
                arg=a3+B*nodi;
                effe=effe+omega(q)*f(arg(1),arg(2))*phivettore(j)*2*Area;
            end
            b(jj)=b(jj)+effe;
            effe=0;
        end
    end
    
end
end


