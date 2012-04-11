HDF_DIR = /opt/local
HDF_INCLUDE = /opt/local/include
X11_DIR = /usr/X11R6
PRECISION = r8

COMPILER="gnu"
DEBUG="no"
OPENMP="no"

ifeq ($(COMPILER), "gnu")
CC = gcc-mp-4.6
CCxx = g++-mp-4.6
LDR = g++-mp-4.6
ifeq ($(DEBUG), "yes")
CFLAGS = -O0
LDFLAGS = -O0
else
CFLAGS = -O3
LDFLAGS = -O3
endif
ifeq ($(OPENMP),"yes")
CFLAGS += -fopenmp
LDFLAGS += -fopenmp
endif
endif

ifeq ($(COMPILER), "intel")
CC = icc
CCxx = icpc
LDR = icpc
ifeq ($(DEBUG), "yes")
CFLAGS = -O0
LDFLAGS = -O0
else
CFLAGS = -fast
LDFLAGS = -fast
endif
ifeq ($(OPENMP),"yes")
CFLAGS += -openmp
LDFLAGS += -openmp
endif
endif

DEFINES =

INCLUDE = -I. -I$(HDF_INCLUDE) `sdl-config --cflags` -I$(X11_DIR)/include

LIBS = -L$(HDF_DIR)/lib `sdl-config --libs` \
	-lhdf5 -lmfhdf -ldf -ljpeg -lz

OBJS =	AddLevel.o			\
	CreateNewRays.o			\
	DefineFieldNames.o		\
	FieldsToLoad.o			\
	FindField.o			\
	GetGrid.o			\
	Grid_ComputeVertexData.o	\
	Grid_FindNeighbors.o		\
	Grid_LoadDataHDF4.o		\
	Grid_LoadDataHDF5.o		\
	Grid_ReadGridProperties.o	\
	Grid_SetSubgridMarker.o		\
	Grid_constructor.o		\
	Grid_destructor.o		\
	InterpretKeyPress.o		\
	LoadCT.o			\
	Load_RGBA.o			\
	ListIO.o			\
	Ray_GridBoundaryCheck.o		\
	Ray_Initialize.o		\
	Ray_InterpolateValue.o		\
	Ray_Trace.o			\
	Ray_constructor.o		\
	RayTraceInteractive.o		\
	ReadGridInfo.o			\
	ReadHierarchyEntry.o		\
	ResetReloadFlag.o		\
	UnloadAllData.o			\
	UnloadGrids.o			\
	UpdateScreen.o			\
	TraceAllRays.o			\
	camera.o			\
	my_vector.o			\
	print_help.o

#	RenderFunctions.o		\

.C.o:
	$(CCxx) $(DEFINES) $(INCLUDE) -D$(PRECISION) -g -c $(CFLAGS) $*.C

iTrace: $(OBJS)
	$(LDR) $(LDFLAGS) $(LIBS) -o iTrace $(OBJS)

clean:
	rm -f *~ *.o iTrace

${OBJS}: typedef.h Grid.h my_vector.h Ray.h precision.h colors.h render.h
