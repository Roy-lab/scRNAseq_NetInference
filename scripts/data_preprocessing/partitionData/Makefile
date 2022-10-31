LFLAG = -lgsl -lgslcblas 
SRC = partitionData.C

CC=g++
CFLAGS = -g

makePartitions: $(SRC)
	$(CC) $(SRC) $(LFLAG) $(CFLAGS) -o makePartitions

clean:
	rm -f makePartitions *~
