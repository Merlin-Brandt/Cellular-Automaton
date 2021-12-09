EXECUTABLE=cells
SOURCES=$(wildcard src/*.c)
HEADERS=$(wildcard src/*.h)
CC= gcc
CFLAGS=-xc -std=gnu99 `pkg-config --cflags glfw3`
LDFLAGS=`pkg-config --static --libs glfw3` -lGLEW -lGLU -lGL

OBJECTS_TMP=$(SOURCES:.cpp=.o)
OBJECTS=$(OBJECTS_TMP:.c=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o$(EXECUTABLE) $(OBJECTS) $(LDFLAGS)
	
%.o: %.cpp $(HEADERS)
	$(CC) -o$@ -c $< $(CFLAGS)

clean:
	mkdir -p backup
	cp -tbackup $(OBJECTS) $(EXECUTABLE) $(SOURCES)
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
	
backup:
	mkdir -p backup
	cp -tbackup $(OBJECTS) $(EXECUTABLE) $(SOURCES)

ycm-extra-config:
	echo "flags = ['$(CFLAGS)']" > .ycm_extra_conf.py
	chmod +x .ycm_extra_conf
