


#########
# Makefile to build parts of the Turk on Linux systems.
# That's right, Windows sucks in every way.
########


include Makefile.include

DEPENDS := $(OBJDIR)/terrain.d

.PHONY: all clean doc

all: $(BINS)

clean:
	rm $(TOOL_OBJS) $(BINS) $(OBJDIR)/*.o $(OBJDIR)/*.d

doc:
	doxygen Turk.doxy


$(TOOL_OBJS): %.o: %.cc
	$(CXX) -MMD -c -o $@ $<  -llapack

$(OBJDIR)/terrain: standalone/terrain.cc $(TOOL_OBJS)
	$(CXX) -MMD -I $(INCDIR) -o $@ $^ -llapack

-include $(DEPENDS) 
