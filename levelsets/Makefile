C	= C
CC = g++

CFLAGS		= -g -Wall -O2 -fopenmp

LDFLAGS =  `Magick++-config  --cppflags --cxxflags --ldflags --libs` -L/usr/local/lib -L/group/dpa/local/openvdb/lib -lOpenImageIO -lHalf -ltbb -lopenvdb -Wl,--rpath=/group/dpa/local/openvdb/lib -fopenmp

INCLUDES = -I ./ `Magick++-config  --cppflags --cxxflags` -I/group/dpa/local/openvdb/include

PROJECT	= levelsets

SOURCES = PolyModel.C Grid.C Matrix.C Camera.C Levelsets.C OIIOFiles.C Image.C Renderer.C Lighting.C levelsetsmain.C

$(PROJECT): $(SOURCES:%.$(C)=%.o)
	$(CC) $(SOURCES:%.$(C)=%.o) $(INCLUDES) $(LDFLAGS) -o $@

$(SOURCES:%.$(C)=%.o): %.o: %.$(C)
	$(CC) -o $@ -c $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f core.* *.o *~ ${PROJECT}

