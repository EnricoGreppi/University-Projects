function [A,Ad,b] = assemblatore(f, Nd, x, y, ele, pivot)
%eps, beta,gamma,Ndof,Nd, geom )
Ndof = max(pivot);
Nt = length(ele);
eps=1;
beta=[0, 0];
gamma=0;

A=zeros(Ndof);
Ad=zeros(Ndof,Nd);
b=zeros(Ndof,1);
deltax=zeros(1,3);
deltay=zeros(1,3);

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
    %B^-T; B^-1 ??
    for j=1:3
        jj=pivot(ele(e,j));
        if jj>0
            for k=1:3
                kk=pivot(ele(e,k));
                if kk>0
                    A(jj,kk)=A(jj,kk)+eps*(deltay(k)*deltay(j)+deltax(k)*...
                        deltax(j))/(4*Area)+(beta(1)*deltay(k) + beta(2)*...
                        deltax(k))/6 +(gamma*Area/12)*(1+(j==k));
                else
                    Ad(jj,-kk)=Ad(jj,-kk)+eps*(deltay(k)*deltay(j)+deltax(k)*...
                        deltax(j))/(4*Area)+(beta(1)*deltay(k) + beta(2)*...
                        deltax(k))/6 +(gamma*Area/12)*(1+(j==k));
                end
            end
            b(jj)=b(jj)+f(x(ele(e, j)), y(ele(e, j)))*Area/3;
        end
    end
end
end

