CXX?=g++
STDVER?=c++98
WARNFLAGS = -Wall -Wextra -pedantic -Wno-long-long
CXXOPTS = -I../HippoMocks/ $(WARNFLAGS) -g
TARGET = $(PREFIX)test.exe 

OBJECTS = $(patsubst %,$(PREFIX)%,is_virtual.o test.o test_args.o test_array.o test_autoptr.o test_cfuncs.o test_class_args.o test_constref_params.o test_cv_funcs.o test_do.o test_dontcare.o test_except.o test_exception_quality.o test_filter.o test_inparam.o test_membermock.o test_mi.o test_nevercall.o test_optional.o test_outparam.o test_overload.o test_ref_args.o test_regression_arg_count.o test_retval.o test_transaction.o test_zombie.o Framework.o main.o)

all: $(TARGETS)

clean:
	$(RM) $(OBJECTS) $(TARGETS) $(OBJECTS:%.o=%.d)

runtest: $(TARGET)
	./$(TARGET)

%.o: %.cpp makefile
	$(CXX) $(CFLAGS) $(CXXOPTS) -std=$(STDVER) -c -o $@ $< -MMD -MP

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) $(CXXOPTS) -std=$(STDVER) -o $@ $^

-include $(OBJECTS:%.o=%.d)


