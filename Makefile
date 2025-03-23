# Kompilator
CXX = g++
# Flagi kompilacji
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
# Nazwa pliku wykonywalnego
TARGET = traceroute
# Pliki źródłowe
SRCS = main.cpp traceroute.cpp icmp_packet.cpp icmp_receiver.cpp
# Pliki obiektowe
OBJS = $(SRCS:.cpp=.o)

# Reguła domyślna
all: $(TARGET)
	sudo setcap cap_net_raw+ep $(TARGET)  # Nadaj uprawnienia po kompilacji

# Kompilacja pliku wykonywalnego
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Kompilacja plików źródłowych do plików obiektowych
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Czyszczenie plików obiektowych i wykonywalnego
clean:
	rm -f $(OBJS) $(TARGET)

# Uruchomienie programu
run: $(TARGET)
	./$(TARGET) 8.8.8.8  # Przykładowy adres IP do testowania