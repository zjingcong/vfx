
CC		= g++
C		= C

CFLAGS		= -g -Wall `Magick++-config --cppflags`
LFLAGS		= -g `Magick++-config --ldflags`

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lMagick++ -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGL -lMagick++ -lGLU -lOpenImageIO -lm
  endif
endif

HFILES	= Volume.h Shape.h Vector.h Camera.h Renderer.h Color.h
OFILES  = Camera.o

PROJECT		= figure

${PROJECT}:	${PROJECT}.o 
	${CC} ${LFLAGS} -o ${PROJECT} ${PROJECT}.o ${LDFLAGS}

${PROJECT}.o:	${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

Camera.o: Camera.${C} Camera.h
	${CC} ${CFLAGS} -c Camera.${C}

clean:
	rm -f core.* *.o *~ ${PROJECT}
