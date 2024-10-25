import src.polygon as polygon
import src.intersector as intersector
import src.segment as segment
import src.point as point
import numpy as np
import src.vertex as vertex

class ICutter:
    def cutPolygon(self, points, polygonVertices, segment):
        pass
    def setCutter(self, points, vertex, segment):
        pass
    def setTolerance(self, toll):
        pass
    def getCut(self):
        pass
    def getTolerance(self):
        pass



class ConvexCutter(ICutter):
    def __init__(self):
       self._points = [] #creando un vettore di punti vuoto invece della lista
       self._polygons = []
       self._cut = segment.Segment()
       self._tolerance = 1.0E-7
       self.rightSide = []
       self.leftSide = []
       self.controll = []
       self.intersector = intersector.Intersector()
       self.cambi = 0


    def setCutter(self, points, vertex, segment):
        self._points.append(points)
        poligono=polygon.Polygon(vertex)
        self._polygons.append(poligono) #aggiungo nel vettore di poligoni un nuovo poligono formato dai vertici che sono dentro vertex
        self._cut = segment
    def setTolerance(self, toll):
        self._tolerance = toll
    def getTolerance(self):
        return self._tolerance
    def getCut(self):
        return self._cut
    def getPolygon(self, i):
        return self._polygons[i]
    def cutPolygon(self, points, polygonVertices, cut):
        self.setCutter(points, polygonVertices, cut)
        oldP = self.getPolygon(0)
        self.cambi = 0 #dopo due self.cambi nel caso convesso non si controlla piu, se self.cambi è uguale a 3 siamo nel caso cutter coincidente a un lato
        intersection = point.Point()
        intersectionV = vertex.Vertex()
        vert = vertex.Vertex()
        self.numberNewVertices =oldP.numberVertices()
        intersected =segment.Segment()
        cutter = self.getCut()
        fromcutter = cutter.getFrom()
        tocutter = cutter.getTo()
        tov= vertex.Vertex()
        fromv = vertex.Vertex()
        a = 0
        i=0
        while i<oldP.numberVertices() and self.cambi < 2:  #metto while perchè con 2 condizioni è meglio
            a=i
            vert = oldP.getVertex(i)
            if self.cambi ==0:
                self.leftSide.append(vert)
            else:
                self.rightSide.append(vert)
            intersected =oldP.getEdge(i)
            self.intersector.setCutter(cutter)
            self.intersector.setEdge(intersected)
            self.intersector.computeIntersection()
            if self.intersector.getTypeIntersection() == intersector.Type.IntersectionOnSegment or self.intersector.getTypeIntersection()== intersector.Type.IntersectionOnLine: #c'è intersezione
                if self.intersector.positionIntersectionEdge() != intersector.Position.End and self.intersector.positionIntersectionEdge() != intersector.Position.Begin: #intersezione sta  in mezzo
                    intersection = point.Point()
                    intersection.setvectorcoordinate(self.intersector.IntersectionEdgeParametricCoordinate(intersected))
                    intersectionV = vertex.Vertex()
                    intersectionV.setPoint(self.numberNewVertices,intersection)
                    self._points.append(intersection)
                    self.numberNewVertices = self.numberNewVertices+1 #il prossimo vertice nuovo avrà questo id
                    self.leftSide.append(intersectionV) #la pusho qui
                    self.cambi= self.cambi+1 #mi segnalo il self.cambio
                    distanceFrom= intersection.computeNorm2(fromcutter)
                    distanceTo= intersection.computeNorm2(tocutter) #controlli per pushare from e to se sono interni al poligono
                    if oldP.insidePoint(fromcutter) and oldP.insidePoint(tocutter) and self.cambi ==1:
                        if distanceFrom<distanceTo:
                             # le due righe invece di fromv= vertex.Vertex(self.numberNewVertices,fromcutter)
                            fromv.setPoint(self.numberNewVertices, fromcutter)
                            self.leftSide.append(fromv)
                            self.numberNewVertices = self.numberNewVertices+1
                            tov.setPoint(self.numberNewVertices, tocutter)
                            self.leftSide.append(tov)
                            self._points.append(fromcutter) #append è il pushback
                            self._points.append(tocutter)
                            self.numberNewVertices = self.numberNewVertices + 1
                            self.rightSide.append(tov)
                            self.rightSide.append(fromv)
                        else:
                            tov.setPoint(self.numberNewVertices, tocutter)
                            self.leftSide.append(tov)
                            self.numberNewVertices = self.numberNewVertices+1
                            fromv.setPoint(self.numberNewVertices,fromcutter)
                            self.leftSide.append(fromv)
                            self.numberNewVertices =self.numberNewVertices+1
                            self._points.append(fromcutter)
                            self._points.append(tocutter)
                            self.rightSide.append(fromv) #append è il push
                            self.rightSide.append(tov)
                    elif self.cambi == 1 and oldP.insidePoint(fromcutter):
                        fromv.setPoint(self.numberNewVertices,fromcutter)
                        self._points.append(fromcutter)
                        self.leftSide.append(fromv)
                        self.rightSide.append(fromv)
                        self.numberNewVertices = self.numberNewVertices + 1
                    elif self.cambi == 1 and oldP.insidePoint(tocutter):
                        tov.setPoint(self.numberNewVertices, tocutter)
                        self._points.append(tocutter)
                        self.leftSide.append(tov)
                        self.rightSide.append(tov)
                        self.numberNewVertices = self.numberNewVertices + 1
                    self.rightSide.append(intersectionV)  #la  pusho di la
            if self.intersector.getTypeIntersection()== intersector.Type.IntersectionParallelOnLine or self.intersector.getTypeIntersection()== intersector.Type.IntersectionParallelOnSegment: #casi paralleli
                self.cambi = 10
                if oldP.insidePoint(fromcutter) and oldP.insidePoint(tocutter): #controlli per from e to
                    distanceFrom = vert.getPoint().computeNorm2(fromcutter)
                    distanceTo = vert.getPoint().computeNorm2(tocutter)
                    if distanceFrom < distanceTo:
                        v= vertex.Vertex()
                        self.leftSide.append(v.setPoint(self.numberNewVertices,fromcutter))
                        self.numberNewVertices = self.numberNewVertices + 1
                        v= vertex.Vertex()
                        self.leftSide.append(v.setPoint(self.numberNewVertices, tocutter))
                        self.numberNewVertices = self.numberNewVertices + 1
                        self._points.append(fromcutter)
                        self._points.append(tocutter)
                    else:
                        v = vertex.Vertex()
                        self.leftSide.append(v.setPoint(self.numberNewVertices, tocutter))
                        self.numberNewVertices = self.numberNewVertices + 1
                        v = vertex.Vertex()
                        self.leftSide.append(v.setPoint(self.numberNewVertices, fromcutter))
                        self.numberNewVertices = self.numberNewVertices + 1
                        self._points.append(fromcutter)
                        self._points.append(tocutter)
                elif oldP.insidePoint(fromcutter):
                    v = vertex.Vertex()
                    self.leftSide.append(v.setPoint(self.numberNewVertices, fromcutter))
                    self.numberNewVertices = self.numberNewVertices + 1
                    self._points.append(fromcutter)
                elif oldP.insidePoint(tocutter):
                    v = vertex.Vertex()
                    self.leftSide.append(v.setPoint(self.numberNewVertices, tocutter))
                    self.numberNewVertices = self.numberNewVertices + 1
                    self._points.append(tocutter)
            if self.intersector.positionIntersectionEdge() == intersector.Position.Begin: #inizio
                distanceFrom = vert.getPoint().computeNorm2(fromcutter)
                distanceTo = vert.getPoint().computeNorm2(tocutter)
                if oldP.insidePoint(fromcutter) and oldP.insidePoint(tocutter) and self.cambi == 0:
                    if distanceFrom<distanceTo:
                        fromv = vertex.Vertex()
                        fromv.setPoint(self.numberNewVertices, fromcutter)
                        self.leftSide.append(fromv)
                        self.numberNewVertices = self.numberNewVertices + 1
                        tov = vertex.Vertex()
                        tov.setPoint(self.numberNewVertices, tocutter)
                        self.leftSide.append(tov)
                        self.numberNewVertices = self.numberNewVertices + 1
                        self.rightSide.append(tov)
                        self.rightSide.append(fromv)
                        self._points.append(fromcutter)
                        self._points.append(tocutter)
                    else:
                        tov = vertex.Vertex()
                        tov.setPoint(self.numberNewVertices, tocutter)
                        self.leftSide.append(tov)
                        self.numberNewVertices = self.numberNewVertices + 1
                        fromv = vertex.Vertex()
                        fromv.setPoint(self.numberNewVertices, fromcutter)
                        self.leftSide.append(fromv)
                        self.numberNewVertices = self.numberNewVertices + 1
                        self.rightSide.append(fromv)
                        self.rightSide.append(tov)
                        self._points.append(fromcutter)
                        self._points.append(tocutter)

                elif self.cambi == 0 and oldP.insidePoint(fromcutter):
                    fromv = vertex.Vertex()
                    fromv.setPoint(self.numberNewVertices, fromcutter)
                    self.leftSide.append(fromv)
                    self.numberNewVertices = self.numberNewVertices + 1
                    case = 3
                    self._points.append(fromcutter)
                elif self.cambi == 0 and oldP.insidePoint(tocutter):
                    tov = vertex.Vertex()
                    tov.setPoint(self.numberNewVertices, tocutter)
                    self.leftSide.append(tov)
                    self.numberNewVertices = self.numberNewVertices + 1
                    case = 4
                    self._points.append(tocutter)
                if self.cambi == 0:
                    self.rightSide.append(vert)
                else:
                    self.leftSide.append(vert)
                self.cambi = self.cambi+1
            i=i+1

        a = a+1
        k=a
        while k<oldP.numberVertices():
            vert =oldP.getVertex(k)
            self.leftSide.append(vert)
            k=k+1
        self.setPolygon()

    def setPolygon(self):  #attenzione se devo togliere qualcosa qua non toglie, aggiunge solo
        if self.cambi == 0:
            poligono=polygon.Polygon(self._polygons[0])
            self._polygons.append(poligono)
        elif self.cambi == 2:
            poligono1=polygon.Polygon(self.rightSide)
            poligono2=polygon.Polygon(self.leftSide)
            self._polygons.append(poligono1)
            self._polygons.append(poligono2)
        elif self.cambi == 10:
            poligono=polygon.Polygon(self.leftSide)
            self._polygons.append(poligono)


class PositionSide:  #funziona come enum, classe con delle variabili
    Left=0
    Right = 1
    Forward = 2
    Behind = 3
    Begin = 4
    End = 5
    In = 6

class ConcaveCutter(ICutter):
    def __init__(self):
       self._points = [] #creando un vettore di punti vuoto invece della lista
       self._polygons = []
       self._cut = segment.Segment()
       self._tolerance = 1.0E-7
       self.inseriti = []
       self.inseritiAppoggio = []
       self.nuoviVertici = []
       self.appoggio=[]
       self.intersector = intersector.Intersector()


    def setCutter(self, points, vertex, segment):
        self._points.append(points)
        poligono=polygon.Polygon(vertex)
        self._polygons.append(poligono) # aggiungo nel vettore di poligoni un nuovo poligono formato dai vertici che sono dentro vertex
        self._cut = segment

    def setTolerance(self, toll):
        self._tolerance = toll
    def getTolerance(self):
        return self._tolerance
    def getCut(self):
        return self._cut



    def cutPolygon(self, points, polygonVertices, segment):
        self.setCutter(points, polygonVertices, segment)
        oldP = polygon.Polygon(polygonVertices)
        intersection = point.Point()
        intersectionV = vertex.Vertex()
        intersection2 = point.Point()
        intersectionV2 = vertex.Vertex()
        vert = vertex.Vertex()
        side = PositionSide
        idContr = 0
        particularCase = 0
        contrS = 0
        self.numberNewVertices = oldP.numberVertices()
        ciclo = 0


        cut = self.getCut()
        position = PositionSide
        particularPosition = PositionSide
        out = 0
        fromcutter = cut.getFrom()
        tocutter = cut.getTo()
        distanceFrom = 0.0
        distanceTo = 0.0
        tov = vertex.Vertex()
        fromv = vertex.Vertex()

        for f in range(0,oldP.numberVertices()):
            self.nuoviVertici.append(oldP.getVertex(f))
        for j in range(0,oldP.numberVertices()):
            vert =oldP.getVertex(j)
            if self.searchInseriti(j) == False:  #se vertice j non è ancora stato inserito da nessuna parte entra qui
                side = cut.sidePoint((vert.getPoint())) #mi segno la posizione
                if side != PositionSide.Left and side != PositionSide.Right: #prendo come vertici di riferimento soltanto quelli che stanno oa destra o a sinistra
                    particularCase = 1 #mi farà poi uscire da tutto il ciclo dopo
                    self.inseriti.append(j)#tutti i particolari li lascio stare quando cerco i v di riferimento
                if particularCase != 1:
                    self.appoggio.append(vert)
                    self.inseriti.append(j)
                    self.inseritiAppoggio.append(j)
                if j == (oldP.numberVertices()-1): #quando arrivo all' ultimo vertice devo poi controllare nel ciclo dopo il vertice 0
                    j=-1
                i=j+1
                while i<oldP.numberVertices() and ciclo ==0: #questo ciclo serve per costruire il poligono a cui appartiene  il vertice di rif
                    if particularCase ==1:
                        break
                    vert = oldP.getVertex(i)
                    if self.searchInseritiAppoggio(i)== True: #quando si riarriva a un  vertice  gia  inserito  nel poligono di costruzione possiamo uscire
                        ciclo = 1 #per uscire dal ciclo
                    position = cut.sidePoint(vert.getPoint())
                    if position == side and ciclo == 0: # se è  dalla stessa parte del v  di rif lo aggiungo
                       self.appoggio.append(vert)
                       self.inseriti.append(i)
                       self.inseritiAppoggio.append(i)
                    if position != side: #se non lo è cerco  di capire  che succede
                       if i==0:
                           i=oldP.numberVertices()
                       edge = oldP.getEdge(i-1)
                       self.intersector.setCutter(cut)
                       self.intersector.setEdge(edge)
                       self.intersector.computeIntersection()
                       if self.intersector.getTypeIntersection() != intersector.Type.IntersectionParallelOnLine and self.intersector.getTypeIntersection() != intersector.Type.IntersectionParallelOnSegment:
                           intersection = point.Point()
                           intersection.setvectorcoordinate(self.intersector.IntersectionEdgeParametricCoordinate(edge))
                           intersectionV = vertex.Vertex()
                           intersectionV.setPoint(self.numberNewVertices, intersection)
                           if self.searchVertex(intersectionV) == False:
                               self.numberNewVertices = self.numberNewVertices+1
                               self.nuoviVertici.append(intersectionV)
                               self._points.append(intersection)
                           else:
                               intersectionV = self.exactVertex(intersectionV)
                           #self._points.append(intersection)
                           self.appoggio.append(intersectionV)
                           distanceFrom= intersection.computeNorm2(fromcutter)
                           distanceTo = intersection.computeNorm2(tocutter)

                           idContr = self.exactSegment(edge,oldP,intersection)  #da qui parte il lato da controllare
                           edge2 = oldP.getEdge(idContr)
                           self.intersector.setCutter(cut)
                           self.intersector.setEdge(edge2)
                           self.intersector.computeIntersection()
                           if self.intersector.positionIntersectionEdge() == intersector.Position.Begin:
                               particularPosition = cut.sidePoint(edge2.getTo())
                               if (particularPosition != side): #se siamo in questo caso non inserisco niente perchè altrimenti inserisce tutto due volte ( vedi  ultimo caso)
                                   out=1
                           if self.intersector.getTypeIntersection()!=intersector.Type.IntersectionParallelOnLine and self.intersector.getTypeIntersection()!=intersector.Type.IntersectionParallelOnSegment:
                               intersection2 = point.Point()
                               intersection2.setvectorcoordinate(self.intersector.IntersectionEdgeParametricCoordinate(edge2))
                               contrS = self.controllSegment(edge,edge2,cut) #controlla per inserire  s1 e s2
                               if contrS == 3 and out == 0:
                                   if distanceFrom < distanceTo:
                                       fromv = vertex.Vertex()
                                       fromv.setPoint(self.numberNewVertices, cut.getFrom())
                                       if self.searchVertex(fromv)== False:
                                           self.numberNewVertices = self.numberNewVertices+1
                                           self.nuoviVertici.append(fromv)
                                           self._points.append(fromcutter)
                                       else:
                                           fromv= self.exactVertex(fromv)

                                       tov = vertex.Vertex()
                                       tov.setPoint(self.numberNewVertices, cut.getTo())
                                       if self.searchVertex(tov) == False:
                                            self.numberNewVertices = self.numberNewVertices + 1
                                            self.nuoviVertici.append(tov)
                                            self._points.append(tocutter)
                                       else:
                                           tov = self.exactVertex(tov)
                                       self.appoggio.append(fromv)
                                       self.appoggio.append(tov)
                                   else:
                                       tov = vertex.Vertex()
                                       tov.setPoint(self.numberNewVertices, cut.getTo())
                                       if self.searchVertex(tov) == False:
                                           self.numberNewVertices = self.numberNewVertices+1
                                           self.nuoviVertici.append(tov)
                                           self._points.append(tocutter)
                                       else:
                                           tov= self.exactVertex(tov)


                                       fromv = vertex.Vertex()
                                       fromv.setPoint(self.numberNewVertices,cut.getFrom())
                                       if self.searchVertex(fromv) == False:
                                           self.numberNewVertices = self.numberNewVertices + 1
                                           self.nuoviVertici.append(fromv)
                                           self._points.append(fromcutter)
                                       else:
                                           fromv= self.exactVertex(fromv)
                                       self.appoggio.append(tov)
                                       self.appoggio.append(fromv)

                               if contrS == 2 and out==0:
                                    tov = vertex.Vertex()
                                    tov.setPoint(self.numberNewVertices, cut.getTo())
                                    if self.searchVertex(tov) == False:
                                        self.numberNewVertices = self.numberNewVertices+1
                                        self.nuoviVertici.append(tov)
                                        self._points.append(tocutter)
                                    else:
                                        tov = self.exactVertex(tov)
                                    self.appoggio.append(tov)
                               if contrS ==1 and out == 0:
                                   fromv = vertex.Vertex()
                                   fromv.setPoint(self.numberNewVertices, cut.getFrom())
                                   if self.searchVertex(fromv) == False:
                                       self.numberNewVertices = self.numberNewVertices + 1
                                       self.nuoviVertici.append(fromv)
                                       self._points.append(fromcutter)
                                   else:
                                       fromv = self.exactVertex(fromv)
                                   self.appoggio.append(fromv)

                               intersectionV2 = vertex.Vertex()
                               intersectionV2.setPoint(self.numberNewVertices,intersection2)
                               if self.searchVertex(intersectionV2) == False and out ==0:
                                   self.numberNewVertices = self.numberNewVertices + 1
                                   self.nuoviVertici.append(intersectionV2)
                                   self._points.append(intersection2)
                               elif out ==0:
                                   intersectionV2 = self.exactVertex(intersectionV2)
                               if out == 0:
                                   self.appoggio.append(intersectionV2)
                               i = idContr
                               out = 0
                           elif self.intersector.getTypeIntersection() == intersector.Type.IntersectionParallelOnLine or self.intersector.getTypeIntersection() == intersector.Type.IntersectionParallelOnSegment:
                               vertexPar = oldP.getVertex(idContr)
                               intersectionPar = vertexPar.getPoint()
                               self.appoggio.append(vertexPar)
                               contrS = self.controllSegment2(edge2,cut)
                               if (contrS==3):
                                   if distanceFrom<distanceTo:
                                       fromv = vertex.Vertex()
                                       fromv.setPoint(self.numberNewVertices,cut.getFrom())
                                       if self.searchVertex(fromv)== False:
                                           self.numberNewVertices = self.numberNewVertices + 1
                                           self.nuoviVertici.append(fromv)
                                           self._points.append(fromcutter)
                                       else:
                                           fromv = self.exactVertex(fromv)
                                       self.appoggio.append(fromv)
                                       tov = vertex.Vertex()
                                       tov.setPoint(self.numberNewVertices, cut.getTo())
                                       if self.searchVertex(tov)== False:
                                           self.numberNewVertices = self.numberNewVertices + 1
                                           self.nuoviVertici.append(tov)
                                           self._points.append(tocutter)
                                       else:
                                           tov = self.exactVertex(tov)
                                       self.appoggio.append(tov)
                                   else:
                                       tov = vertex.Vertex()
                                       tov.setPoint(self.numberNewVertices, cut.getTo())
                                       if self.searchVertex(tov) == False:
                                           self.numberNewVertices = self.numberNewVertices + 1
                                           self.nuoviVertici.append(tov)
                                           self._points.append(tocutter)
                                       else:
                                           tov = self.exactVertex(tov)
                                       fromv = vertex.Vertex()
                                       fromv.setPoint(self.numberNewVertices, cut.getFrom())
                                       if self.searchVertex(fromv) == False:
                                           self.numberNewVertices = self.numberNewVertices + 1
                                           self.nuoviVertici.append(fromv)
                                           self._points.append(fromcutter)
                                       else:
                                           fromv = self.exactVertex(fromv)
                                       self.appoggio.append(tov)
                                       self.appoggio.append(fromv)
                               if (contrS == 2):
                                  tov = vertex.Vertex()
                                  tov.setPoint(self.numberNewVertices, cut.getTo())
                                  if self.searchVertex(tov) == False:
                                    self.numberNewVertices = self.numberNewVertices + 1
                                    self.nuoviVertici.append(tov)
                                    self._points.append(tocutter)
                                  else:
                                      tov = self.exactVertex(tov)
                                      self.appoggio.append(tov)
                               if contrS == 1:
                                 fromv = vertex.Vertex()
                                 fromv.setPoint(self.numberNewVertices, cut.getFrom())
                                 if self.searchVertex(intersectionV) == False:
                                   self.numberNewVertices = self.numberNewVertices + 1
                                   self.nuoviVertici.append(fromv)
                                   self._points.append(fromcutter)
                                 else:
                                     fromv = self.exactVertex(fromv)
                                 self.appoggio.append(fromv)
                               i=idContr+1
                               b=i+1
                               c=oldP.getVertex(b)
                               if (cut.sidePoint(c.getPoint())==side):
                                   self.appoggio.append(oldP.getVertex(i))


                    i=i+1 #fine while interno

                if particularCase != 1: #uscito dal ciclo interno pusho  il poligono
                    p = polygon.Polygon(self.appoggio)
                    self._polygons.append(p)
                    for i in range(0,len(self.appoggio)):
                        self.appoggio.pop()
                    self.inseritiAppoggio.clear()
                    j=-1
                    ciclo=0
                particularCase =0


    def exactSegment(self, intersected,oldP, intersection): #serve per spostarsi nella giusta direzione sul cutter dopo aver trovato una prima intersezione
        exactIntersection = point.Point()
        entered = 0
        intersectionContr = point.Point()
        find = vertex.Vertex()
        distance =-1
        distanceContr = 0.0
        i=0
        while i<oldP.numberVertices():
            controllS= oldP.getEdge(i) #controllo ogni lato
            if intersected.sidePoint(controllS.getFrom()) == segment.PositionSide.Left or intersected.sidePoint(controllS.getTo()) == segment.PositionSide.Left:
                self.intersector.setCutter(self._cut)
                self.intersector.setEdge(controllS)
                self.intersector.computeIntersection()
                if self.intersector.positionIntersectionEdge()!= intersector.Position.End and self.intersector.getTypeIntersection() != intersector.Type.IntersectionParallelOnLine and self.intersector.getTypeIntersection() != intersector.Type.IntersectionParallelOnSegment and self.intersector.getTypeIntersection() != intersector.Type.NoIntersection:
                    intersectionContr = point.Point()
                    intersectionContr.setvectorcoordinate(self.intersector.IntersectionEdgeParametricCoordinate(controllS))
                    distanceContr = intersectionContr.computeNorm2(intersection)
                    entered =1
                    if intersected.sidePoint(intersectionContr) == PositionSide.Right:
                        entered =0
                if self.intersector.getTypeIntersection() == intersector.Type.IntersectionParallelOnLine or self.intersector.getTypeIntersection() == intersector.Type.IntersectionParallelOnSegment: #caso parallelo
                    intersectionContr=controllS.getFrom() #prendo l'inizio del segmento, questo è un punto
                    distanceContr = intersectionContr.computeNorm2(intersection) #calcolo la norma 2 tra i 2 punti
                    entered = 1
                    if intersected.sidePoint(intersectionContr) == PositionSide.Right:
                        entered = 0
                if distance == -1 and entered == 1 and distanceContr != 0:
                    exactIntersection = intersectionContr
                    distance =distanceContr
                    find =oldP.getVertex(i)

                if distanceContr < distance and distanceContr != 0:
                    exactIntersection = intersectionContr
                    distance = distanceContr
                    find = oldP.getVertex(i)
                i=i+1
            else:
                i=i+1
        return find.Id

    def searchInseriti(self, id): #è un booleano
        for it in range(0,len(self.inseriti)):
            if self.inseriti[it] == id:
                return True
        return False

    def searchInseritiAppoggio(self, id):  # è un booleano
        for it in range(0,len(self.inseritiAppoggio)):
            if self.inseritiAppoggio[it] == id: #it va avanti e io verifico se dentro it è = a id
                return True
        return False


    def exactVertex(self, controll): #mi ritorna il vertice con il suo giusto Id
        for it in range (0,len(self.nuoviVertici)):
            if self.nuoviVertici[it].getPoint() == controll.getPoint():  # it va avanti e io verifico se dentro it è = a id
                return self.nuoviVertici[it]
        v= vertex.Vertex()
        return v.setPoint(-1,point.Point())


    def searchVertex(self, controll): #controlla se due vertici hanno stesso punto e diverso Id e in  caso usa quella sopra
        for it in range(0,len(self.nuoviVertici)):
            if self.nuoviVertici[it].getPoint() == controll.getPoint():
                return True
        return False

    def searchPolygon(self, polygono): #cerca  un poligono tra quelli tagliati
        for it in range(0,len(self._polygons)):
            if self._polygons[it].numberVertices()==polygono.numberVertices() :
                bool = True
                for i in range (0, polygono.numberVertices()) :
                    v1 = polygono.getVertex(i)
                    v2 = self._polygons[it].getVertex(i)
                    x1=v1.getX()
                    x2=v2.getX()
                    y1=v1.getY()
                    y2=v2.getY()
                    id1=v1.Id
                    id2=v2.Id
                    if x1 != x2 or y1 != y2 or  id1 != id2 :
                        bool=False
                if bool == True:
                    return True
        return False

    def controllSegment (self,intersected1, intersected2, cut): #fa controllo se inserire to e from
        s1=cut.getFrom()

        s2=cut.getTo()
        if intersected1.sidePoint(s1)==PositionSide.Left and intersected2.sidePoint(s1)==PositionSide.Left:
            if intersected1.sidePoint(s2)==segment.PositionSide.Left and intersected2.sidePoint(s2)== PositionSide.Left:
                return 3 #sia s1 che s2 da inserire
            else:
                return 1 #solo s1 da inserire
        if intersected1.sidePoint(s2)== PositionSide.Left and intersected2.sidePoint(s2)== PositionSide.Left:
            return 2 #solo s2 da inserire
        else:
            return 0 #nessuno da inserire
    def controllSegment2(self, edge, cut): #fa la stessa cosa per nel caso parallelo
        s1=cut.getFrom()
        s2=cut.getTo()
        if edge.sidePoint(s1) == PositionSide.In and edge.sidePoint(s2) == PositionSide.In:
            return 3
        elif edge.sidePoint(s1) == PositionSide.In:
            return 1
        elif edge.sidePoint(s1) == PositionSide.In:
            return 2
        return 0
