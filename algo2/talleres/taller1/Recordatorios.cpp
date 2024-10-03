#include <iostream>
#include <list>

using namespace std;


using uint = unsigned int;

// Pre: 0 <= mes < 12
uint dias_en_mes(uint mes) {
    uint dias[] = {
        // ene, feb, mar, abr, may, jun
        31, 28, 31, 30, 31, 30,
        // jul, ago, sep, oct, nov, dic
        31, 31, 30, 31, 30, 31
    };
    return dias[mes - 1];
}

// Ejercicio 7, 8, 9 y 10

// Clase Fecha
class Fecha {
  public:
    // Completar declaraciones funciones
    Fecha(int mes, int dia);
    int mes();
    int dia();
    #if EJ >= 9 // Para ejercicio 9
    bool operator==(Fecha o);
    #endif
    void incrementar_dia();

  private:
    //Completar miembros internos
    int mes_;
    int dia_;
};

void Fecha::incrementar_dia() {
    Fecha::dia_ += 1;
    if (dias_en_mes(Fecha::mes_) < Fecha::dia_){
        if(Fecha::mes_ == 12) {
            Fecha::mes_ = 1;
        }
        else
        {
            Fecha::mes_ += 1;
        }
        Fecha::dia_ = 1;
    }
}

Fecha::Fecha(int mes, int dia): mes_(mes), dia_(dia){}

int Fecha::mes() {
    return Fecha::mes_;
}

int Fecha::dia() {
    return Fecha::dia_;
}

ostream& operator<<(ostream& os, Fecha r) {
    os << r.dia() << '/' << r.mes();
    return os;
}

#if EJ >= 9
bool Fecha::operator==(Fecha o) {
    bool igual_dia = this->dia() == o.dia();
    bool igual_mes = this->mes() == o.mes();
    // Completar iguadad (ej 9)
    return igual_dia && igual_mes;
}
#endif

// Ejercicio 11, 12

// Clase Horario
class Horario {
public:
    Horario(uint hora, uint min);
    uint hora();
    uint min();
    bool operator<(Horario h);
private:
    uint h_;
    uint m_;
};

Horario::Horario(uint hora, uint min) : h_(hora), m_(min){}

uint Horario::hora() {
    return h_;
}

uint Horario::min() {
    return m_;
}

ostream& operator<<(ostream& os, Horario r) {
    os << r.hora() << ':' << r.min();
    return os;
}

bool Horario::operator<(Horario h) {
    bool hora_menor = this->hora() < h.hora();
    bool misma_hora_menor_min = this->hora() == h.hora() && this->min() < h.min();

    return hora_menor || misma_hora_menor_min;
}


// Ejercicio 13

class Recordatorio {
public:
    Recordatorio(Fecha f, Horario h, string mensaje);
    string message();
    Fecha fecha();
    Horario horario();

private:
    Fecha f_;
    Horario h_;
    string m_;
};

string Recordatorio::message() {
    return m_;
}

Fecha Recordatorio::fecha() {
    return f_;
}

Horario Recordatorio::horario() {
    return h_;
}

Recordatorio::Recordatorio(Fecha f, Horario h, string mensaje) : f_(f), h_(h), m_(mensaje) {}

ostream& operator<<(ostream& os, Recordatorio r) {
    os << r.message() << " @ " << r.fecha() << " " << r.horario();
    return os;
}


// Ejercicio 14
class Agenda {
public:
    Agenda(Fecha fecha_inicial);
    void agregar_recordatorio(Recordatorio rec);
    void incrementar_dia();
    list<Recordatorio> recordatorios_de_hoy();
    Fecha hoy();
private:
    Fecha f_;
    list<Recordatorio> recordatorios_;
};

Agenda::Agenda(Fecha fecha_inicial) : f_(fecha_inicial) {}

void Agenda::agregar_recordatorio(Recordatorio rec) {
    recordatorios_.push_back(rec);
    recordatorios_.sort([](Recordatorio a, Recordatorio b){
        return a.horario() < b.horario();
    });
}

void Agenda::incrementar_dia() {
    f_.incrementar_dia();
}

Fecha Agenda::hoy() {
    return f_;
}

list<Recordatorio> Agenda::recordatorios_de_hoy() {
    list<Recordatorio> resultado;
    for (Recordatorio r: recordatorios_) {
        if (r.fecha() == Agenda::hoy()) {
            resultado.push_back(r);
        }
    }
    return resultado;
}

ostream& operator<<(ostream& os, Agenda r) {
    os << r.hoy() << endl;
    os<<"=====" << endl;
    for (Recordatorio re : r.recordatorios_de_hoy()) {
        os << re << endl;
    }
    return os;
}

