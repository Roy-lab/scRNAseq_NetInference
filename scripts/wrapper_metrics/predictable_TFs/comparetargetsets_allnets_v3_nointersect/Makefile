LFLAG = -lgsl 
SRC = Framework.C Clique.C Distance.C GeneNameMapper.C Graph.C HyperGeomPval.C Vertex.C

CC=g++
CFLAGS = -g

BIN=compareTgtSetAcrossDataset
$(BIN) : $(SRC)
	$(CC) $(SRC) $(LFLAG) $(CFLAGS) -o $(BIN)

clean:
	rm -f $(BIN) *~
