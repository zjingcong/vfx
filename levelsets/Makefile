C	= C
CC = g++

CFLAGS		= -g -Wall -O2 -fopenmp

DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj

LDFLAGS =  -L/usr/local/lib -L/group/dpa/local/openvdb/lib -lOpenImageIO -lHalf -ltbb -lopenvdb -Wl,--rpath=/group/dpa/local/openvdb/lib -fopenmp

INCLUDES = -I ./ -I/group/dpa/local/openvdb/include -I$(DIR_INC)

PROJECT	= levelsets

SOURCES = PolyModel.C Grid.C Matrix.C Camera.C Levelsets.C OIIOFiles.C Image.C Renderer.C Lighting.C levelsetsmain.C

$(PROJECT): $(SOURCES:%.$(C)=$(DIR_OBJ)/%.o)
	$(CC) $(SOURCES:%.$(C)=$(DIR_OBJ)/%.o) $(INCLUDES) $(LDFLAGS) -o $@

$(SOURCES:%.$(C)=$(DIR_OBJ)/%.o): $(DIR_OBJ)/%.o: $(DIR_SRC)/%.$(C)
	$(CC) -o $@ -c $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f $(DIR_OBJ)/*.o *~ ${PROJECT}

