
CC		= g++
C		= C

CFLAGS		= -g -Wall -fopenmp
LFLAGS		= -g -fopenmp

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lMagick++ -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/
  endif
endif

HFILES	= Shape.h Vector.h Camera.h Renderer.h Image.h PropertyVolume.h Operations.h ScalarVolumeXform.h
OFILES  = Camera.o Image.o Renderer.o

PROJECT		= figure

${PROJECT}:	${PROJECT}.o ${OFILES}
	${CC} ${LFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS}

${PROJECT}.o:	${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

Camera.o: Camera.${C} Camera.h Vector.h
	${CC} ${CFLAGS} -c Camera.${C}

Image.o: Image.${C} Image.h
	${CC} ${CFLAGS} -c Image.${C}

Renderer.o: Renderer.${C} Renderer.h Vector.h Color.h Image.h Camera.h Volume.h PropertyVolume.h
	${CC} ${CFLAGS} -c Renderer.${C}

clean:
	# rm -f core.* *.o *~ ${PROJECT}
	rm -rf *.o *~
