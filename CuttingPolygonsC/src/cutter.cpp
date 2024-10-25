#include "cutter.h"
namespace CutterNamespace
{

ConvexCutter::ConvexCutter()
{
    _polygons.resize(1);
    _polygons[0]=Polygon();
    _cut=Segment();
    _points.push_back(Point());
}

ConvexCutter::~ConvexCutter()
{

}

void ConvexCutter::SetCutter(const vector<Point> & points,const vector<Vertex>& vertex,const Segment &segment)
{
    int n;
    n=points.size();
    for (int i=0; i<n;i++)
        _points.push_back(points[i]);
    _polygons.resize(1);
    _polygons[0]=Polygon(vertex);
    _cut=segment;
}
void ConvexCutter::SetTolerance(const double &toll)
{
    tolerance=toll;
}

double ConvexCutter::GetTolerance()
{
    return tolerance;
}

Segment ConvexCutter::GetCut() const
{
    return _cut;
}
Polygon ConvexCutter::GetPolygon(const int &i)const
{
    return _polygons[i];
}




void ConvexCutter::CutPolygon(const vector<Point> & points,const vector<Vertex> & polygonVertices,const Segment &segment)
{
    SetCutter(points,polygonVertices,segment);
    Polygon oldP;
    cambi=0; //dopo due cambi nel caso convesso non si controlla piu, se cambi è uguale a 10 siamo nel caso cutter coincidente a un lato
    oldP=GetPolygon(0);
    Point intersezione;
    Vertex intersezioneV;
    Vertex vertice;
    double distanzaFrom;
    double distanzaTo;
    int numeroNewVertice=oldP.NumberVertices();
    Segment intersecato;
    Segment cutter=GetCut();
    Point from= cutter.GetFrom();
    Point to=cutter.GetTo();
    Vertex tov;
    Vertex fromv;
    int a;
    for (int i=0;i<oldP.NumberVertices() && cambi<2; i++)
    {
        a=i; //serve dopo
        vertice=oldP.GetVertex(i);
        if (cambi==0)
            leftSide.push(vertice); // leftside in realtà è parte1, potrebbe anche trovarsi a destra ma viene pushato qui
        else
            rightSide.push(vertice); //dopo il primo cambio qui
        intersecato=oldP.GetEdge(i);
        intersector.SetCutter(cutter);
        intersector.SetEdge(intersecato);
        intersector.ComputeIntersection();

        if(intersector.TypeIntersection()==Intersector::IntersectionOnLine || intersector.TypeIntersection()==Intersector::IntersectionOnSegment)// ce intersezione
        {
            if (intersector.PositionIntersectionEdge()!=Intersector::End && intersector.PositionIntersectionEdge()!=Intersector::Begin) // intersezione sta  in mezzo
            {
                intersezione = Point(intersector.IntersectionEdgeParametricCoordinate(intersecato));
                intersezioneV = Vertex(numeroNewVertice,intersezione);
                _points.push_back(intersezione);
                numeroNewVertice=numeroNewVertice+1; // il prossimo vertice nuovo avrà questo id
                leftSide.push(intersezioneV); //la pusho qui, dopo la pusherò  anche a destra, vedi fine controlli
                cambi=cambi+1; //mi segno il cambio
                distanzaFrom=intersezione.ComputeNorm2(from);
                distanzaTo=intersezione.ComputeNorm2(to); //controlli per pushare from e to se sono interni al poligono
                if ( oldP.PuntoInterno(from) && oldP.PuntoInterno(to) && cambi==1)
                {
                    if (distanzaFrom<distanzaTo)
                    {
                        fromv=Vertex(numeroNewVertice,from);
                       leftSide.push(fromv);
                        numeroNewVertice=numeroNewVertice+1;
                        tov=Vertex(numeroNewVertice,to);
                        leftSide.push(tov);
                        _points.push_back(from);
                        _points.push_back(to);
                         numeroNewVertice=numeroNewVertice+1;
                         rightSide.push(tov);
                         rightSide.push(fromv);
                    }
                    else
                    {
                        tov=Vertex(numeroNewVertice,to);
                        leftSide.push(tov);
                         numeroNewVertice=numeroNewVertice+1;
                         fromv=Vertex(numeroNewVertice,from);
                         leftSide.push(fromv);
                          numeroNewVertice=numeroNewVertice+1;
                          _points.push_back(from);
                          _points.push_back(to);
                          rightSide.push(fromv);
                          rightSide.push(tov);

                    }
                }
                else if (cambi==1 && oldP.PuntoInterno(from))
                {
                    fromv=Vertex(numeroNewVertice,from);
                    _points.push_back(from);
                    leftSide.push(fromv);
                    rightSide.push(fromv);
                     numeroNewVertice=numeroNewVertice+1;

                }
                else if(cambi==1 && oldP.PuntoInterno(to))
                {
                    tov=Vertex(numeroNewVertice,to);
                    _points.push_back(to);
                    leftSide.push(tov);
                    rightSide.push(tov);
                     numeroNewVertice=numeroNewVertice+1;

                }

                rightSide.push(intersezioneV); //la  pusho di la

            }

        }
        if (intersector.TypeIntersection()==Intersector::IntersectionParallelOnLine || intersector.TypeIntersection()==Intersector::IntersectionParallelOnSegment) //casi paralleli
        {
            cambi=10;

            if ( oldP.PuntoInterno(from) && oldP.PuntoInterno(to)) //controlli per from e to
            {
                distanzaFrom=vertice.GetPoint().ComputeNorm2(from);
                distanzaTo=vertice.GetPoint().ComputeNorm2(to);
                if (distanzaFrom<distanzaTo)
                {
                   leftSide.push(Vertex(numeroNewVertice,from));
                    numeroNewVertice=numeroNewVertice+1;
                    leftSide.push(Vertex(numeroNewVertice,to));
                     numeroNewVertice=numeroNewVertice+1;
                     _points.push_back(from);
                     _points.push_back(to);
                }
                else
                {
                    leftSide.push(Vertex(numeroNewVertice,to));
                     numeroNewVertice=numeroNewVertice+1;
                     leftSide.push(Vertex(numeroNewVertice,from));
                      numeroNewVertice=numeroNewVertice+1;
                      _points.push_back(from);
                      _points.push_back(to);

                }
            }
            else if (oldP.PuntoInterno(from))
            {
                leftSide.push(Vertex(numeroNewVertice,from));
                 numeroNewVertice=numeroNewVertice+1;
                 _points.push_back(from);
            }
            else if( oldP.PuntoInterno(to))
            {
                leftSide.push(Vertex(numeroNewVertice,to));
                 numeroNewVertice=numeroNewVertice+1;
                 _points.push_back(to);
            }

        }
        if(intersector.PositionIntersectionEdge() == Intersector::Begin) //inizio
        {
            distanzaFrom=vertice.GetPoint().ComputeNorm2(from);
            distanzaTo=vertice.GetPoint().ComputeNorm2(to);
            if ( oldP.PuntoInterno(from) && oldP.PuntoInterno(to) && cambi==0)
            {
                if (distanzaFrom<distanzaTo)
                {
                    fromv=Vertex(numeroNewVertice,from);
                   leftSide.push(fromv);
                    numeroNewVertice=numeroNewVertice+1;
                    tov=Vertex(numeroNewVertice,to);
                    leftSide.push(tov);
                     numeroNewVertice=numeroNewVertice+1;
                     rightSide.push(tov);
                     rightSide.push(fromv);
                     _points.push_back(from);
                     _points.push_back(to);
                }
                else
                {
                    tov=Vertex(numeroNewVertice,to);
                    leftSide.push(tov);
                     numeroNewVertice=numeroNewVertice+1;
                     fromv=Vertex(numeroNewVertice,from);
                     leftSide.push(fromv);
                      numeroNewVertice=numeroNewVertice+1;
                      rightSide.push(fromv);
                      rightSide.push(tov);
                      _points.push_back(from);
                      _points.push_back(to);

                }
            }
            else if (cambi==0 && oldP.PuntoInterno(from))
            {
                fromv=Vertex(numeroNewVertice,from);
                leftSide.push(fromv);
                 numeroNewVertice=numeroNewVertice+1;

                 _points.push_back(from);
            }
            else if(cambi==0 && oldP.PuntoInterno(to))
            {
                tov=Vertex(numeroNewVertice,to);
                leftSide.push(Vertex(numeroNewVertice,to));
                 numeroNewVertice=numeroNewVertice+1;

                 _points.push_back(to);
            }
            if (cambi==0)
                rightSide.push(vertice);
            else
                leftSide.push(vertice);
            cambi=cambi+1;

          }
    }
    a=a+1;
    for (int k=a;k<oldP.NumberVertices();k++)
    {
        vertice=oldP.GetVertex(k);
        leftSide.push(vertice);
    }
    SetPolygon();
}

void ConvexCutter::SetPolygon()
{
    if (cambi==0)
    {
        _polygons.resize(2);
        _polygons[1]=_polygons[0];

    }
    else if (cambi==2)
    {
        _polygons.resize(3);
        _polygons[1]=Polygon(rightSide);
        _polygons[2]=Polygon(leftSide);
    }
    else if(cambi==10)
    {
        _polygons.resize(2);
        _polygons[1]=Polygon(leftSide);
    }

}





//********************************************************************************************


ConcaveCutter::ConcaveCutter()
{
    _cut=Segment();
    _points.push_back(Point());
    _polygons.push_back(Polygon());
}

ConcaveCutter::~ConcaveCutter()
{

}

void ConcaveCutter::SetCutter(const vector<Point>& points,const vector<Vertex> &vertex,const Segment &segment)
{
    int n;
    n=points.size();
    for (int i=0; i<n;i++)
        _points.push_back(points[i]);
    _polygons.push_back(Polygon(vertex));
    _cut=segment;
}

void ConcaveCutter::SetTolerance(const double &toll)
{
    tolerance=toll;
}

Segment ConcaveCutter::GetCut() const
{
    return _cut;
}

double ConcaveCutter::GetTolerance()
{
    return tolerance;
}




void ConcaveCutter::CutPolygon(const vector<Point>& points, const  vector<Vertex>& polygonVertices,const Segment &segment)
{

    SetCutter(points,polygonVertices,segment);
    Polygon oldP;
    oldP=Polygon(polygonVertices);
    Point intersezione;
    Vertex intersezioneV;
    Point intersezione2;
    Vertex intersezioneV2;
    Vertex vertice;
    Segment::PositionSide side;
    int idContr;
    int casoParticolare=0;
    int contrS;
    int numeroNewVertice;
    int ciclo=0;
    numeroNewVertice=oldP.NumberVertices();
    Segment lato;
    Segment lato2;
    Segment cutter=GetCut();
    Segment::PositionSide posizione;
    Segment::PositionSide posizioneParticolare;
    int esci=0;
    Point from= cutter.GetFrom();
    Point to=cutter.GetTo();
    double distanzaFrom;
    double distanzaTo;
    Vertex tov;
    Vertex fromv;
    for (int f=0; f<oldP.NumberVertices();f++)
        nuoviVertici.push_back(oldP.GetVertex(f));


    for (int j=0; j<oldP.NumberVertices();j++)
    {
        vertice=oldP.GetVertex(j);
        if (SearchInseriti(j)==false) //se vertice j non è ancora stato inserito da nessuna parte entra qui, se non è stato inserito non lo prendiamo come rif
        {
            side=cutter.SidePoint(vertice.GetPoint()); //mi segno la posizione, posizione del vertice rif rispetto al cut
            if (side!=  Segment::Left && side != Segment::Right)  //prendo come vertici di riferimento soltanto quelli che stanno oa destra o a sinistra
            {
                casoParticolare=1; //mi farà poi uscire da tutto il ciclo dopo
                inseriti.push_back(j); //tutti i particolari li lascio stare quando cerco i v di riferimento
            }
            if (casoParticolare!=1) //lo prende come rif
            {
                appoggio.push(vertice);  //appoggio per poligono in costruzione
                inseriti.push_back(j);
                inseritiAppoggio.push_back(j);
            }
            if (j==(oldP.NumberVertices()-1)) //quando arrivo all' ultimo vertice devo poi controllare nel ciclo dopo il vertice 0
                j=-1;
            for (int i=j+1; i<oldP.NumberVertices() && ciclo==0;i++) // questo ciclo serve per costruire il poligono a cui appartiene  il vertice di rif
            {
                if (casoParticolare==1)
                    break;
                vertice=oldP.GetVertex(i);
                if (SearchInseritiAppoggio(i)==true) //quando si riarriva a un  vertice  gia  inserito  nel poligono di costruzione possiamo uscire
                    ciclo=1;//per uscire dal ciclo
                posizione=cutter.SidePoint(vertice.GetPoint());
                if (posizione==side && ciclo==0 )  // se è  dalla stessa parte del v  di rif lo aggiungo
                {
                    appoggio.push(vertice);
                    inseriti.push_back(i);
                    inseritiAppoggio.push_back(i);
                }
                if  (posizione!=side) // se non lo è cerco  di capire  che succede
                {
                    if (i==0)
                        i=oldP.NumberVertices();
                    lato=oldP.GetEdge(i-1);
                    intersector.SetCutter(cutter);
                    intersector.SetEdge(lato);
                    intersector.ComputeIntersection();

                    if (intersector.TypeIntersection()!=Intersector::IntersectionParallelOnLine && intersector.TypeIntersection()!=Intersector::IntersectionParallelOnSegment)
                    {
                        intersezione=Point(intersector.IntersectionEdgeParametricCoordinate(lato));
                        intersezioneV = Vertex(numeroNewVertice,intersezione);
                        if (SearchVertici(intersezioneV)==false) //  per controllare se il vertice è nuovo
                        {
                            numeroNewVertice=numeroNewVertice+1;
                            nuoviVertici.push_back(intersezioneV);
                            _points.push_back(intersezione);
                        }
                        else
                        {
                            intersezioneV=GiustoVertice(intersezioneV);
                        }

                        appoggio.push(intersezioneV);                      
                        distanzaFrom=intersezione.ComputeNorm2(from);
                        distanzaTo=intersezione.ComputeNorm2(to);


                        idContr=GiustoSegmento(lato,oldP,intersezione); //da qui parte il lato da controllare
                        lato2=oldP.GetEdge(idContr);
                        intersector.SetCutter(cutter);
                        intersector.SetEdge(lato2);
                        intersector.ComputeIntersection();
                        if (intersector.PositionIntersectionEdge()==Intersector::Begin)
                        {
                            posizioneParticolare=cutter.SidePoint(lato2.GetTo());
                            if (posizioneParticolare!=side) // se siamo in questo caso non inserisco niente perchè altrimenti inserisce tutto due volte (vedi  ultimo caso)
                                esci=1;
                        }
                        if (intersector.TypeIntersection()!=Intersector::IntersectionParallelOnLine && intersector.TypeIntersection()!=Intersector::IntersectionParallelOnSegment)
                        {
                            intersezione2=Point(intersector.IntersectionEdgeParametricCoordinate(lato2));
                            contrS=ControlloSegmento(lato,lato2,cutter); //controlla per inserire  s1 e s2
                            if (contrS==3 && esci==0)
                            {
                                if(distanzaFrom<distanzaTo)
                                {
                                    fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                    if (SearchVertici(fromv)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(fromv);
                                        _points.push_back(from);
                                    }
                                    else
                                    {
                                        fromv=GiustoVertice(fromv);
                                    }

                                    tov=Vertex(numeroNewVertice,cutter.GetTo());
                                    if (SearchVertici(tov)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(tov);
                                        _points.push_back(to);
                                    }
                                    else
                                    {
                                        tov=GiustoVertice(tov);
                                    }
                                    appoggio.push(fromv);
                                    appoggio.push(tov);
                                }
                                else
                                {
                                    tov=Vertex(numeroNewVertice,cutter.GetTo());
                                    if (SearchVertici(tov)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(tov);
                                        _points.push_back(to);
                                    }
                                    else
                                    {
                                        tov=GiustoVertice(tov);
                                    }
                                    fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                    if (SearchVertici(intersezioneV)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(fromv);
                                        _points.push_back(from);
                                    }
                                    else
                                    {
                                       fromv=GiustoVertice(fromv);
                                    }

                                    appoggio.push(tov);
                                    appoggio.push(fromv);
                                }
                            }
                            if (contrS==2  && esci==0)
                            {
                                tov=Vertex(numeroNewVertice,cutter.GetTo());
                                if (SearchVertici(tov)==false)
                                {
                                    numeroNewVertice=numeroNewVertice+1;
                                    nuoviVertici.push_back(tov);
                                    _points.push_back(to);
                                }
                                else
                                {
                                    tov=GiustoVertice(tov);
                                }
                                appoggio.push(tov);
                            }
                            if (contrS==1 && esci==0)
                            {
                                fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                if (SearchVertici(fromv)==false)
                                {
                                    numeroNewVertice=numeroNewVertice+1;
                                    nuoviVertici.push_back(fromv);
                                    _points.push_back(from);
                                }
                                else
                                {
                                    fromv=GiustoVertice(fromv);
                                }
                                appoggio.push(fromv);
                            }
                            intersezioneV2=Vertex(numeroNewVertice,intersezione2);
                            if (SearchVertici(intersezioneV2)==false && esci==0)
                            {
                                numeroNewVertice=numeroNewVertice+1;
                                nuoviVertici.push_back(intersezioneV2);
                                _points.push_back(intersezione2);
                            }
                            else if (esci==0)
                            {
                                intersezioneV2=GiustoVertice(intersezioneV2);
                            }

                            if (esci==0)
                                appoggio.push(intersezioneV2);
                            i=idContr;
                            esci=0;
                    }
                        else if(intersector.TypeIntersection()==Intersector::IntersectionParallelOnLine || intersector.TypeIntersection()==Intersector::IntersectionParallelOnSegment) //casi paralleli
                        {
                            Point intersezionePar;
                            Vertex verticePar;
                            verticePar=oldP.GetVertex(idContr);
                            intersezionePar=verticePar.GetPoint();
                            appoggio.push(verticePar);
                            contrS=ControlloSegmento2(lato2,cutter);
                            if (contrS==3)
                            {
                                if(distanzaFrom<distanzaTo)
                                {
                                    fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                    if (SearchVertici(fromv)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(fromv);
                                        _points.push_back(from);
                                    }
                                    else
                                    {
                                        fromv=GiustoVertice(fromv);
                                    }

                                    tov=Vertex(numeroNewVertice,cutter.GetTo());
                                    if (SearchVertici(tov)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(tov);
                                        _points.push_back(to);
                                    }
                                    else
                                    {
                                        tov=GiustoVertice(tov);
                                    }
                                    appoggio.push(fromv);
                                    appoggio.push(tov);
                                }
                                else
                                {
                                    tov=Vertex(numeroNewVertice,cutter.GetTo());
                                    if (SearchVertici(tov)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(tov);
                                        _points.push_back(to);
                                    }
                                    else
                                    {
                                        tov=GiustoVertice(tov);
                                    }
                                    fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                    if (SearchVertici(intersezioneV)==false)
                                    {
                                        numeroNewVertice=numeroNewVertice+1;
                                        nuoviVertici.push_back(fromv);
                                        _points.push_back(from);
                                    }
                                    else
                                    {
                                       fromv=GiustoVertice(fromv);
                                    }

                                    appoggio.push(tov);
                                    appoggio.push(fromv);
                                }
                            }
                            if (contrS==2)
                            {
                                tov=Vertex(numeroNewVertice,cutter.GetTo());
                                if (SearchVertici(tov)==false)
                                {
                                    numeroNewVertice=numeroNewVertice+1;
                                    nuoviVertici.push_back(tov);
                                    _points.push_back(to);
                                }
                                else
                                {
                                    tov=GiustoVertice(tov);
                                }
                                appoggio.push(tov);
                            }
                            if (contrS==1)
                            {
                                fromv=Vertex(numeroNewVertice,cutter.GetFrom());
                                if (SearchVertici(fromv)==false)
                                {
                                    numeroNewVertice=numeroNewVertice+1;
                                    nuoviVertici.push_back(fromv);
                                    _points.push_back(from);
                                }
                                else
                                {
                                    fromv=GiustoVertice(fromv);
                                }
                                appoggio.push(fromv);
                            }
                            i=idContr+1; //cosi salta il to del lato con intersezione parallela
                            int b=i+1;
                            Vertex c=oldP.GetVertex(b); //se il vertice dopo il to non è dalla stessa parte il to verrà controllato in un altro giro
                            if (cutter.SidePoint(c.GetPoint())==side) //altrimenti è da pushare qui
                                appoggio.push(oldP.GetVertex(i));
                        }

                    }

                }
            }
            if (casoParticolare!=1) //  uscito dal ciclo interno pusho  il poligono
            {
                _polygons.push_back(Polygon(appoggio));
                for (int k=0; k<appoggio.size(); k++)
                    appoggio.pop();
                inseritiAppoggio.clear();
                j=-1; //il ciclo esterno riparte da 0
                ciclo=0;
            }
            casoParticolare=0;
        }

    }
}


int ConcaveCutter::GiustoSegmento(const Segment &intersecato, const Polygon &oldP, const Point &intersezione) //serve per spostarsi nella giusta direzione sul cutter dopo aver trovato una prima intersezione
{
    Point intersezioneGiusta;
    int entrato;
    Point intersezioneContr;
    Vertex trovato;
    double distanza=-1;
    double distanzaContr;
    Segment controlloS;
    for (int i=0; i<oldP.NumberVertices(); i++)
    {
        controlloS=oldP.GetEdge(i); //controllo ogni lato
        if (intersecato.SidePoint(controlloS.GetFrom())==Segment::Left || intersecato.SidePoint(controlloS.GetTo())==Segment::Left  ) // se nessuno dei due a sinistra lascio stare tanto è escluso
        {
            intersector.SetCutter(_cut);
            intersector.SetEdge(controlloS);
            intersector.ComputeIntersection();
            if (intersector.PositionIntersectionEdge()!=Intersector::End && intersector.TypeIntersection()!=Intersector::IntersectionParallelOnLine && intersector.TypeIntersection()!=Intersector::IntersectionParallelOnSegment && intersector.TypeIntersection()!= Intersector::NoIntersection) // controlla che ci sia intersezione, caso  end loescludiamosempre
            {
                intersezioneContr = Point(intersector.IntersectionEdgeParametricCoordinate(controlloS));
                distanzaContr=intersezioneContr.ComputeNorm2(intersezione);
                entrato=1;
                if (intersecato.SidePoint(intersezioneContr)==Segment::Right) //se  l'intersezione è a destra non è l'intersezione  giusta
                    entrato=0;
            }
            else if (intersector.TypeIntersection()==Intersector::IntersectionParallelOnLine || intersector.TypeIntersection()==Intersector::IntersectionParallelOnSegment) // caso parallelo
            {
                intersezioneContr=controlloS.GetFrom();
                distanzaContr=intersezioneContr.ComputeNorm2(intersezione);
                entrato=1;
                if (intersecato.SidePoint(intersezioneContr)==Segment::Right)
                    entrato=0;
            }
            if (distanza==-1 && entrato==1 && distanzaContr!=0) // se distanzaContr è 0 vuol dire che è la stessa gia trovata e non m i serve
            {
                intersezioneGiusta = intersezioneContr;
                distanza=distanzaContr;
                trovato=oldP.GetVertex(i);
            }
            if (distanzaContr<distanza && distanzaContr !=0)
            {
                intersezioneGiusta = intersezioneContr;
                distanza=distanzaContr;
                trovato=oldP.GetVertex(i);

            }


        }
    }
    return trovato.Id;
}

bool ConcaveCutter::SearchInseriti(const int &id) // vertice gia inserito da qualche parte?
{
    for(list<int>::const_iterator it=inseriti.begin(); it!= inseriti.end();it++)
    {
        const int & element= *it;
        if (element==id)
            return  true;
    }
    return false;
}

bool ConcaveCutter::SearchInseritiAppoggio(const int &id) //vertice gia inserito nel poligono in costruzione
{
    for(list<int>::const_iterator it=inseritiAppoggio.begin(); it!= inseritiAppoggio.end();it++)
    {
        const int& element= *it;
        if (element==id)
            return  true;
    }
    return false;
}

Vertex ConcaveCutter::GiustoVertice(const Vertex &controllo) //mi ritorna il vertice con il suo giusto Id
{
    for(list<Vertex>::const_iterator it=nuoviVertici.begin(); it!= nuoviVertici.end();it++)
    {
        const Vertex& element= *it;
        if (element.GetPoint()==controllo.GetPoint())
            return  element;
    }
    return Vertex(-1,Point());
}

bool ConcaveCutter::SearchVertici(const Vertex &controllo) //controlla se due vertici hanno stesso punto e diverso Id e in  caso usa quella sopra
{
    for(list<Vertex>::const_iterator it=nuoviVertici.begin(); it!= nuoviVertici.end();it++)
    {
        const Vertex& element= *it;
        if (element.GetPoint()==controllo.GetPoint())
            return  true;
    }
    return false;
}

bool ConcaveCutter::SearchPolygon(const Polygon &poligono) //cerca  un poligono tra quelli tagliati
{
    for(list<Polygon>::const_iterator it=_polygons.begin(); it!= _polygons.end();it++)
    {
        const Polygon& element= *it;
        if (element==poligono)
            return  true;
    }
    return false;
}




int ConcaveCutter::ControlloSegmento(const Segment &intersecato1, const Segment &intersecato2, const Segment &cutter)  //fa controllo se inserire  to  e from
{
    Point s1=cutter.GetFrom();
    Point s2=cutter.GetTo();
    if (intersecato1.SidePoint(s1)==Segment::Left && intersecato2.SidePoint(s1)==Segment::Left )
    {
        if (intersecato1.SidePoint(s2)==Segment::Left && intersecato2.SidePoint(s2)==Segment::Left )
            return 3;// sia s1 che s2 da inserire
        else
            return 1; //solo s1 da inserire
    }
    if (intersecato1.SidePoint(s2)==Segment::Left && intersecato2.SidePoint(s2)==Segment::Left )
        return 2; //solo s2 da inserire
    else
        return 0; // nessuno da inserire
}

int ConcaveCutter::ControlloSegmento2(const Segment &lato,const Segment &cutter) // fa la stessa cosa per nel caso parallelo
{
    Point s1=cutter.GetFrom();
    Point s2=cutter.GetTo();
    if (lato.SidePoint(s1)==Segment::In && lato.SidePoint(s2)==Segment::In)
        return 3;
    else if (lato.SidePoint(s1)==Segment::In) {
        return 1;
    }
    else if(lato.SidePoint(s2)==Segment::In){
        return 2;
    }
    return 0;
}

}


