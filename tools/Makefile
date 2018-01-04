
include ../Makefile.include

objs := $(notdir $(TOOL_OBJS))
DEPENDS := $(objs:.o=.d)

%.o: %.cc
	$(CXX) -MMD -c -o $@ $<

all: $(objs)
	@echo $(objs)

clean:
	rm *.o *.d


-include $(DEPENDS)


