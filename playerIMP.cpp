#include <algorithm>
#include <cctype>
#include <string>
#include "playerH.h"
using namespace std;

// Position helpers
string position_to_string(Position pos) {
    switch (pos) {
        case Position::QB: return "QB";
        case Position::RB: return "RB";
        case Position::WR: return "WR";
        case Position::TE: return "TE";
        case Position::OL: return "OL";
        case Position::DL: return "DL";
        case Position::LB: return "LB";
        case Position::DB: return "DB";
        case Position::K:  return "K";
        case Position::P:  return "P";
    }
    return "?";
}

string position_full_name(Position pos) {
    switch (pos) {
        case Position::QB: return "Quarterback";
        case Position::RB: return "Running Back";
        case Position::WR: return "Wide Receiver";
        case Position::TE: return "Tight End";
        case Position::OL: return "Offensive Lineman";
        case Position::DL: return "Defensive Lineman";
        case Position::LB: return "Linebacker";
        case Position::DB: return "Defensive Back";
        case Position::K:  return "Kicker";
        case Position::P:  return "Punter";
    }
    return "Unknown";
}

bool position_from_string(const string& s, Position& out) {
    string upper = s;
    transform(upper.begin(), upper.end(), upper.begin(),
              [](unsigned char c) { return toupper(c); });
    for (Position pos : ALL_POSITIONS) {
        if (position_to_string(pos) == upper) {
            out = pos;
            return true;
        }
    }
    return false;
}

bool is_offense(Position pos) {
    return pos == Position::QB || pos == Position::RB || pos == Position::WR ||
           pos == Position::TE || pos == Position::OL;
}

const vector<string>& attribute_names(Position pos) {
    static const vector<string> qb = {"throw power", "throw accuracy", "speed", "passing vision"};
    static const vector<string> rb = {"speed", "carrying", "elusiveness", "blocking"};
    static const vector<string> wr = {"speed", "catching", "route running"};
    static const vector<string> te = {"speed", "catching", "blocking"};
    static const vector<string> ol = {"strength", "blocking", "tackling"};
    static const vector<string> dl = {"strength", "tackling", "pass rushing"};
    static const vector<string> lb = {"speed", "tackling", "coverage"};
    static const vector<string> db = {"speed", "coverage", "tackling"};
    static const vector<string> k = {"kick power", "kick accuracy"};
    static const vector<string> p = {"punt power", "punt accuracy"};
    switch (pos) {
        case Position::QB: return qb;
        case Position::RB: return rb;
        case Position::WR: return wr;
        case Position::TE: return te;
        case Position::OL: return ol;
        case Position::DL: return dl;
        case Position::LB: return lb;
        case Position::DB: return db;
        case Position::K:  return k;
        case Position::P:  return p;
    }
    return qb;
}

shared_ptr<player> make_player(Position pos, int orat, int num, int a,
                               const string& n, const vector<int>& attrs) {
    const vector<int>& v = attrs;
    switch (pos) {
        case Position::QB: return make_shared<quarterback>(orat, num, a, n, v.at(0), v.at(1), v.at(2), v.at(3));
        case Position::RB: return make_shared<runningback>(orat, num, a, n, v.at(0), v.at(1), v.at(2), v.at(3));
        case Position::WR: return make_shared<wide_receiver>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::TE: return make_shared<tight_end>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::OL: return make_shared<lineman>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::DL: return make_shared<defensive_lineman>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::LB: return make_shared<linebacker>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::DB: return make_shared<defensive_back>(orat, num, a, n, v.at(0), v.at(1), v.at(2));
        case Position::K:  return make_shared<kicker>(orat, num, a, n, v.at(0), v.at(1));
        case Position::P:  return make_shared<punter>(orat, num, a, n, v.at(0), v.at(1));
    }
    return nullptr;
}

// Implementations for base player
player::player(Position pos, int orat, int num, int a, string n)
    : position(pos), overall_rating(orat), number(num), age(a), name(n) {}

Position player::get_position() const { return position; }
string player::get_position_name() const { return position_to_string(position); }
int player::get_overall_rating() const { return overall_rating; }
int player::get_number() const { return number; }
int player::get_age() const { return age; }
string player::get_name() const { return name; }

void player::set_overall_rating(int orat) { overall_rating = orat; }
void player::set_number(int num) { number = num; }
void player::set_age(int a) { age = a; }
void player::set_name(const string& n) { name = n; }

// Quarterback implementations
quarterback::quarterback(int orat, int num, int a, string n,
                         int tp, int ta, int s, int pv)
    : player(Position::QB, orat, num, a, n), throw_power(tp), throw_accuracy(ta), speed(s), passing_vision(pv) {}

int quarterback::get_throw_power() const { return throw_power; }
int quarterback::get_throw_accuracy() const { return throw_accuracy; }
int quarterback::get_speed() const { return speed; }
int quarterback::get_passing_vision() const { return passing_vision; }

void quarterback::set_throw_power(int tp) { throw_power = tp; }
void quarterback::set_throw_accuracy(int ta) { throw_accuracy = ta; }
void quarterback::set_speed(int s) { speed = s; }
void quarterback::set_passing_vision(int pv) { passing_vision = pv; }

vector<int> quarterback::get_attributes() const { return {throw_power, throw_accuracy, speed, passing_vision}; }

// Running back implementations
runningback::runningback(int orat, int num, int a, string n,
                         int s, int c, int e, int b)
    : player(Position::RB, orat, num, a, n), speed(s), carrying(c), elusiveness(e), blocking(b) {}

int runningback::get_speed() const { return speed; }
int runningback::get_carrying() const { return carrying; }
int runningback::get_elusiveness() const { return elusiveness; }
int runningback::get_blocking() const { return blocking; }

void runningback::set_speed(int s) { speed = s; }
void runningback::set_carrying(int c) { carrying = c; }
void runningback::set_elusiveness(int e) { elusiveness = e; }
void runningback::set_blocking(int b) { blocking = b; }

vector<int> runningback::get_attributes() const { return {speed, carrying, elusiveness, blocking}; }

// Wide receiver implementations
wide_receiver::wide_receiver(int orat, int num, int a, string n,
                             int s, int c, int rr)
    : player(Position::WR, orat, num, a, n), speed(s), catching(c), route_running(rr) {}

int wide_receiver::get_speed() const { return speed; }
int wide_receiver::get_catching() const { return catching; }
int wide_receiver::get_route_running() const { return route_running; }

void wide_receiver::set_speed(int s) { speed = s; }
void wide_receiver::set_catching(int c) { catching = c; }
void wide_receiver::set_route_running(int rr) { route_running = rr; }

vector<int> wide_receiver::get_attributes() const { return {speed, catching, route_running}; }

// Tight end implementations
tight_end::tight_end(int orat, int num, int a, string n,
                     int s, int c, int b)
    : player(Position::TE, orat, num, a, n), speed(s), catching(c), blocking(b) {}

int tight_end::get_speed() const { return speed; }
int tight_end::get_catching() const { return catching; }
int tight_end::get_blocking() const { return blocking; }

void tight_end::set_speed(int s) { speed = s; }
void tight_end::set_catching(int c) { catching = c; }
void tight_end::set_blocking(int b) { blocking = b; }

vector<int> tight_end::get_attributes() const { return {speed, catching, blocking}; }

// Lineman implementations
lineman::lineman(int orat, int num, int a, string n,
                 int str, int b, int t)
    : player(Position::OL, orat, num, a, n), strength(str), blocking(b), tackling(t) {}

int lineman::get_strength() const { return strength; }
int lineman::get_blocking() const { return blocking; }
int lineman::get_tackling() const { return tackling; }

void lineman::set_strength(int s) { strength = s; }
void lineman::set_blocking(int b) { blocking = b; }
void lineman::set_tackling(int t) { tackling = t; }

vector<int> lineman::get_attributes() const { return {strength, blocking, tackling}; }

// Kicker implementations
kicker::kicker(int orat, int num, int a, string n,
               int kp, int ka)
    : player(Position::K, orat, num, a, n), kick_power(kp), kick_accuracy(ka) {}

int kicker::get_kick_power() const { return kick_power; }
int kicker::get_kick_accuracy() const { return kick_accuracy; }

void kicker::set_kick_power(int kp) { kick_power = kp; }
void kicker::set_kick_accuracy(int ka) { kick_accuracy = ka; }

vector<int> kicker::get_attributes() const { return {kick_power, kick_accuracy}; }

// Punter implementations
punter::punter(int orat, int num, int a, string n,
               int pp, int pa)
    : player(Position::P, orat, num, a, n), punt_power(pp), punt_accuracy(pa) {}

int punter::get_punt_power() const { return punt_power; }
int punter::get_punt_accuracy() const { return punt_accuracy; }

void punter::set_punt_power(int pp) { punt_power = pp; }
void punter::set_punt_accuracy(int pa) { punt_accuracy = pa; }

vector<int> punter::get_attributes() const { return {punt_power, punt_accuracy}; }

// Defensive back implementations
defensive_back::defensive_back(int orat, int num, int a, string n,
                               int s, int c, int t)
    : player(Position::DB, orat, num, a, n), speed(s), coverage(c), tackling(t) {}

int defensive_back::get_speed() const { return speed; }
int defensive_back::get_coverage() const { return coverage; }
int defensive_back::get_tackling() const { return tackling; }

void defensive_back::set_speed(int s) { speed = s; }
void defensive_back::set_coverage(int c) { coverage = c; }
void defensive_back::set_tackling(int t) { tackling = t; }

vector<int> defensive_back::get_attributes() const { return {speed, coverage, tackling}; }

// Linebacker implementations
linebacker::linebacker(int orat, int num, int a, string n,
                       int s, int t, int c)
    : player(Position::LB, orat, num, a, n), speed(s), tackling(t), coverage(c) {}

int linebacker::get_speed() const { return speed; }
int linebacker::get_tackling() const { return tackling; }
int linebacker::get_coverage() const { return coverage; }

void linebacker::set_speed(int s) { speed = s; }
void linebacker::set_tackling(int t) { tackling = t; }
void linebacker::set_coverage(int c) { coverage = c; }

vector<int> linebacker::get_attributes() const { return {speed, tackling, coverage}; }

// Defensive lineman implementations
defensive_lineman::defensive_lineman(int orat, int num, int a, string n,
                                     int str, int t, int pr)
    : player(Position::DL, orat, num, a, n), strength(str), tackling(t), pass_rushing(pr) {}

int defensive_lineman::get_strength() const { return strength; }
int defensive_lineman::get_tackling() const { return tackling; }
int defensive_lineman::get_pass_rushing() const { return pass_rushing; }

void defensive_lineman::set_strength(int s) { strength = s; }
void defensive_lineman::set_tackling(int t) { tackling = t; }
void defensive_lineman::set_pass_rushing(int pr) { pass_rushing = pr; }

vector<int> defensive_lineman::get_attributes() const { return {strength, tackling, pass_rushing}; }
