# Credit for the Auto-dependecy generation portion
# of this file goes to Tom Tromey <tromey@cygnus.com>.
# This file is based on a post that focused on
# this method. The post can be found at:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Modifications to the original Makefile are contributed by
#  Zachary Sullivan, Fall 2017

CXX = g++

DEPDIR := .deps
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(basename $*).Td

CXXFLAGS = `sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lSDL2_image -lexpat -lSDL2_ttf -lSDL2_mixer

SRCFLS = $(shell find . -regex ".*\.cpp" -printf "%f ")
SRCDIR = $(shell find . -regex ".*\.cpp" -printf "%h\n" | sort -u | tr '\n' ':')

OBJDIR  = build
$(shell mkdir -p $(OBJDIR) >/dev/null)
OBJFLS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFLS))

EXEC = run

VPATH = $(SRCDIR)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	@echo "Creating Object $(notdir $@)"
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

$(EXEC): $(OBJFLS)
	@echo "Creating Executable $@"
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJFLS) $(LDFLAGS)

clean:
	@echo "Deleting Objects"
	@rm -rf $(OBJFLS)
	@echo "Deleting Executable"
	@rm -rf $(EXEC)
	@echo "Deleting Dependecy List"
	@rm -rf $(DEPDIR)
	@echo "Deleting Images"
	@rm -rf frames/*.bmp

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(notdir $(basename $(OBJFLS)))))
