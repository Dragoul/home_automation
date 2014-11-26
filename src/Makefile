
CXX:=		c++
CXXFLAGS:=	-I.

TARGET:=		light

SRC:=		433.c
INC:=		
LIBS:=		-lrt



OBJ:=		$(SRC:.c=.o)
OBJ_DIR:=	obj
_OBJ:=		$(patsubst %,$(OBJ_DIR)/%,$(OBJ))

all: $(TARGET)

debug:
	@echo "obj=$(OBJ)"
	@echo "_obj=$(_OBJ)"
	@echo "OBJ_DIR=$(OBJ_DIR)"

$(OBJ_DIR)/%.o: %.c $(INC)
	mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(_OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)



.PHONY: clean

clean:
	rm -rf $(OBJ_DIR) *~ $(TARGET)