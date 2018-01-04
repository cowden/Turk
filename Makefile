


#########
# Makefile to build parts of the Turk on Linux systems.
# That's right, Windows sucks in every way.
########


include Makefile.include

DEPENDS := $(OBJDIR)/terrain.d

.PHONY: all clean

all: $(BINS)


clean:
	rm $(TOOL_OBJS) $(BINS) $(OBJDIR)/*.o $(OBJDIR)/*.d


$(TOOL_OBJS): %.o: %.cc
	$(CXX) -MMD -c -o $@ $<

$(OBJDIR)/terrain: standalone/terrain.cc $(TOOL_OBJS)
	$(CXX) -MMD -I $(INCDIR) -o $@ $^

-include $(DEPENDS) 
