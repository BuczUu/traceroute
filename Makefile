# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Nazwa pliku wykonywalnego
TARGET = program

# Pliki źródłowe
SRCS = main.cpp traceroute.cpp icmp_packet.cpp icmp_receiver.cpp

# Pliki obiektowe (generowane automatycznie)
OBJS = $(SRCS:.cpp=.o)

# Reguła domyślna
all: $(TARGET)
# //g++ main.cpp traceroute.cpp icmp_packet.cpp icmp_receiver.cpp -o traceroute -std=c++17 -Wall -Wextra

# Kompilacja pliku wykonywalnego
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Reguła dla plików obiektowych
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Czyszczenie plików obiektowych
clean:
	rm -f $(OBJS)

# Czyszczenie plików obiektowych i wykonywalnego
distclean: clean
	rm -f $(TARGET)

# Phony targets (nie są plikami)
.PHONY: all clean distclean