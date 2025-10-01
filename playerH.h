#ifndef PLAYERH_H
#define PLAYERH_H

#include <string>
using namespace std;

// Base player class
class player {
    private:
        string position;
        int overall_rating;
        int number;
        int age;
        string name;
    public:
        player(string pos, int orat, int num, int a, string n);
        virtual ~player() = default;

        // Getters
        string get_position() const;
        int get_overall_rating() const;
        int get_number() const;
        int get_age() const;
        string get_name() const;

        // Setters
        void set_position(const string& pos);
        void set_overall_rating(int orat);
        void set_number(int num);
        void set_age(int a);
        void set_name(const string& n);
};

// Quarterback
class quarterback : public player {
    private:
        int throw_power;
        int throw_accuracy;
        int speed;
        int passing_vision;
    public:
        quarterback(string pos, int orat, int num, int a, string n,
                    int tp, int ta, int s, int pv);

        int get_throw_power() const;
        int get_throw_accuracy() const;
        int get_speed() const;
        int get_passing_vision() const;

        void set_throw_power(int tp);
        void set_throw_accuracy(int ta);
        void set_speed(int s);
        void set_passing_vision(int pv);
};

// Running back
class runningback : public player {
    private:
        int speed;
        int carrying;
        int elusiveness;
        int blocking;
    public:
        runningback(string pos, int orat, int num, int a, string n,
                    int s, int c, int e, int b);

        int get_speed() const;
        int get_carrying() const;
        int get_elusiveness() const;
        int get_blocking() const;

        void set_speed(int s);
        void set_carrying(int c);
        void set_elusiveness(int e);
        void set_blocking(int b);
};

// Wide receiver
class wide_receiver : public player {
    private:
        int speed;
        int catching;
        int route_running;
    public:
        wide_receiver(string pos, int orat, int num, int a, string n,
                      int s, int c, int rr);

        int get_speed() const;
        int get_catching() const;
        int get_route_running() const;

        void set_speed(int s);
        void set_catching(int c);
        void set_route_running(int rr);
};

// Tight end
class tight_end : public player {
    private:
        int speed;
        int catching;
        int blocking;
    public:
        tight_end(string pos, int orat, int num, int a, string n,
                  int s, int c, int b);

        int get_speed() const;
        int get_catching() const;
        int get_blocking() const;

        void set_speed(int s);
        void set_catching(int c);
        void set_blocking(int b);
};

// Lineman
class lineman : public player {
    private:
        int strength;
        int blocking;
        int tackling;
    public:
        lineman(string pos, int orat, int num, int a, string n,
                int str, int b, int t);

        int get_strength() const;
        int get_blocking() const;
        int get_tackling() const;

        void set_strength(int s);
        void set_blocking(int b);
        void set_tackling(int t);
};

// Kicker
class kicker : public player {
    private:
        int kick_power;
        int kick_accuracy;
    public:
        kicker(string pos, int orat, int num, int a, string n,
               int kp, int ka);

        int get_kick_power() const;
        int get_kick_accuracy() const;

        void set_kick_power(int kp);
        void set_kick_accuracy(int ka);
};

// Punter
class punter : public player {
    private:
        int punt_power;
        int punt_accuracy;
    public:
        punter(string pos, int orat, int num, int a, string n,
               int pp, int pa);

        int get_punt_power() const;
        int get_punt_accuracy() const;

        void set_punt_power(int pp);
        void set_punt_accuracy(int pa);
};

// Defensive back
class defensive_back : public player {
    private:
        int speed;
        int coverage;
        int tackling;
    public:
        defensive_back(string pos, int orat, int num, int a, string n,
                       int s, int c, int t);

        int get_speed() const;
        int get_coverage() const;
        int get_tackling() const;

        void set_speed(int s);
        void set_coverage(int c);
        void set_tackling(int t);
};

// Linebacker
class linebacker : public player {
    private:
        int speed;
        int tackling;
        int coverage;
    public:
        linebacker(string pos, int orat, int num, int a, string n,
                   int s, int t, int c);

        int get_speed() const;
        int get_tackling() const;
        int get_coverage() const;

        void set_speed(int s);
        void set_tackling(int t);
        void set_coverage(int c);
};

// Defensive lineman
class defensive_lineman : public player {
    private:
        int strength;
        int tackling;
        int pass_rushing;
    public:
        defensive_lineman(string pos, int orat, int num, int a, string n,
                          int str, int t, int pr);

        int get_strength() const;
        int get_tackling() const;
        int get_pass_rushing() const;

        void set_strength(int s);
        void set_tackling(int t);
        void set_pass_rushing(int pr);
};

#endif // PLAYERH_H