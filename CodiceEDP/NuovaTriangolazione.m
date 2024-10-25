function [nuovaMesh] = NuovaTriangolazione(triangles)
oldN=length(triangles);
newN=oldN*4; %da un triangolo ne escono 4
j=1;
nuovaMesh=zeros(newN, 3);
for i=1:oldN
    nuovaMesh(j,1)=triangles(i,4);
    nuovaMesh(j,2)=triangles(i,2);
    nuovaMesh(j,3)=triangles(i,5);
    j=j+1;
    nuovaMesh(j,1)=triangles(i,1);
    nuovaMesh(j,2)=triangles(i,4);
    nuovaMesh(j,3)=triangles(i,6);
    j=j+1;
    nuovaMesh(j,1)=triangles(i,5);
    nuovaMesh(j,2)=triangles(i,3);
    nuovaMesh(j,3)=triangles(i,6);
    j=j+1;
    nuovaMesh(j,1)=triangles(i,4);
    nuovaMesh(j,2)=triangles(i,5);
    nuovaMesh(j,3)=triangles(i,6);
    j=j+1;
end
end

